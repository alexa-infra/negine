/**
 * \file
 * \author      Alexey Vasilyev <alexa.infra@gmail.com>
 * \copyright   MIT License
 **/
#include "md3mesh.h"
#include <assert.h>

namespace base {
namespace resource {

Md3MeshBase::Md3MeshBase()
    : triangles(NULL)
    , shaders(NULL)
{
}

Md3MeshBase::~Md3MeshBase()
{
    delete[] triangles;
    delete[] shaders;
}

Md3Mesh::Md3Mesh()
    : vertexes(NULL)
    , uv(NULL)
    , mesh_base(NULL)
{
}

Md3Mesh::~Md3Mesh()
{
    delete[] vertexes;
    delete[] uv;
}

Md3FrameData::Md3FrameData()
    : meshes(NULL)
    , tags(NULL)
{
}

Md3FrameData::~Md3FrameData()
{
    delete[] tags;
    delete[] meshes;
}

Md3Model::Md3Model()
    : frame_data(NULL)
    , meshes(NULL)
{
}

Md3Model::~Md3Model()
{
    delete[] frame_data;
    delete[] meshes;
}

void Md3Model::load_md3(FileBinary& file) {
    /*u32 hdr_start = */file.position();

    hdr = file.read_type<Md3Header>();
    assert(hdr.ident == 0x33504449);
    assert(hdr.version == MD3_VERSION);

    frame_data = new Md3FrameData[hdr.num_frames];

//    file.set_position(hdr_start + hdr.ofs_frames);
    for (i32 i = 0; i<hdr.num_frames; i++)
        frame_data[i].frame = file.read_type<Md3Frame>();

    //    file.set_position(hdr_start + hdr.ofs_tags);
    for (i32 i = 0; i<hdr.num_frames; i++)
    {
        frame_data[i].tags = new Md3Tag[hdr.num_tags];
        for (i32 j=0; j<hdr.num_tags; j++)
            frame_data[i].tags[j] = file.read_type<Md3Tag>();
    }

    meshes = new Md3MeshBase[hdr.num_surfaces];
    for (i32 i = 0; i<hdr.num_frames; i++)
    {
        frame_data[i].meshes = new Md3Mesh[hdr.num_surfaces];
        for (i32 j=0; j<hdr.num_surfaces; j++)
            frame_data[i].meshes[j].mesh_base = &meshes[j];
    }
    
//    file.set_position(hdr_start + hdr.ofs_surfaces);
    for (i32 i = 0; i<hdr.num_surfaces; i++)
    {
        /*u32 surface_start = */file.position();
        
        Md3MeshBase& mesh = meshes[i];
        Md3Surface& surface = mesh.surface;
        surface = file.read_type<Md3Surface>();

        assert(surface.ident == 0x33504449);

        mesh.shaders = new Md3Shader[surface.num_shaders];
//        file.set_position(surface_start + surface.ofs_shaders);
        for (i32 j=0; j<surface.num_shaders; j++)
            mesh.shaders[j] = file.read_type<Md3Shader>();

        mesh.triangles = new Md3Triangle[surface.num_triangles];
//        file.set_position(surface_start + surface.ofs_triangles);
        for (i32 j=0; j<surface.num_triangles; j++) {
            mesh.triangles[j] = file.read_type<Md3Triangle>();
        }

//        file.set_position(surface_start + surface.ofs_st);
        for (i32 k=0; k<surface.num_frames; k++) {
            Md3Mesh& mesh_data = frame_data[k].meshes[i];
            mesh_data.uv = new Md3TexCoord[surface.num_verts];
            for (i32 j=0; j< surface.num_verts; j++) {
                mesh_data.uv[j] = file.read_type<Md3TexCoord>();
            }
        }
//        file.set_position(surface_start + surface.ofs_xyzn);
        for (i32 k=0; k<surface.num_frames; k++) {
            Md3Mesh& mesh_data = frame_data[k].meshes[i];
            mesh_data.vertexes = new Md3Vertex[surface.num_verts];
            for (i32 j=0; j<surface.num_verts; j++) {
                mesh_data.vertexes[j] = file.read_type<Md3Vertex>();
            }
        }
    }
}

//void Md3Model::load_md3(FileBinary& file) {
//    Md3Header* hdr = (Md3Header*)data;
//    assert(hdr->ident == 0x33504449);
//    assert(hdr->version == MD3_VERSION);
//    
//    std::vector<Mesh*> mesh_list;
//
//    Md3Frame* frames = (Md3Frame*)((u8*)hdr + sizeof(Md3Header));
////    for (i32 i = 0; i<hdr->num_frames; i++) {
////        frames[i];   
////    }
//    Md3Tag* tags = (Md3Tag*)((u8*)frames + hdr->num_frames *sizeof(Md3Frame));
////    for (i32 i = 0; i<hdr->num_frames * hdr->num_tags; i++) {
////        tags[i];
////    }
//
//    Md3Surface* surface = (Md3Surface*)((u8*)tags + hdr->num_frames * hdr->num_tags * sizeof(Md3Tag));
//    for (i32 i = 0; i<hdr->num_surfaces; i++)
//    {
//        Mesh* mesh = new Mesh;
//
//        assert(surface->ident == 0x33504449);
//
//        mesh->name = std::string((const char *)surface->name);
//        mesh->num_vertexes = surface->num_verts;
//        mesh->num_faces = surface->num_triangles;
//
//        mesh->vertexes = new Vertex[surface->num_frames * surface->num_verts];
//        mesh->faces = new Face[surface->num_frames * surface->num_triangles];
//
//        Md3Shader* shaders = (Md3Shader*)((u8*)surface + sizeof(Md3Surface));
//        for (i32 j=0; j<surface->num_shaders; j++) {
//            shaders[j];
//        }
//        Md3Triangle* triangles = (Md3Triangle*)((u8*)shaders + surface->num_shaders * sizeof(Md3Shader));
//        for (i32 j=0; j<surface->num_triangles; j++) {
//            for (u8 k=0; k<3; k++)
//                mesh->faces[j].index[k] = triangles[j].indexes[k];
//        }
//        Md3TexCoord* texcoord = (Md3TexCoord*)((u8*)triangles + surface->num_triangles * sizeof(Md3Triangle));
//        for (i32 j=0; j<surface->num_frames * surface->num_verts; j++) {
//            mesh->vertexes[j].tex.x = texcoord[j].st[0];
//            mesh->vertexes[j].tex.y = texcoord[j].st[1];
//        }
//        Md3Vertex* vert = (Md3Vertex*)((u8*)texcoord + surface->num_frames * surface->num_verts * sizeof(Md3TexCoord));
//        for (i32 j=0; j<surface->num_frames * surface->num_verts; j++) {
//            mesh->vertexes[j].n = DecodeNormal(vert[j].normal);
//            mesh->vertexes[j].pos.x = vert[j].coord[0] / 64.0f;
//            mesh->vertexes[j].pos.y = vert[j].coord[2] / 64.0f;
//            mesh->vertexes[j].pos.z = vert[j].coord[1] / 64.0f;
//            mesh->vertexes[j].color = Vector4(1, 1, 1, 1);
//        }
//        
//        mesh_list.push_back(mesh);
//
//        if (i + 1 != hdr->num_surfaces)
//            surface = (Md3Surface*)((u8*)vert + surface->num_frames * surface->num_verts * sizeof(Md3Vertex));
//    }
//    return mesh_list;
//}


void Md3Model::Load(const std::string& name) {
    FileBinary file(name);
    
    load_md3(file);
}

//void Md3Model::Load2(const std::string& name) {
//    FileBinary file(name);
//    u32 filesize = file.size();
//    u8* data = new u8[filesize];
//    file.read(data, filesize);
//    
//    load_md3(data);
//    
//    delete[] data;
//}

}
}
