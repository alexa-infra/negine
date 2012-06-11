/**
 * \file
 * \brief       Q3 map (BSP) loader
 * \author      Alexey Vasilyev <alexa.infra@gmail.com>
 * \copyright   MIT License
 **/
#pragma once

#include "base/types.h"
#include "renderer/mesh.h"
#include "base/math/vector.h"
#include "base/stream.h"
#include <string.h>
#include <assert.h>
#include "renderer/camera.h"
#include "renderer/glcontext.h"
#include "renderer/glprogram.h"
#include "renderer/gltexture.h"
#include "renderer/statistics.h"

#include <deque>
#include <set>

namespace base {
namespace opengl {

struct q3header {
    u8 magic[4];
    u32 version;
};

struct q3lump {
    u32 offset;
    u32 length;
};

struct q3texture {
    char name[64];
    u32 flags;
    u32 contents;
};

struct q3plane {
    Vector3 normal;
    f32 dist;
};

struct q3node {
    i32 planeIndex;
    i32 frontPlaneIndex;
    i32 backPlaneIndex;
    i32 mins[3];
    i32 maxs[3];
};

namespace FaceTypes {
    enum FaceType {
        Polygon,
        Patch,
        Mesh,
        Billboard
    };
}
typedef FaceTypes::FaceType FaceType;

struct q3face {
    i32 textureID;
    i32 effect;
    i32 type; // FaceType
    i32 vertexIndex;
    i32 numOfVerts;
    i32 faceVertexIndex;
    i32 numOfFaceIndices;
    i32 lightmapID;
    i32 lMapCornerX;
    i32 lMapCornerY;
    i32 lMapSizeX;
    i32 lMapSizeY;
    Vector3 lMapPos;
    Vector3 lMapVecs0;
    Vector3 lMapVecs1;
    Vector3 normal;
    i32 patchWidth;
    i32 patchHeight;
};

struct q3vertex {
    Vector3 pos;
    Vector2 surfaceUV;
    Vector2 lightmapUV;
    Vector3 normal;
    u8 color[4];
};

struct q3brushside {
    i32 planeIndex;
    i32 textureIndex;
};

struct q3brush {
    i32 brushSide;
    i32 numberOfBrushSides;
    i32 textureIndex;
};

struct q3leaf {
    i32 cluster;
    i32 area;
    i32 mins[3];
    i32 maxs[3];
    i32 leafFace;
    i32 numberOfLeafFaces;
    i32 leafBrush;
    i32 numberOfLeafBrushes;
};

struct q3visibility {
    i32 nVecs;
    i32 szVecs;
    u8* vecs;
    bool isClusterVisible(i32 visCluster, i32 testCluster) const {
        if (visCluster < 0) {
            return true;
        }
        return (vecs[((visCluster * szVecs) + (testCluster >> 3))] & (1 << (testCluster & 7))) != 0;
    }
};

struct q3lightmap {
    u8 data[128][128][3];
};

class q3map {
};

class q3maploader {
public:
    FileBinary& f;
    std::vector<q3texture>  textures;
    std::vector<q3plane>    planes;
    std::vector<q3node>     nodes;
    std::vector<q3leaf>     leafs;
    std::vector<u32>        leafFaces;
    std::vector<q3vertex>   vertexes;
    std::vector<q3face>     faces;
    std::vector<u32>        faceIndexes;
    q3visibility            visibility;

    q3maploader(FileBinary& file) : f(file) {
    }
    void load() {
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
    }

    void check_header() {
        f.set_position(0);
        q3header hdr = f.read_type<q3header>();
        assert(strncmp(reinterpret_cast<char*>(hdr.magic), "IBSP", 4) == 0);
        assert(hdr.version == 0x2e);
    }

    q3lump read_lump(i32 index) {
        f.set_position(index * sizeof(q3lump) + sizeof(q3header));
        return f.read_type<q3lump>();
    }

    template<typename T>
    std::vector<T> read(i32 lumpIndex) {
        q3lump lump = read_lump(lumpIndex);

        const u32 count = lump.length / sizeof(T);
        std::vector<T> items(count);
        for(u32 i=0; i<count; i++) {
            f.set_position(lump.offset + i * sizeof(T));
            items[i] = f.read_type<T>();
        }
        return items;
    }
    i32 findLeaf(const Vector3& camPos) const {
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
    void render(Camera& camera, AttributeBinding& binding, Program* pr, TextureLoader& txloader) const {
        i32 cameraLeafIndex = findLeaf(camera.position());
        const q3leaf& cameraLeaf = leafs[cameraLeafIndex];
        std::deque<int> visibleLeafs;
        for (u32 i=0; i<leafs.size(); i++) {
            const q3leaf& leaf = leafs[i];
            if (visibility.isClusterVisible(cameraLeaf.cluster, leaf.cluster)) {
                if (camera.IsInFrustum(Vector3(leaf.mins[0], leaf.mins[1], leaf.mins[2]), Vector3(leaf.maxs[0], leaf.maxs[1], leaf.maxs[2]))) {
                    visibleLeafs.push_back(i);
                }
            }
        }
        std::set<int> visibleFaces;
        for (u32 i=0; i<visibleLeafs.size(); i++) {
            const q3leaf& leaf = leafs[visibleLeafs[i]];
            for (i32 j=0; j<leaf.numberOfLeafFaces; j++) {
                i32 faceIndex = leaf.leafFace + j;
                visibleFaces.insert(faceIndex);
            }
        }
        std::vector<int> visibleFacesArr(visibleFaces.begin(), visibleFaces.end());

        for (u32 i=0; i<visibleFacesArr.size(); i++) {
            int iface = leafFaces[visibleFacesArr[i]];
            const q3face& face = faces[iface];
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
};

} // opengl
} // base
