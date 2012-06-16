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

class Bezier {
private:
    i32                     level;
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
    return (vecs[((visCluster * szVecs) + (testCluster >> 3))] & (1 << (testCluster & 7))) != 0;
}

q3maploader::q3maploader(FileBinary& file)
    : f(file)
    , visFaces(NULL)
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

    q3lump lump_vis = read_lump(16);
    f.set_position(lump_vis.offset);
    visibility.nVecs = f.read_type<i32>();
    visibility.szVecs = f.read_type<i32>();
    visibility.vecs = new u8[visibility.nVecs * visibility.szVecs];
    for(int i=0; i<visibility.nVecs * visibility.szVecs; i++)
        visibility.vecs[i] = f.read_type<u8>();

    visFaces = new u8[faces.size()];
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
        const f32 distance = Dot(plane.normal, camPos) - plane.dist;
        if (distance >= 0) {
            index = node.frontPlaneIndex;
        } else {
            index = node.backPlaneIndex;
        }
    }
    return -index - 1;
}

void q3maploader::render(Camera& camera, Program* pr, TextureLoader& txloader) const {

    i32 cameraLeafIndex = findLeaf(camera.position());
    const q3leaf& cameraLeaf = leafs[cameraLeafIndex];
    memset(visFaces, 0, faces.size());
    std::deque<int> visibleFaces;
    for (u32 i=0; i<leafs.size(); i++) {
        const q3leaf& leaf = leafs[i];
        if (visibility.isClusterVisible(cameraLeaf.cluster, leaf.cluster)) {
            Vector3 mins(static_cast<f32>(leaf.mins[0]), static_cast<f32>(leaf.mins[1]), static_cast<f32>(leaf.mins[2]));
            Vector3 maxs(static_cast<f32>(leaf.maxs[0]), static_cast<f32>(leaf.maxs[1]), static_cast<f32>(leaf.maxs[2]));
            if (camera.IsInFrustum(mins, maxs)) {
                for (i32 j=0; j<leaf.numberOfLeafFaces; j++) {
                    int faceIndex = leafFaces[leaf.leafFace + j];
                    if (visFaces[faceIndex] == 0)
                    {
                        visibleFaces.push_back(faceIndex);
                        visFaces[faceIndex] = 1;
                    }
                }
            }
        }
    }

    for (u32 i=0; i<visibleFaces.size(); i++) {
        int faceIndex = visibleFaces[i];
        const q3face& face = faces[faceIndex];
        Texture* t = txloader.Load((char*)textures[face.textureID].name);
        if (t == NULL) continue;
        pr->set_uniform(base::opengl::UniformVars::Diffuse, t);
        if (face.type == 1) {
            render_polygons(face, pr);
        } else if (face.type == 2) {
            render_patch(face, pr);
        }
    }
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

    for (int y = 0; y < numOfPatchesHigh; y++) {
        for (int x = 0; x < numOfPatchesWide; x++) {
            Bezier b;
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
            b.tessellate(10);
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
    for (i32 i = 0; i <= L; ++i) {
        const f32 a = i / (f32)L;
        const f32 b = 1 - a;

        vertex[i] =
            controls[0] * (b * b) + 
            controls[3] * (2 * b * a) +
            controls[6] * (a * a);
    }

    for (i32 i = 1; i <= L; ++i) {
        const f32 a = i / (f32)L;
        const f32 b = 1.0 - a;

        q3vertex temp[3];

        for (i32 j = 0; j < 3; ++j) {
            int k = 3 * j;
            temp[j] =
                controls[k + 0] * (b * b) + 
                controls[k + 1] * (2 * b * a) +
                controls[k + 2] * (a * a);
        }

        for(i32 j = 0; j <= L; ++j) {
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
    for (i32 row = 0; row < L; ++row) {
        for(int col = 0; col <= L; ++col)   {
            indexes[(row * (L + 1) + col) * 2 + 1] = row * L1 + col;
            indexes[(row * (L + 1) + col) * 2] = (row + 1) * L1 + col;
        }
    }

    trianglesPerRow.resize(L);
    rowIndexes.resize(L);
    for (i32 row = 0; row < L; ++row) {
        trianglesPerRow[row] = 2 * L1;
        rowIndexes[row] = &indexes[row * 2 * L1];
    }
}

void Bezier::render(Program* program) {
    AttributeBinding binding = program->binding();
    bind_attr(program, vertex[0]);

    for(int i=0; i<level; i++)
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
