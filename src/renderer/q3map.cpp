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
        const double distance = Dot(plane.normal, camPos) - plane.dist;
        if (distance >= 0) {
            index = node.frontPlaneIndex;
        } else {
            index = node.backPlaneIndex;
        }
    }
    return -index - 1;
}

void q3maploader::render(Camera& camera, Program* pr, TextureLoader& txloader) const {
    AttributeBinding binding = pr->binding();

    i32 cameraLeafIndex = findLeaf(camera.position());
    const q3leaf& cameraLeaf = leafs[cameraLeafIndex];
    memset(visFaces, 0, faces.size());
    std::deque<int> visibleFaces;
    for (u32 i=0; i<leafs.size(); i++) {
        const q3leaf& leaf = leafs[i];
        if (visibility.isClusterVisible(cameraLeaf.cluster, leaf.cluster)) {
            Vector3 mins(static_cast<f32>(leaf.mins[0]), static_cast<f32>(leaf.mins[1]), static_cast<f32>(leaf.mins[2]));
            Vector3 maxs(static_cast<f32>(leaf.maxs[0]), static_cast<f32>(leaf.maxs[1]), static_cast<f32>(leaf.maxs[2]));
            if (camera.IsInFrustum2(mins, maxs)) {
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
        if (face.type != 1) continue;

        Texture* t = txloader.Load((char*)textures[face.textureID].name);
        if (t == NULL) continue;

        if (t->has_alpha())
        {
            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        }

        pr->set_uniform(base::opengl::UniformVars::Diffuse, t);
       
        u32 bindPos = binding[VertexAttrs::tagPosition];
        glVertexAttribPointer(
            bindPos,
            3,
            GL_FLOAT,
            GL_FALSE,
            sizeof(q3vertex),
            (u8*)&(vertexes[face.vertexIndex].pos));
        
        u32 bindTex = binding[VertexAttrs::tagTexture];
        glVertexAttribPointer(
            bindTex,
            2,
            GL_FLOAT,
            GL_FALSE,
            sizeof(q3vertex),
            (u8*)&(vertexes[face.vertexIndex].surfaceUV));

        u32 bindNorm = binding[VertexAttrs::tagNormal];
        glVertexAttribPointer(
            bindNorm,
            3,
            GL_FLOAT,
            GL_FALSE,
            sizeof(q3vertex),
            (u8*)&(vertexes[face.vertexIndex].normal));

        u32 bindLM = binding[VertexAttrs::tagTangent];
        glVertexAttribPointer(
            bindLM,
            2,
            GL_FLOAT,
            GL_FALSE,
            sizeof(q3vertex),
            (u8*)&(vertexes[face.vertexIndex].lightmapUV));
        glDrawElements(GL_TRIANGLES, face.numOfFaceIndices, GL_UNSIGNED_INT, &faceIndexes[face.faceVertexIndex]); 
        Stats::add_polygons(face.numOfFaceIndices / 3);

        if (t->has_alpha())
            glDisable(GL_BLEND);
    }
}

} // namespace opengl
} // namespace base
