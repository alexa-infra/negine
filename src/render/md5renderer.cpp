/**
 * \file
 * \brief       Md5 mesh renderer
 * \author      Alexey Vasilyev <alexa.infra@gmail.com>
 * \copyright   MIT License
 **/
#include "render/md5renderer.h"
#include "render/glcontext.h"

using base::resource::Md5Mesh;
using base::resource::Md5Model;
using base::resource::Md5Model;
using base::resource::Md5Joint;
using base::resource::Md5Weight;
using base::math::Vector2;
using base::math::Vector3;

namespace base
{
namespace opengl
{

Md5Renderer::Md5Renderer( Md5Model* model )
    : mesh_( NULL )
    , vb( NULL )
    , md5( model )
{
//    vb = new VertexBuffer;
    Md5Mesh& mesh = md5->meshes[0];

    MeshBuilder builder;
    builder
        .addAttribute(VertexAttrs::tagPosition)
        .addAttribute(VertexAttrs::tagNormal)
        .addAttribute(VertexAttrs::tagTexture)
        .addAttribute(VertexAttrs::tagTangent)
        .addAttribute(VertexAttrs::tagBinormal);
    mesh_ = new MeshExt(builder, mesh.num_verts, mesh.num_tris * 3);
    vb->EnableAttributeMesh(mesh_);
}

Md5Renderer::~Md5Renderer()
{
    delete vb;
    delete mesh_;
}

void Md5Renderer::Draw( )
{
    vb->BindAttributes();
    //glDrawElements(
    //    GL_TRIANGLES, mesh_->numIndexes(), 
    //    GL_UNSIGNED_SHORT, (void*)0);
    vb->UnbindAttributes();
}

void Md5Renderer::Commit()
{
    Md5Mesh& mesh = md5->meshes[0];
    GenerateVertexes( mesh );
    GenerateIndexes( mesh );
    GenerateLightningInfo( mesh );
    vb->SetVertexData( mesh_->data(), mesh_->rawSize() );
    vb->SetIndexData( mesh_->indices(), mesh_->numIndexes() * sizeof(u16) );
    vb->Load();
}

void Md5Renderer::GenerateVertexes( Md5Mesh& mesh )
{
    const Md5Joint* skeleton = md5->baseSkel;
    math::Vector2* tex = mesh_->findAttributeTyped<math::Vector2>(VertexAttrs::tagTexture);
    math::Vector3* tangent = mesh_->findAttributeTyped<math::Vector3>(VertexAttrs::tagTangent);
    math::Vector3* pos = mesh_->findAttributeTyped<math::Vector3>(VertexAttrs::tagPosition);
    math::Vector3* n = mesh_->findAttributeTyped<math::Vector3>(VertexAttrs::tagNormal);

    for ( i32 i = 0; i < mesh.num_verts; ++i ) {
        tex[i] = mesh.vertices[i].st;
        pos[i].set( 0.0f );
        n[i].set( 0.0f );
        tangent[i].set( 0.0f );

        for ( u32 j = 0; j < mesh.vertices[i].count; j++ ) {
            const Md5Weight& weight = mesh.weights[mesh.vertices[i].start + j];
            const Md5Joint& joint = skeleton[weight.joint];
            pos[i] += joint.translate( weight.pos ) * weight.bias;
            n[i] += joint.orient.RotatePoint( weight.normal );
            tangent[i] += joint.orient.RotatePoint( weight.tangent );
        }

        n[i].Normalize();
        tangent[i].Normalize();
        UpdateBoundingBox( pos[i] );
    }
}

void Md5Renderer::GenerateIndexes( Md5Mesh& mesh )
{
    u16* indicies = mesh_->indices();
    for ( int i = 0; i < mesh.num_tris; i++ ) {
        for ( int j = 0; j < 3; j++ ) {
            indicies[3*i + j] = (u16)mesh.triangles[i].index[j];
        }
    }
}

void Md5Renderer::GenerateLightningInfo( Md5Mesh& mesh )
{
    math::Vector2* tex = mesh_->findAttributeTyped<math::Vector2>(VertexAttrs::tagTexture);
    math::Vector3* tangent = mesh_->findAttributeTyped<math::Vector3>(VertexAttrs::tagTangent);
    math::Vector3* binormal = mesh_->findAttributeTyped<math::Vector3>(VertexAttrs::tagBinormal);
    math::Vector3* pos = mesh_->findAttributeTyped<math::Vector3>(VertexAttrs::tagPosition);
    math::Vector3* n = mesh_->findAttributeTyped<math::Vector3>(VertexAttrs::tagNormal);

    for ( i32 i = 0; i < mesh.num_tris; i++ ) {
        f32 d0[5], d1[5];
        math::Vector3 normal, tangents[2];
        const Vector3* a = pos + mesh.triangles[i].index[0];
        const Vector3* b = pos + mesh.triangles[i].index[1];
        const Vector3* c = pos + mesh.triangles[i].index[2];
        const Vector2* ta = tex + mesh.triangles[i].index[0];
        const Vector2* tb = tex + mesh.triangles[i].index[1];
        const Vector2* tc = tex + mesh.triangles[i].index[2];
        d0[0] = b->x - a->x;
        d0[1] = b->y - a->y;
        d0[2] = b->z - a->z;
        d0[3] = tb->x - ta->x;
        d0[4] = tb->y - ta->y;
        d1[0] = c->x - a->x;
        d1[1] = c->y - a->y;
        d1[2] = c->z - a->z;
        d1[3] = tc->x - ta->x;
        d1[4] = tc->y - ta->y;
        normal = Vector3(
                     d1[1] * d0[2] - d1[2] * d0[1],
                     d1[2] * d0[0] - d1[0] * d0[2],
                     d1[0] * d0[1] - d1[1] * d0[0]
                 );
        normal.Normalize();
        tangents[0] = Vector3(
                          d0[0] * d1[4] - d0[4] * d1[0],
                          d0[1] * d1[4] - d0[4] * d1[1],
                          d0[2] * d1[4] - d0[4] * d1[2]
                      );
        tangents[0].Normalize();
        tangents[1] = Vector3(
                          d0[3] * d1[0] - d0[0] * d1[3],
                          d0[3] * d1[1] - d0[1] * d1[3],
                          d0[3] * d1[2] - d0[2] * d1[3]
                      );
        tangents[1].Normalize();

        for ( int j = 0; j < 3; j++ ) {
            u32 idx = mesh.triangles[i].index[j];
            n[idx] += normal;
            tangent[idx] += tangents[0];
            binormal[idx] += tangents[1];
        }
    }

    for ( i32 i = 0; i < mesh.num_verts; i++ ) {
        n[i].Normalize();
        f32 d = Dot( tangent[i], n[i] );
        tangent[i] = tangent[i] - n[i] * d;
        tangent[i].Normalize();
        d = Dot( binormal[i], n[i] );
        binormal[i] = binormal[i] - n[i] * d;
        binormal[i].Normalize();
    }
}

void Md5Renderer::UpdateBoundingBox( math::Vector3& pos )
{
    if ( boundingBox.min.x > pos.x ) {
        boundingBox.min.x = pos.x;
    }

    if ( boundingBox.min.y > pos.y ) {
        boundingBox.min.y = pos.y;
    }

    if ( boundingBox.min.z > pos.z ) {
        boundingBox.min.z = pos.z;
    }

    if ( boundingBox.max.x < pos.x ) {
        boundingBox.max.x = pos.x;
    }

    if ( boundingBox.max.y < pos.y ) {
        boundingBox.max.y = pos.y;
    }

    if ( boundingBox.max.z < pos.z ) {
        boundingBox.max.z  = pos.z;
    }
}

} // namespace base
} // namespace opengl
