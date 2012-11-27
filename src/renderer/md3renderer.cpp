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
using base::math::Vector2;
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

    MeshBuilder builder;
    builder
        .addAttribute(VertexAttrs::tagPosition)
        .addAttribute(VertexAttrs::tagNormal)
        .addAttribute(VertexAttrs::tagColor)
        .addAttribute(VertexAttrs::tagTexture);
    for ( i32 i = 0; i < model->hdr.num_surfaces; i++ ) {
         MeshExt* mesh = new MeshExt(builder, model->meshes[i].surface.num_verts, model->meshes[i].surface.num_triangles * 3);
         meshes[i] = mesh;
         vbs[i] = new VertexBuffer;
         vbs[i]->EnableAttributeMesh(mesh);
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
        MeshExt* mesh = meshes[i];
        Md3Surface& info = md3.mesh_base->surface;

        u16* indices = mesh->indices();
        for ( i32 j = 0; j < info.num_triangles; j++ ) {
            for ( u8 k = 0; k < 3; k++ ) {
                indices[j + k] = md3.mesh_base->triangles[j].indexes[k];
            }
        }

        math::Vector2* tex = mesh->findAttributeTyped<math::Vector2>(VertexAttrs::tagTexture);
        math::Vector3* pos = mesh->findAttributeTyped<math::Vector3>(VertexAttrs::tagPosition);
        math::Vector4* color = mesh->findAttributeTyped<math::Vector4>(VertexAttrs::tagColor);

        for ( i32 j = 0; j < info.num_verts; j++ ) {
            Md3TexCoord& uv = md3.uv[j];
            tex[j].x = uv.st[0];
            tex[j].y = uv.st[1];
            Md3Vertex& vert = md3.vertexes[j];
            pos[j] = DecodeNormal( vert.normal );
            pos[j].x = vert.coord[0] / 64.0f;
            pos[j].y = vert.coord[2] / 64.0f;
            pos[j].z = vert.coord[1] / 64.0f;
            color[j] = Vector4( 1, 1, 1, 1 );
        }

        vbs[i]->SetVertexData( mesh->data(), mesh->rawSize());
        vbs[i]->SetIndexData( mesh->indices(), mesh->numIndexes() * sizeof(u32));
        vbs[i]->Load();
    }
}

void Md3Renderer::Draw( )
{
    for ( u32 i = 0; i < vbs.size(); i++ ) {
        vbs[i]->BindAttributes();
        glDrawElements(
            GL_TRIANGLES, meshes[i]->numIndexes() * 3, 
            GL_UNSIGNED_INT, (void*)0);
        vbs[i]->UnbindAttributes();
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
