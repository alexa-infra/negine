/**
 * \file
 * \brief       MD3 model renderer
 * \author      Alexey Vasilyev <alexa.infra@gmail.com>
 * \copyright   MIT License
 **/
#include "render/md3renderer.h"
#include "render/glcontext.h"
#include "math/vec4.h"

using base::resource::Md3FrameData;
using base::resource::Md3Mesh;
using base::resource::Md3Vertex;
using base::resource::Md3TexCoord;
using base::resource::Md3Model;
using base::resource::Md3Surface;
using base::math::pi;
using base::math::vec2f;
using base::math::vec3f;
using base::math::vec4f;

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
         Mesh* mesh = new Mesh();
         (*mesh)
             .addAttribute(VertexAttrs::tagPosition)
             .addAttribute(VertexAttrs::tagNormal)
             .addAttribute(VertexAttrs::tagColor)
             .addAttribute(VertexAttrs::tagTexture)
             .vertexCount(model->meshes[i].surface.num_verts)
             .indexCount(model->meshes[i].surface.num_triangles * 3, IndexTypes::UInt16)
             .complete();
         meshes[i] = mesh;
//         vbs[i] = new VertexBuffer;
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
        Mesh* mesh = meshes[i];
        Md3Surface& info = md3.mesh_base->surface;

        u16* indices = reinterpret_cast<u16*>(mesh->indices());
        for ( i32 j = 0; j < info.num_triangles; j++ ) {
            for ( u8 k = 0; k < 3; k++ ) {
                indices[j + k] = md3.mesh_base->triangles[j].indexes[k];
            }
        }

        math::vec2f* tex = mesh->findAttributeTyped<math::vec2f>(VertexAttrs::tagTexture);
        math::vec3f* pos = mesh->findAttributeTyped<math::vec3f>(VertexAttrs::tagPosition);
        math::vec3f* n = mesh->findAttributeTyped<math::vec3f>(VertexAttrs::tagNormal);
        math::vec4f* color = mesh->findAttributeTyped<math::vec4f>(VertexAttrs::tagColor);

        for ( i32 j = 0; j < info.num_verts; j++ ) {
            Md3TexCoord& uv = md3.uv[j];
            tex[j].x = uv.st[0];
            tex[j].y = uv.st[1];
            Md3Vertex& vert = md3.vertexes[j];
            DecodeNormal( vert.normal, n[j] );
            pos[j].x = vert.coord[0] / 64.0f;
            pos[j].y = vert.coord[2] / 64.0f;
            pos[j].z = vert.coord[1] / 64.0f;
            color[j] = vec4f( 1, 1, 1, 1 );
        }

        vbs[i]->SetVertexData( mesh->data(), mesh->rawSize());
        vbs[i]->SetIndexData( mesh->indices(), mesh->numIndexes() * sizeof(u16));
        vbs[i]->Load();
    }
}

void Md3Renderer::Draw( )
{
    for ( u32 i = 0; i < vbs.size(); i++ ) {
        vbs[i]->BindAttributes();
        //glDrawElements(
        //    GL_TRIANGLES, meshes[i]->numIndexes(), 
        //    GL_UNSIGNED_SHORT, (void*)0);
        vbs[i]->UnbindAttributes();
    }
}

void Md3Renderer::DecodeNormal( const u8* normal, vec3f& v )
{
    f32 const& zenith = normal[0];
    f32 const& azimuth = normal[1];
    f32 lat = zenith * ( 2 * pi ) / 255.f;
    f32 lng = azimuth * ( 2 * pi ) / 255.f;

    v.x = cosf( lat ) * sinf( lng );
    v.y = sinf( lat ) * sinf( lng );
    v.z = cosf( lng );
}

}
}
