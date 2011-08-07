/**
 * @author  Alexey Vasilyev <alexa.infra@gmail.com>
 * @section LICENSE
 * 
 * This file is part of Negine (https://alexadotlife.com/hg/negine)
 * Licensed under LGPLv3 (or GPLv3) - see LICENSE file for details
 **/
#include "md3mesh.h"
#include "base/stream.h"
#include "base/math/vector.h"
#include <iostream>
#include <assert.h>

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

struct  Md3Header
{
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
struct  Md3Frame
{
    f32 min_bounds[3];  // first corner of bounding box
    f32 max_bounds[3];  // second corner of bounding box
    f32 local_origin[3];// local origin, position
    f32 radius;         // radius of bounding sphere
    u8  name[16];       // null-terminated name of frame
};

// attachment point for another md3 model
struct  Md3Tag
{
    u8  name[64];       // null-terminated name of tag
    f32 origin[3];      // position of tag
    f32 axis[3][3];     // rotation matrix
};

struct  Md3Surface
{
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

struct  Md3Shader
{
    u8  name[64];       // name of shader object
    i32 shader_index;   // ???
};

struct  Md3Triangle
{
    i32 indexes[3];     // indexes on triangles array, with clockwise winding
};

struct  Md3TexCoord
{
    f32 st[2];          // st-coordinates (left-handed space)
};

struct  Md3Vertex
{
    i16 coord[3];       // x, y, and z coordinates in left-handed 3-space, scaled down by factor 1.0/64
    u8  normal[2];      // Zenith and azimuth angles of normal vector
};

#pragma pack(pop)

using namespace base::math;

Vector3 DecodeNormal(const u8* normal) {
    f32 const& zenith = normal[0];
    f32 const& azimuth = normal[1];
    f32 lat = zenith * (2 * pi ) / 255.f;
    f32 lng = azimuth * (2 * pi) / 255.f;
    
    Vector3 ret;
    ret.x = cosf(lat) * sinf(lng);
    ret.y = sinf(lat) * sinf(lng);
    ret.z = cosf(lng);
    return ret;
}

namespace base {
namespace opengl {

std::vector<Mesh*> load_md3(FileBinary& file) {
    u32 hdr_start = file.position();

    Md3Header hdr = file.read_type<Md3Header>();
    assert(hdr.ident == 0x33504449);
    assert(hdr.version == MD3_VERSION);

    std::vector<Mesh*> mesh_list;

    file.set_position(hdr_start + hdr.ofs_frames);
    for (i32 i = 0; i<hdr.num_frames; i++)
        file.read_type<Md3Frame>();
    file.set_position(hdr_start + hdr.ofs_tags);
    for (i32 i = 0; i<hdr.num_frames * hdr.num_tags; i++)
        file.read_type<Md3Tag>();
    file.set_position(hdr_start + hdr.ofs_surfaces);
    for (i32 i = 0; i<hdr.num_surfaces; i++)
    {
        Mesh* mesh = new Mesh;

        u32 surface_start = file.position();
        Md3Surface surface = file.read_type<Md3Surface>();
        assert(surface.ident == 0x33504449);

        mesh->num_vertexes = surface.num_verts;
        mesh->num_faces = surface.num_triangles;

        mesh->vertexes = new Vertex[surface.num_frames * surface.num_verts];
        mesh->faces = new Face[surface.num_frames * surface.num_triangles];

        file.set_position(surface_start + surface.ofs_shaders);
        for (i32 j=0; j<surface.num_shaders; j++)
            file.read_type<Md3Shader>();
        file.set_position(surface_start + surface.ofs_triangles);
        for (i32 j=0; j<surface.num_triangles; j++) {
            Md3Triangle tri = file.read_type<Md3Triangle>();
            for (u8 k=0; k<3; k++)
                mesh->faces[j].index[k] = tri.indexes[k];
        }
        file.set_position(surface_start + surface.ofs_st);
        for (i32 j=0; j<surface.num_frames * surface.num_verts; j++) {
            Md3TexCoord tex = file.read_type<Md3TexCoord>();
            mesh->vertexes[j].tex.x = tex.st[0];
            mesh->vertexes[j].tex.y = tex.st[1];
        }
        file.set_position(surface_start + surface.ofs_xyzn);
        for (i32 j=0; j<surface.num_frames * surface.num_verts; j++) {
            Md3Vertex v = file.read_type<Md3Vertex>();
            mesh->vertexes[j].n = DecodeNormal(v.normal);
            mesh->vertexes[j].pos.x = v.coord[0] / 64.0f;
            mesh->vertexes[j].pos.y = v.coord[2] / 64.0f;
            mesh->vertexes[j].pos.z = v.coord[1] / 64.0f;
            mesh->vertexes[j].color = Vector4(1, 1, 1, 1);
        }
        
        mesh_list.push_back(mesh);
    }
    return mesh_list;
}

std::vector<Mesh*> load_md3(u8* data) {
    Md3Header* hdr = (Md3Header*)data;
    assert(hdr->ident == 0x33504449);
    assert(hdr->version == MD3_VERSION);

    std::vector<Mesh*> mesh_list;

    Md3Frame* frames = (Md3Frame*)(data + hdr->ofs_frames);
    for (i32 i = 0; i<hdr->num_frames; i++) {
        frames[i];   
    }
    Md3Tag* tags = (Md3Tag*)(data + hdr->ofs_tags);
    for (i32 i = 0; i<hdr->num_frames * hdr->num_tags; i++) {
        tags[i];
    }

    Md3Surface* surface = (Md3Surface*)(data + hdr->ofs_surfaces);
    for (i32 i = 0; i<hdr->num_surfaces; i++)
    {
        Mesh* mesh = new Mesh;

        assert(surface->ident == 0x33504449);

        mesh->num_vertexes = surface->num_verts;
        mesh->num_faces = surface->num_triangles;

        mesh->vertexes = new Vertex[surface->num_frames * surface->num_verts];
        mesh->faces = new Face[surface->num_frames * surface->num_triangles];

        Md3Shader* shaders = (Md3Shader*)((u8*)surface + surface->ofs_shaders);
        for (i32 j=0; j<surface->num_shaders; j++) {
            shaders[j];
        }
        Md3Triangle* triangles = (Md3Triangle*)((u8*)surface + surface->ofs_triangles);
        for (i32 j=0; j<surface->num_triangles; j++) {
            for (u8 k=0; k<3; k++)
                mesh->faces[j].index[k] = triangles[j].indexes[k];
        }
        Md3TexCoord* texcoord = (Md3TexCoord*)((u8*)surface + surface->ofs_st);
        for (i32 j=0; j<surface->num_frames * surface->num_verts; j++) {
            mesh->vertexes[j].tex.x = texcoord[j].st[0];
            mesh->vertexes[j].tex.y = texcoord[j].st[1];
        }
        Md3Vertex* vert = (Md3Vertex*)((u8*)surface + surface->ofs_xyzn);
        for (i32 j=0; j<surface->num_frames * surface->num_verts; j++) {
            mesh->vertexes[j].n = DecodeNormal(vert[j].normal);
            mesh->vertexes[j].pos.x = vert[j].coord[0] / 64.0f;
            mesh->vertexes[j].pos.y = vert[j].coord[2] / 64.0f;
            mesh->vertexes[j].pos.z = vert[j].coord[1] / 64.0f;
            mesh->vertexes[j].color = Vector4(1, 1, 1, 1);
        }
        
        mesh_list.push_back(mesh);

        if (i + 1 != hdr->num_surfaces)
            surface = (Md3Surface*)((u8*)surface + surface->ofs_end);
    }
    return mesh_list;
}


std::vector<Mesh*> load_md3(const std::string& name) {
    FileBinary file(name);
    return load_md3(file);
}

std::vector<Mesh*> load_md3_se(const std::string& name) {
    FileBinary file(name);
    u32 filesize = file.size();
    u8* data = new u8[filesize];
    file.read(data, filesize);
    std::vector<Mesh*> ret = load_md3(data);
    delete[] data;
    return ret;
}

}
}