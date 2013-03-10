#include "q3map.h"

#include <string.h>
#include <deque>
#include <set>
#include <map>
#include "render/camera.h"
#include "render/glcontext.h"
#include "render/gpuprogram.h"
#include "render/mesh.h"
#include "render/texture.h"
#include "render/statistics.h"
#include "base/timer.h"

using base::math::Vector3;
using base::math::Vector4;

namespace base
{
namespace opengl
{

void bind_attr( GpuProgram* pr, const q3vertex& vertexes );

void swizzle( Vector3& v )
{
    std::swap(v.y, v.z);
    std::swap(v.z, v.x);
}

void swizzle( i32* v )
{
    std::swap(v[1], v[2]);
    std::swap(v[2], v[0]);
}

class Bezier
{
private:
    u32                     level;
    std::vector<q3vertex>   vertex;
    std::vector<u32>        indexes;
    std::vector<i32>        trianglesPerRow;
    std::vector<u32*>       rowIndexes;

public:
    q3vertex                controls[9];

    void tessellate( u32 level );
    void render( GpuProgram* program );
};


bool q3visibility::isClusterVisible( i32 visCluster, i32 testCluster ) const
{
    if ( visCluster < 0 ) {
        return true;
    }

    return ( vecs[visCluster * szVecs + ( testCluster >> 3 )] & ( 1 << ( testCluster & 7 ) ) ) != 0;
}

q3maploader::q3maploader( FileBinary& file )
    : f( file )
    , visFaces( NULL )
#ifdef USE_PVS
    , _frame_index( 1 )
    , _camera_cluster( -2 )
#endif
{
}

q3maploader::~q3maploader()
{
    for( u32 i = 0; i < lm_textures.size(); i++ )
        delete lm_textures[i];
    delete[] visFaces;
}

void q3maploader::load()
{
    check_header();
    textures    = read<q3texture>( 1 );
    planes      = read<q3plane>( 2 );
    nodes       = read<q3node>( 3 );
    leafs       = read<q3leaf>( 4 );
    leafFaces   = read<u32>( 5 );
    vertexes    = read<q3vertex>( 10 );
    faces       = read<q3face>( 13 );
    faceIndexes = read<u32>( 11 );

    for( size_t i=0; i<planes.size(); i++ )
        swizzle( planes[i].normal );
    for( size_t i=0; i<nodes.size(); i++ ) {
        swizzle( nodes[i].mmin );
        swizzle( nodes[i].mmax );
    }
    for( size_t i = 0; i < faces.size(); i++ ) {
        swizzle( faces[i].normal );
    }

    for( size_t i = 0; i < vertexes.size(); i++ ) {
        swizzle( vertexes[i].pos );
        swizzle( vertexes[i].normal );
        //vertexes[i].surfaceUV.x = 1 - vertexes[i].surfaceUV.x;
    }

    for( size_t i=0; i<leafs.size(); i++ ) {
        swizzle( leafs[i].mins );
        swizzle( leafs[i].maxs );
    }

    q3lump lump_vis = read_lump( 16 );
    f.set_position( lump_vis.offset );
    visibility.nVecs = f.read_type<i32>();
    visibility.szVecs = f.read_type<i32>();
    i32 visDataSize = visibility.nVecs * visibility.szVecs;
    visibility.vecs = new u8[visDataSize];
    f.read( visibility.vecs, visDataSize );
    
    visFaces = new u8[faces.size()];
    tree.resize( nodes.size() + leafs.size() );
    Node* firstNode = &tree.front();
    Node* firstLeaf = firstNode + nodes.size();

    std::vector<q3lightmap> lm = read<q3lightmap>( 14 );
    lm_textures.resize( lm.size() );
    TextureInfo ti;
    ti.MinFilter = TextureMinFilters::LINEAR;
    ti.GenerateMipmap = true;
    ti.Pixel = PixelTypes::RGB;
    ti.Width = 128;
    ti.Height = 128;
    for( u32 i = 0; i < lm.size(); i++ )    {
        Texture*& t = lm_textures[i];
        t = new Texture;
        t->GenerateFromBuffer( ti, (u8*)lm[i].data );
    }

    for( size_t i = 0; i < nodes.size(); i++ ) {
        const q3node& node = nodes[i];
        Node& treeNode = tree[i];
        treeNode.maxs = node.maxs();
        treeNode.mins = node.mins();
        const q3plane& plane = planes[node.planeIndex];
        treeNode.plane.set( plane.normal.x, plane.normal.y, plane.normal.z, plane.dist );

        for( size_t j = 0; j < 2; j++ ) {
            i32 p = node.children[j];

            if ( p >= 0 ) {
                treeNode.children[j] = firstNode + p;
            } else {
                treeNode.children[j] = firstLeaf + ( -p - 1 );
            }

            treeNode.children[j]->parent = &treeNode;
        }
    }

    for( size_t i = 0; i < leafs.size(); i++ ) {
        const q3leaf& leaf = leafs[i];
        Node& treeNode = *( firstLeaf + i );
        treeNode.maxs = leaf.maxv();
        treeNode.mins = leaf.minv();
        treeNode.cluster = leaf.cluster;
        treeNode.area = leaf.area;
        treeNode.firstFace = leaf.leafFace;
        treeNode.numFaces = leaf.numberOfLeafFaces;
    }
}

void q3maploader::PreloadTextures( TextureLoader& textureLoader )
{
    Texture* checker = textureLoader.Load( "checker.png" );

    d_textures.resize( textures.size() );
    for ( size_t i = 0; i < textures.size(); i++ ) {
        Texture* t = textureLoader.Load( ( char* )textures[i].name );
        d_textures[i] = (t == NULL) ? checker : t;
    }
}

void q3maploader::check_header()
{
    f.set_position( 0 );
    q3header hdr = f.read_type<q3header>();
    ASSERT( strncmp( reinterpret_cast<char*>( hdr.magic ), "IBSP", 4 ) == 0 );
    ASSERT( hdr.version == 0x2e );
}

q3lump q3maploader::read_lump( i32 index )
{
    f.set_position( index * sizeof( q3lump ) + sizeof( q3header ) );
    return f.read_type<q3lump>();
}

i32 q3maploader::findLeaf( const Vector3& camPos ) const
{
    i32 index = 0;

    while ( index >= 0 ) {
        const q3node&  node  = nodes[index];
        const q3plane& plane = planes[node.planeIndex];
        const f32 distance = plane.side( camPos );

        if ( distance >= 0 ) {
            index = node.frontPlaneIndex;
        } else {
            index = node.backPlaneIndex;
        }
    }

    return -index - 1;
}

void q3maploader::ComputePossibleVisible( const Vector3& cameraPos )
{
    i32 cameraLeafIndex = findLeaf( cameraPos );
    const q3leaf& cameraLeaf = leafs[cameraLeafIndex];

    if ( cameraLeaf.cluster == _camera_cluster ) {
        return;
    }

    _camera_cluster = cameraLeaf.cluster;
    _frame_index++;

    for( size_t i = nodes.size(); i < tree.size(); i++ ) {
        if ( !visibility.isClusterVisible( cameraLeaf.cluster, tree[i].cluster ) ) {
            continue;
        }

        tree[i].frame = _frame_index;
        Node* parent = tree[i].parent;

        while( parent != NULL ) {
            if ( parent->frame == _frame_index ) {
                break;
            }

            parent->frame = _frame_index;
            parent = parent->parent;
        }
    }
}

void q3maploader::AddVisibleNode( Node* node )
{
    for ( i32 j = node->firstFace; j < node->firstFace + node->numFaces; j++ ) {
        int faceIndex = leafFaces[j];

        if ( visFaces[faceIndex] == 0 ) {
            _visible_faces.push_back( faceIndex );
            visFaces[faceIndex] = 1;
        }
    }

    //_wb.setMinMaxPoints(node->mins, node->maxs);
    //_wb.Draw(_program);
}

void q3maploader::ComputeVisible_R( const Camera& camera, Node* node, u32 planeMask )
{
    if ( node->frame != _frame_index ) {
        return;
    }

    const Vector3& mins = node->mins;
    const Vector3& maxs = node->maxs;
    for ( u32 i = 0; i < 6; i++ ) {
        u32 mask = 1 << i;

        if ( ( planeMask & mask ) == 0 ) {
            continue;
        }

        const math::Plane& p = camera.planes()[i];

        u8 r = p.BoxOnPlaneSide( mins, maxs );

        if ( r == 2 ) {
            return;
        }

        if ( r == 1 ) {
            planeMask &= ~mask;
        }
    }

    if ( node->children[0] == NULL && node->children[1] == NULL ) {
        AddVisibleNode( node );
        return;
    }

    for( u32 j = 0; j < 2; j++ ) {
        if ( node->children[j] != NULL ) {
            ComputeVisible_R( camera, node->children[j], planeMask );
        }
    }
}

typedef std::pair<Texture*, int> TextureFace;
bool comp_texture_array(const TextureFace& a, const TextureFace& b)
{
    return a.first < b.first;
}

void q3maploader::render( const Camera& camera, GpuProgram* pr, TextureLoader& txloader )
{
    Timer tt;
    tt.Reset();

    memset( visFaces, 0, faces.size() );
    _visible_faces.clear();
    _visible_faces.reserve( faces.size() );
    ComputePossibleVisible( camera.position() );
    _program = pr;
    ComputeVisible_R( camera, &tree.front(), ( 1 << 7 ) - 1 );
    //return;
    //float t1 = tt.Reset();

    std::vector<TextureFace> sorted;
    size_t ii = _visible_faces.size();
    sorted.reserve(ii);
    for ( u32 i = 0; i < ii; i++ ) {
        int faceIndex = _visible_faces[i];
        const q3face& face = faces[faceIndex];
        Texture* t = d_textures[face.textureID];
        sorted.push_back( std::pair<Texture*, int>( t, faceIndex ) );
    }
    std::sort(sorted.begin(), sorted.end(), comp_texture_array);
    //float t2 = tt.Reset();

    int switches = 0;
    int patches = 0;

    u32 lightMapID = 0xffffffff;
    u32 diffuesID = 0xffffffff;
    for ( std::vector<TextureFace>::iterator ita = sorted.begin(); ita != sorted.end(); ++ita ) {

        Texture* t = ( *ita ).first;
        if (diffuesID != t->id())
        {
            pr->set_uniform( base::opengl::UniformVars::Diffuse, t );
            diffuesID = t->id();
            switches++;
        }

        int faceIndex = ( *ita ).second;
        const q3face& face = faces[faceIndex];

        if ( face.lightmapID >= 0 && lightMapID != face.lightmapID ) {
            lightMapID = face.lightmapID;
            Texture* lightmap = lm_textures[face.lightmapID];
            pr->set_uniform( base::opengl::UniformVars::Lightmap, lightmap );
            switches++;
        }

        if ( face.type == 1 ) {
            render_polygons( face, pr );
        } else if ( face.type == 2 ) {
            render_patch( face, pr );
            patches++;
        }
    }
    //float t3 = tt.Reset();

//    std::cout
//    << Stats::polygons() << " "
//    //<< _visible_faces.size() << " "
//    //<< switches << " "
//    << patches << " "
//    << Stats::drawcalls() << " "
//    << (int)t1 << " " << (int)t2 << " " << (int)t3 << " "
//    << std::endl;
}

void q3maploader::render_polygons( const q3face& face, GpuProgram* pr ) const
{
    bind_attr( pr, vertexes[face.vertexIndex] );
    glDrawElements( GL_TRIANGLES, face.numOfFaceIndices, GL_UNSIGNED_INT, &faceIndexes[face.faceVertexIndex] );
    Stats::add_polygons( face.numOfFaceIndices / 3 );
}

void q3maploader::render_patch( const q3face& face, GpuProgram* pr ) const
{
    i32 numOfPatchesWide = ( face.patchWidth - 1 ) >> 1;
    i32 numOfPatchesHigh = ( face.patchHeight - 1 ) >> 1;
    i32 patchWidth = face.patchWidth;
    Bezier b;

    for ( int y = 0; y < numOfPatchesHigh; y++ ) {
        for ( int x = 0; x < numOfPatchesWide; x++ ) {
            int offset = face.vertexIndex + ( y * face.patchWidth << 1 ) + ( x << 1 );
            b.controls[0] = vertexes[offset];
            b.controls[1] = vertexes[offset + 1];
            b.controls[2] = vertexes[offset + 2];
            b.controls[3] = vertexes[offset + patchWidth];
            b.controls[4] = vertexes[offset + patchWidth + 1];
            b.controls[5] = vertexes[offset + patchWidth + 2];
            b.controls[6] = vertexes[offset + ( patchWidth << 1 )];
            b.controls[7] = vertexes[offset + ( patchWidth << 1 ) + 1];
            b.controls[8] = vertexes[offset + ( patchWidth << 1 ) + 2];
            b.tessellate( 8 );
            b.render( pr );
        }
    }
}

void bind_attr( GpuProgram* pr, const q3vertex& vertex )
{
    u32 bindPos = VertexAttrs::GetAttributeLocation(VertexAttrs::tagPosition);
    glEnableVertexAttribArray(bindPos);
    glVertexAttribPointer(
        bindPos,
        3,
        GL_FLOAT,
        GL_FALSE,
        sizeof( q3vertex ),
        ( const u8* )&vertex.pos );
    u32 bindTex = VertexAttrs::GetAttributeLocation(VertexAttrs::tagTexture);
    glEnableVertexAttribArray(bindTex);
    glVertexAttribPointer(
        bindTex,
        2,
        GL_FLOAT,
        GL_FALSE,
        sizeof( q3vertex ),
        ( const u8* )&vertex.surfaceUV );
    /*
        u32 bindNorm = VertexAttrs::GetAttributeLocation(VertexAttrs::tagNormal);
        glVertexAttribPointer(
            bindNorm,
            3,
            GL_FLOAT,
            GL_FALSE,
            sizeof(q3vertex),
            (const u8*)&vertex.normal);
    */
        u32 bindLM = VertexAttrs::GetAttributeLocation(VertexAttrs::tagTangent);
        glEnableVertexAttribArray(bindLM);
        glVertexAttribPointer(
            bindLM,
            2,
            GL_FLOAT,
            GL_FALSE,
            sizeof(q3vertex),
            (const u8*)&vertex.lightmapUV);
}

void Bezier::tessellate( u32 L )
{
    level = L;
    const u32 L1 = L + 1;
    if (vertex.size() < L1 * L1)
        vertex.resize( L1 * L1 );

    // Compute the vertices
    for ( u32 i = 0; i <= L; ++i ) {
        const f32 a = i / ( f32 )L;
        const f32 b = 1 - a;
        vertex[i] =
            controls[0] * ( b * b ) +
            controls[3] * ( 2 * b * a ) +
            controls[6] * ( a * a );
    }

    for ( u32 i = 1; i <= L; ++i ) {
        const f32 a = i / ( f32 )L;
        const f32 b = 1.0f - a;
        q3vertex temp[3];

        for ( u32 j = 0; j < 3; ++j ) {
            int k = 3 * j;
            temp[j] =
                controls[k + 0] * ( b * b ) +
                controls[k + 1] * ( 2 * b * a ) +
                controls[k + 2] * ( a * a );
        }

        for( u32 j = 0; j <= L; ++j ) {
            const f32 a = j / ( f32 )L;
            const f32 b = 1.0f - a;
            vertex[i * L1 + j] =
                temp[0] * ( b * b ) +
                temp[1] * ( 2 * b * a ) +
                temp[2] * ( a * a );
        }
    }

    // Compute the indices
    indexes.resize( L * ( L + 1 ) * 2 );

    for ( u32 row = 0; row < L; ++row ) {
        for( u32 col = 0; col <= L; ++col )   {
            indexes[( row * ( L + 1 ) + col ) * 2 + 1] = row * L1 + col;
            indexes[( row * ( L + 1 ) + col ) * 2] = ( row + 1 ) * L1 + col;
        }
    }

    trianglesPerRow.resize( L );
    rowIndexes.resize( L );

    for ( u32 row = 0; row < L; ++row ) {
        trianglesPerRow[row] = 2 * L1;
        rowIndexes[row] = &indexes[row * 2 * L1];
    }
}

void Bezier::render( GpuProgram* program )
{
    bind_attr( program, vertex[0] );

    for( u32 i = 0; i < level; i++ ) {
        glDrawElements( GL_TRIANGLE_STRIP,
                        trianglesPerRow[i],
                        GL_UNSIGNED_INT,
                        rowIndexes[i] );
        Stats::add_polygons( trianglesPerRow[i] );
    }
}

} // namespace opengl
} // namespace base
