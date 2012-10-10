/**
 * \file
 * \brief       Q3 map (BSP) loader
 * \author      Alexey Vasilyev <alexa.infra@gmail.com>
 * \copyright   MIT License
 **/
#pragma once

#include "base/types.h"
#include "base/math/vector.h"
#include "base/math/plane.h"
#include "base/stream.h"
#include "renderer/wirebox.h"
#include "renderer/gltexture.h"

namespace base
{
namespace opengl
{

class Camera;
class Program;
class TextureLoader;

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
    math::Vector3 normal;
    f32 dist;

    f32 side( const math::Vector3& v ) const {
        return math::Dot( normal, v ) - dist;
    }
};

struct q3node {
    i32 planeIndex;
    union {
        struct {
            i32 frontPlaneIndex;
            i32 backPlaneIndex;
        };
        i32 children[2];
    };
    i32 mmin[3];
    i32 mmax[3];

    math::Vector3 mins() const {
        return math::Vector3( static_cast<f32>(mmin[0]), static_cast<f32>(mmin[1]), static_cast<f32>(mmin[2]) );
    }
    math::Vector3 maxs() const {
        return math::Vector3( static_cast<f32>(mmax[0]), static_cast<f32>(mmax[1]), static_cast<f32>(mmax[2]) );
    }
};

namespace FaceTypes
{
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
    math::Vector3 lMapPos;
    math::Vector3 lMapVecs0;
    math::Vector3 lMapVecs1;
    math::Vector3 normal;
    i32 patchWidth;
    i32 patchHeight;
};

struct q3vertex {
    math::Vector3 pos;
    math::Vector2 surfaceUV;
    math::Vector2 lightmapUV;
    math::Vector3 normal;
    u8 color[4];

    q3vertex operator*( f32 scalar ) const {
        q3vertex ret;
        ret.pos = pos * scalar;
        ret.surfaceUV = surfaceUV * scalar;
        ret.lightmapUV = lightmapUV * scalar;
        ret.normal = normal * scalar;
        //ret.color = color * scalar;
        return ret;
    }
    q3vertex operator+( const q3vertex& v ) const {
        q3vertex ret;
        ret.pos = pos + v.pos;
        ret.surfaceUV = surfaceUV + v.surfaceUV;;
        ret.lightmapUV = lightmapUV + v.lightmapUV;
        ret.normal = normal + v.normal;
        //ret.color = color + v.color;
        return ret;
    }
    q3vertex& operator=( const q3vertex& v ) {
        pos = v.pos;
        surfaceUV = v.surfaceUV;
        lightmapUV = v.lightmapUV;
        normal = v.normal;
        //color = v.color;
        return *this;
    }
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
    math::Vector3 minv() const {
        return math::Vector3( static_cast<f32>(mins[0]), static_cast<f32>(mins[1]), static_cast<f32>(mins[2]) );
    }
    math::Vector3 maxv() const {
        return math::Vector3( static_cast<f32>(maxs[0]), static_cast<f32>(maxs[1]), static_cast<f32>(maxs[2]) );
    }
};

struct q3visibility {
    i32 nVecs;
    i32 szVecs;
    u8* vecs;
    bool isClusterVisible( i32 visCluster, i32 testCluster ) const;
};

struct q3lightmap {
    u8 data[128][128][3];
};

struct Node {
    math::Plane     plane;
    i32             cluster;
    i32             area;
    math::Vector3   maxs;
    math::Vector3   mins;
    Node*           children[2];
    Node*           parent;
    i32             firstFace;
    i32             numFaces;
    i32             frame;
    Node() {
        frame = -1;
        parent = NULL;
        children[0] = NULL;
        children[1] = NULL;
        cluster = -1;
    }
};

class q3maploader
{
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
    u8*                     visFaces;

    i32                     _camera_cluster;
    i32                     _frame_index;
    std::vector<int>        _visible_faces;

    std::vector<Node>       tree;
    std::vector<math::Plane> planes_;
    std::vector<Texture*>    lm_textures;
    WireBox _wb;
    Program* _program;

    q3maploader( FileBinary& file );
    ~q3maploader();

    void load();
    void PreloadTextures( TextureLoader& textureLoader );

    void render( const Camera& camera, Program* pr, TextureLoader& txloader );
private:
    void check_header();

    q3lump read_lump( i32 index );

    template<typename T>
    std::vector<T> read( i32 lumpIndex ) {
        q3lump lump = read_lump( lumpIndex );
        const u32 count = lump.length / sizeof( T );
        std::vector<T> items( count );
        f.set_position( lump.offset );
        f.read( reinterpret_cast<u8*>( &items.front() ), sizeof( T )*count );
        return items;
    }
    i32 findLeaf( const math::Vector3& camPos ) const;

    void render_polygons( const q3face& face, Program* pr ) const;
    void render_patch( const q3face& face, Program* pr ) const;
    void findDrawLeafs( const Camera& camera, const q3leaf& cameraLeaf, std::vector<int>& visFaces, int index ) const;
    void ComputePossibleVisible( const math::Vector3& cameraPos );
    void AddVisibleNode( Node* node );
    void ComputeVisible_R( const Camera& camera, Node* node, u32 planeMask );
};

} // opengl
} // base
