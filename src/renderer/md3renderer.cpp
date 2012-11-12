/**
 * \file
 * \brief       MD3 model renderer
 * \author      Alexey Vasilyev <alexa.infra@gmail.com>
 * \copyright   MIT License
 **/
#include "renderer/md3renderer.h"

using base::resource::Md3FrameData;
using base::resource::Md3Mesh;
using base::resource::Md3Vertex;
using base::resource::Md3TexCoord;
using base::resource::Md3Model;
using base::resource::Md3Surface;
using base::math::pi;
using base::math::Vector3;
using base::math::Vector4;

namespace base
{
namespace opengl
{

Md3Renderer::Md3Renderer( Md3Model* m )
    : model( m )
{
    meshes.resize( model->hdr.num_surfaces );
    vbs.resize( model->hdr.num_surfaces );

    for ( i32 i = 0; i < model->hdr.num_surfaces; i++ ) {
        meshes[i] = new Mesh;
        meshes[i]->vertexes = new Vertex[model->meshes[i].surface.num_verts];
        meshes[i]->faces = new Face[model->meshes[i].surface.num_triangles];
        vbs[i] = new VertexBufferMemory;
    }
}

Md3Renderer::~Md3Renderer()
{
    for ( u32 i = 0; i < meshes.size(); i++ ) {
        delete meshes[i];
    }

    for ( u32 i = 0; i < vbs.size(); i++ ) {
        delete vbs[i];
    }
}

void Md3Renderer::Commit()
{
    Md3FrameData& frame = model->frame_data[0];

    for ( i32 i = 0; i < model->hdr.num_surfaces; i++ ) {
        Md3Mesh& md3 = frame.meshes[i];
        Mesh* mesh = meshes[i];
        Md3Surface& info = md3.mesh_base->surface;
        mesh->num_vertexes = info.num_verts;
        mesh->num_faces = info.num_triangles;

        for ( i32 j = 0; j < info.num_triangles; j++ ) {
            for ( u8 k = 0; k < 3; k++ ) {
                mesh->faces[j].index[k] = md3.mesh_base->triangles[j].indexes[k];
            }
        }

        for ( i32 j = 0; j < info.num_verts; j++ ) {
            Md3TexCoord& uv = md3.uv[j];
            mesh->vertexes[j].tex.x = uv.st[0];
            mesh->vertexes[j].tex.y = uv.st[1];
            Md3Vertex& vert = md3.vertexes[j];
            mesh->vertexes[j].n = DecodeNormal( vert.normal );
            mesh->vertexes[j].pos.x = vert.coord[0] / 64.0f;
            mesh->vertexes[j].pos.y = vert.coord[2] / 64.0f;
            mesh->vertexes[j].pos.z = vert.coord[1] / 64.0f;
            mesh->vertexes[j].color = Vector4( 1, 1, 1, 1 );
        }

        vbs[i]->SetData( mesh->vertexes, mesh->num_vertexes,
                         mesh->faces, mesh->num_faces );
    }
}

void Md3Renderer::Draw( AttributeBinding& binding )
{
    for ( u32 i = 0; i < vbs.size(); i++ ) {
        vbs[i]->Draw( binding );
    }
}

Vector3 Md3Renderer::DecodeNormal( const u8* normal )
{
    f32 const& zenith = normal[0];
    f32 const& azimuth = normal[1];
    f32 lat = zenith * ( 2 * pi ) / 255.f;
    f32 lng = azimuth * ( 2 * pi ) / 255.f;
    Vector3 ret;
    ret.x = cosf( lat ) * sinf( lng );
    ret.y = sinf( lat ) * sinf( lng );
    ret.z = cosf( lng );
    return ret;
}

}
}
