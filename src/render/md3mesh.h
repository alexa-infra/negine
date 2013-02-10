/**
 * \file
 * \brief       MD3 model loader
 * \author      Alexey Vasilyev <alexa.infra@gmail.com>
 * \copyright   MIT License
 **/
#pragma once

#include "base/types.h"
#include "base/stream.h"
#include <vector>

namespace base
{
namespace resource
{

//
// http://en.wikipedia.org/wiki/MD3_(file_format)
// MD3 is a model format used by the Quake 3 engine
// as well as its many mods. The format is used mostly
// for player models and non-structural geometry
// (non-static models) although it may be used for structural uses.
// Unlike modern formats, animations are not bone based
// but instead vertex based and are directly contained
// within the file.
//

#define MD3_VERSION 15
#define MD3_MAX_FRAMES 1024
#define MD3_MAX_TAGS 16
#define MD3_MAX_SURFACES 32
#define MD3_MAX_SHADERS 256
#define MD3_MAX_VERTS 4096
#define MD3_MAX_TRIANGLES 8192

#pragma pack(push, 1)

struct  Md3Header {
    u32 ident;          // Magic number. As a string reads "IDP3", or 0x33504449
    u32 version;        // 15 latest
    u8  name[64];       // null-terminated md3 name
    i32 flags;          // ???
    i32 num_frames;     // number of frame objects, max is 1024
    i32 num_tags;       // there is one set of tags per frame
    i32 num_surfaces;   // number of surface objects
    i32 num_skins;      // number of skins, not used
    i32 ofs_frames;     // offset to begin of frame array
    i32 ofs_tags;       // offset to begin of tag array
    i32 ofs_surfaces;   // offset to begin of surface array
    i32 ofs_eof;        // end of md3 object
};

// signle animation frame
struct  Md3Frame {
    f32 min_bounds[3];  // first corner of bounding box
    f32 max_bounds[3];  // second corner of bounding box
    f32 local_origin[3];// local origin, position
    f32 radius;         // radius of bounding sphere
    u8  name[16];       // null-terminated name of frame
};

// attachment point for another md3 model
struct  Md3Tag {
    u8  name[64];       // null-terminated name of tag
    f32 origin[3];      // position of tag
    f32 axis[3][3];     // rotation matrix
};

struct  Md3Surface {
    u32 ident;          // Magic number. As a string reads "IDP3", or 0x33504449
    u8  name[64];       // null-terminated name of surface
    i32 flags;          // ???
    i32 num_frames;     // number of animation frame objects
    i32 num_shaders;    // number of shader objects
    i32 num_verts;      // number of vertex objects
    i32 num_triangles;  // number of triangle objects
    i32 ofs_triangles;  // offset to begin of triangles array
    i32 ofs_shaders;    // offset to begin of shaders array
    i32 ofs_st;         // offset to begin of st texture coords
    i32 ofs_xyzn;       // offset to begin of xyznormal array
    i32 ofs_end;        // offset to end of surface object
};

struct  Md3Shader {
    u8  name[64];       // name of shader object
    i32 shader_index;   // ???
};

struct  Md3Triangle {
    i32 indexes[3];     // indexes on triangles array, with clockwise winding
};

struct  Md3TexCoord {
    f32 st[2];          // st-coordinates (left-handed space)
};

struct  Md3Vertex {
    i16 coord[3];       // x, y, and z coordinates in left-handed 3-space, scaled down by factor 1.0/64
    u8  normal[2];      // Zenith and azimuth angles of normal vector
};

#pragma pack(pop)

class Md3MeshBase
{
public:
    Md3Triangle* triangles;
    Md3Shader* shaders;
    Md3Surface surface;

    Md3MeshBase();
    ~Md3MeshBase();
};

class Md3Mesh
{
public:
    Md3Vertex* vertexes;
    Md3TexCoord* uv;
    Md3MeshBase* mesh_base;

    Md3Mesh();
    ~Md3Mesh();
};

class Md3FrameData
{
public:
    Md3Mesh* meshes;
    Md3Tag* tags;
    Md3Frame frame;

    Md3FrameData();
    ~Md3FrameData();
};

class Md3Model
{
public:
    Md3Header hdr;
    Md3FrameData* frame_data;
    Md3MeshBase* meshes;

    Md3Model();
    ~Md3Model();

    void Load( const std::string& name );
//    void Load2(const std::string& name);
private:
//    void load_md3(u8* data);
    void load_md3( FileBinary& file );
};

} // namespace resource
} // namespace base