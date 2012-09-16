#include "q3map.h"

#include <assert.h>
#include <string.h>
#include <deque>
#include <set>
#include "renderer/camera.h"
#include "renderer/glcontext.h"
#include "renderer/glprogram.h"
#include "renderer/mesh.h"
#include "renderer/gltexture.h"
#include "renderer/statistics.h"

namespace base {
namespace opengl {

void bind_attr(Program* pr, const q3vertex& vertexes);

void swizzle(Vector3& v) {
    f32 temp = v.y;
    v.y = v.z;
    v.z = -temp;
}

void swizzle(i32* v) {
    i32 temp = v[1];
    v[1] = v[2];
    v[2] = -temp;
}

class Bezier {
private:
    u32                     level;
    std::vector<q3vertex>   vertex;
    std::vector<u32>        indexes;
    std::vector<i32>        trianglesPerRow;
    std::vector<u32*>       rowIndexes;

public:
    q3vertex                controls[9];

    void tessellate(u32 level);
    void render(Program* program);
};


bool q3visibility::isClusterVisible(i32 visCluster, i32 testCluster) const
{
    if (visCluster < 0) {
        return true;
    }
    return (vecs[visCluster * szVecs + (testCluster >> 3)] & (1 << (testCluster & 7))) != 0;
}

q3maploader::q3maploader(FileBinary& file)
    : f(file)
    , visFaces(NULL)
    , _frame_index(1)
    , _camera_cluster(-2)
{
}

q3maploader::~q3maploader()
{
    delete[] visFaces;
}

void q3maploader::load() {
    check_header();
    textures    = read<q3texture>(1);
    planes      = read<q3plane>(2);
    nodes       = read<q3node>(3);
    leafs       = read<q3leaf>(4);
    leafFaces   = read<u32>(5);
    vertexes    = read<q3vertex>(10);
    faces       = read<q3face>(13);
    faceIndexes = read<u32>(11);

//    for( size_t i=0; i<planes.size(); i++ )
//        swizzle( planes[i].normal );
//    for( size_t i=0; i<nodes.size(); i++ ) {
//        swizzle( nodes[i].mmin );
//        swizzle( nodes[i].mmax );
//    }
    for( size_t i=0; i<faces.size(); i++ ) {
//        swizzle( faces[i].normal );
    }
    for( size_t i=0; i<vertexes.size(); i++ ) {
//        swizzle( vertexes[i].pos );
//        swizzle( vertexes[i].normal );
//        vertexes[i].surfaceUV.x = 1 - vertexes[i].surfaceUV.x;
    }
//    for( size_t i=0; i<leafs.size(); i++ ) {
//        swizzle( leafs[i].mins );
//        swizzle( leafs[i].maxs );
//    }

    q3lump lump_vis = read_lump( 16 );
    f.set_position(lump_vis.offset);
    visibility.nVecs = f.read_type<i32>();
    visibility.szVecs = f.read_type<i32>();
    i32 visDataSize = visibility.nVecs * visibility.szVecs;
    visibility.vecs = new u8[visDataSize];
    f.read(visibility.vecs, visDataSize);

    visFaces = new u8[faces.size()];

    tree.resize(nodes.size() + leafs.size());
    Node* firstNode = &tree.front();
    Node* firstLeaf = firstNode + nodes.size();
    for( size_t i=0; i<nodes.size(); i++ ) {
        const q3node& node = nodes[i];
        Node& treeNode = tree[i];
        treeNode.maxs = node.maxs();
        treeNode.mins = node.mins();
        const q3plane& plane = planes[node.planeIndex];
        treeNode.plane.set(plane.normal.x, plane.normal.y, plane.normal.z, plane.dist);

        for( size_t j=0; j<2; j++ ) {
            i32 p = node.children[j];
            if ( p >= 0 )
                treeNode.children[j] = firstNode + p;
            else
                treeNode.children[j] = firstLeaf + (-p-1);
            treeNode.children[j]->parent = &treeNode;
        }
    }
    
    for( size_t i=0; i<leafs.size(); i++ ) {
        const q3leaf& leaf = leafs[i];
        Node& treeNode = *(firstLeaf + i);
        treeNode.maxs = leaf.maxv();
        treeNode.mins = leaf.minv();
        treeNode.cluster = leaf.cluster;
        treeNode.area = leaf.area;
        treeNode.firstFace = leaf.leafFace;
        treeNode.numFaces = leaf.numberOfLeafFaces;
    }
    std::cout << "Root max: " << tree.front().maxs << " min: " << tree.front().mins << std::endl;
}

void q3maploader::PreloadTextures( TextureLoader& textureLoader ) {

    for ( size_t i = 0; i < textures.size(); i++ ) {
        textureLoader.Load( (char*)textures[i].name );
    }
}

void q3maploader::check_header() {
    f.set_position(0);
    q3header hdr = f.read_type<q3header>();
    assert(strncmp(reinterpret_cast<char*>(hdr.magic), "IBSP", 4) == 0);
    assert(hdr.version == 0x2e);
}

q3lump q3maploader::read_lump(i32 index) {
    f.set_position(index * sizeof(q3lump) + sizeof(q3header));
    return f.read_type<q3lump>();
}

i32 q3maploader::findLeaf(const Vector3& camPos) const {
    i32 index = 0;
    while (index >= 0) {
        const q3node&  node  = nodes[index];
        const q3plane& plane = planes[node.planeIndex];
        const f32 distance = plane.side(camPos);
        if (distance >= 0) {
            index = node.frontPlaneIndex;
        } else {
            index = node.backPlaneIndex;
        }
    }
    return -index - 1;
}

void q3maploader::ComputePossibleVisible( const Vector3& cameraPos ) {
    i32 cameraLeafIndex = findLeaf( cameraPos );
    const q3leaf& cameraLeaf = leafs[cameraLeafIndex];
    if ( cameraLeaf.cluster == _camera_cluster )
        return;
    _camera_cluster = cameraLeaf.cluster;
    _frame_index++;
    int vvisible = 0;
    for( size_t i=nodes.size(); i<tree.size(); i++) {
        if ( !visibility.isClusterVisible( cameraLeaf.cluster, tree[i].cluster ) )
            continue;
        vvisible++;
        tree[i].frame = _frame_index;
        Node* parent = tree[i].parent;
        while( parent != NULL ) {
            if ( parent->frame == _frame_index )
                break;
            parent->frame = _frame_index;
            parent = parent->parent;
        }
    }
}

void q3maploader::AddVisibleNode( Node* node ) {
    for ( i32 j=node->firstFace; j<node->firstFace+node->numFaces; j++ ) {
        int faceIndex = leafFaces[j];
        if ( visFaces[faceIndex] == 0 )
        {
            _visible_faces.push_back( faceIndex );
            visFaces[faceIndex] = 1;
        }
    }
    //_wb.setMinMaxPoints(node->mins, node->maxs);
    //_wb.Draw(_program);
}

void q3maploader::ComputeVisible_R( const Camera& camera, Node* node, u32 planeMask ) {
    if ( node->frame != _frame_index )
        return;
    if (node->children[0] == NULL && node->children[1] == NULL) {
        AddVisibleNode( node );
        return;
    }

    const Vector3& mins = node->mins;
    const Vector3& maxs = node->maxs;

    for ( u32 i=0; i<6; i++ ) {
        u32 mask = 1 << i;
        if ( planeMask & mask == 0 ) continue;

        const Plane& p = camera.planes()[i];
        u8 r = p.BoxOnPlaneSide(mins, maxs);
        
        if (r == 2)
            return;
        if (r == 1)
            planeMask &= ~mask;
    }

    for( u32 j=0; j<2; j++ ) {
        if ( node->children[j] != NULL )
            ComputeVisible_R( camera, node->children[j], planeMask );
    }

}

void q3maploader::render(const Camera& camera, Program* pr, TextureLoader& txloader) {

    memset( visFaces, 0, faces.size() );
    _visible_faces.clear();
    ComputePossibleVisible( camera.position() );
    _program = pr;
    ComputeVisible_R( camera, &tree.front(), ( 1 << 7 )-1 );
    //return;

    for ( u32 i=0; i<_visible_faces.size(); i++ ) {
        int faceIndex = _visible_faces[i];
        const q3face& face = faces[faceIndex];

        Texture* t = txloader.Load( (char*)textures[face.textureID].name );
        if ( t == NULL )
            t = txloader.Load( "checker.png" );
        pr->set_uniform(base::opengl::UniformVars::Diffuse, t);

        if ( face.type == 1 ) {
            render_polygons( face, pr );
        } else if ( face.type == 2 ) {
            render_patch( face, pr );
        }
    }
    std::cout << Stats::polygons() << std::endl;
}

void q3maploader::render_polygons(const q3face& face, Program* pr) const 
{
    bind_attr(pr, vertexes[face.vertexIndex]);
    
    glDrawElements(GL_TRIANGLES, face.numOfFaceIndices, GL_UNSIGNED_INT, &faceIndexes[face.faceVertexIndex]); 
    Stats::add_polygons(face.numOfFaceIndices / 3);
}

void q3maploader::render_patch(const q3face& face, Program* pr) const
{
    i32 numOfPatchesWide = (face.patchWidth - 1) >> 1;
    i32 numOfPatchesHigh = (face.patchHeight - 1) >> 1;
    i32 patchWidth = face.patchWidth;

    Bezier b;
    for (int y = 0; y < numOfPatchesHigh; y++) {
        for (int x = 0; x < numOfPatchesWide; x++) {
            int offset = face.vertexIndex + (y * face.patchWidth << 1) + (x << 1);

            b.controls[0] = vertexes[offset];
            b.controls[1] = vertexes[offset + 1];
            b.controls[2] = vertexes[offset + 2];
            b.controls[3] = vertexes[offset + patchWidth];
            b.controls[4] = vertexes[offset + patchWidth + 1];
            b.controls[5] = vertexes[offset + patchWidth + 2];
            b.controls[6] = vertexes[offset + (patchWidth << 1)];
            b.controls[7] = vertexes[offset + (patchWidth << 1) + 1];
            b.controls[8] = vertexes[offset + (patchWidth << 1) + 2];
            b.tessellate(5);
            b.render(pr);
        }
    }
}

void bind_attr(Program* pr, const q3vertex& vertex)
{
    AttributeBinding binding = pr->binding();
    u32 bindPos = binding[VertexAttrs::tagPosition];
    glVertexAttribPointer(
        bindPos,
        3,
        GL_FLOAT,
        GL_FALSE,
        sizeof(q3vertex),
        (const u8*)&vertex.pos);
    
    u32 bindTex = binding[VertexAttrs::tagTexture];
    glVertexAttribPointer(
        bindTex,
        2,
        GL_FLOAT,
        GL_FALSE,
        sizeof(q3vertex),
        (const u8*)&vertex.surfaceUV);

    u32 bindNorm = binding[VertexAttrs::tagNormal];
    glVertexAttribPointer(
        bindNorm,
        3,
        GL_FLOAT,
        GL_FALSE,
        sizeof(q3vertex),
        (const u8*)&vertex.normal);

    u32 bindLM = binding[VertexAttrs::tagTangent];
    glVertexAttribPointer(
        bindLM,
        2,
        GL_FLOAT,
        GL_FALSE,
        sizeof(q3vertex),
        (const u8*)&vertex.lightmapUV);
}

void Bezier::tessellate(u32 L) {
    level = L;

    const u32 L1 = L + 1;

    vertex.resize(L1 * L1);

    // Compute the vertices
    for (u32 i = 0; i <= L; ++i) {
        const f32 a = i / (f32)L;
        const f32 b = 1 - a;

        vertex[i] =
            controls[0] * (b * b) + 
            controls[3] * (2 * b * a) +
            controls[6] * (a * a);
    }

    for (u32 i = 1; i <= L; ++i) {
        const f32 a = i / (f32)L;
        const f32 b = 1.0 - a;

        q3vertex temp[3];

        for (u32 j = 0; j < 3; ++j) {
            int k = 3 * j;
            temp[j] =
                controls[k + 0] * (b * b) + 
                controls[k + 1] * (2 * b * a) +
                controls[k + 2] * (a * a);
        }

        for(u32 j = 0; j <= L; ++j) {
            const f32 a = j / (f32)L;
            const f32 b = 1.0f - a;

            vertex[i * L1 + j]=
                temp[0] * (b * b) + 
                temp[1] * (2 * b * a) +
                temp[2] * (a * a);
        }
    }

    // Compute the indices
    indexes.resize(L * (L + 1) * 2);
    for (u32 row = 0; row < L; ++row) {
        for(int col = 0; col <= L; ++col)   {
            indexes[(row * (L + 1) + col) * 2 + 1] = row * L1 + col;
            indexes[(row * (L + 1) + col) * 2] = (row + 1) * L1 + col;
        }
    }

    trianglesPerRow.resize(L);
    rowIndexes.resize(L);
    for (u32 row = 0; row < L; ++row) {
        trianglesPerRow[row] = 2 * L1;
        rowIndexes[row] = &indexes[row * 2 * L1];
    }
}

void Bezier::render(Program* program) {
    AttributeBinding binding = program->binding();
    bind_attr(program, vertex[0]);

    for(u32 i=0; i<level; i++)
    {
        glDrawElements(GL_TRIANGLE_STRIP,
            trianglesPerRow[i],
            GL_UNSIGNED_INT,
            rowIndexes[i]); 
        Stats::add_polygons(trianglesPerRow[i]);
    }
}

} // namespace opengl
} // namespace base
