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
using base::resource::Md5Vertex;
using base::math::vec2f;
using base::math::vec3f;

namespace base
{
namespace opengl
{

Md5Renderer::Md5Renderer( Md5Model* model, DeviceContext& gl )
    : mesh_( NULL )
    , vb( NULL )
    , md5( model )
    , GL(gl)
{
    vb = new VertexBuffer(GL);
    Md5Mesh& mesh = md5->meshes[0];

    mesh_ = new Mesh();
    (*mesh_)
        .addAttribute(VertexAttrs::tagPosition)
        .addAttribute(VertexAttrs::tagNormal)
        .addAttribute(VertexAttrs::tagTexture)
        .addAttribute(VertexAttrs::tagTangent)
        .vertexCount(mesh.num_verts)
        .indexCount(mesh.num_tris * 3, IndexTypes::UInt16)
        .complete();
    vb->enableAttributeMesh(mesh_);
}

Md5Renderer::~Md5Renderer()
{
    delete vb;
    delete mesh_;
}

void Md5Renderer::Draw( )
{
    vb->bind();
    GL.DrawElements(
        GL_TRIANGLES, mesh_->numIndexes(), 
        mesh_->indexType(), NULL);
    vb->unbind();
}

void Md5Renderer::Commit()
{
    Md5Mesh& mesh = md5->meshes[0];
    GenerateVertexes( mesh );
    GenerateIndexes( mesh );
    GenerateLightningInfo( mesh );
    vb->setVertexData( mesh_->data(), mesh_->rawSize() );

    // TODO: should it be only once?? and Uint32
    vb->setIndexData( mesh_->indices(), mesh_->numIndexes() * sizeof(u16) );
}

void Md5Renderer::GenerateVertexes( Md5Mesh& mesh )
{
    const Md5Joint* skeleton = md5->baseSkel;
    math::vec2f* tex = mesh_->findAttributeTyped<math::vec2f>(VertexAttrs::tagTexture);
    math::vec3f* pos = mesh_->findAttributeTyped<math::vec3f>(VertexAttrs::tagPosition);

    for ( i32 i = 0; i < mesh.num_verts; ++i ) {
        tex[i] = mesh.vertices[i].st;
        pos[i] = vec3f( 0.0f );

        for ( u32 j = 0; j < mesh.vertices[i].count; j++ ) {
            const Md5Weight& weight = mesh.weights[mesh.vertices[i].start + j];
            const Md5Joint& joint = skeleton[weight.joint];
            pos[i] += joint.translate( weight.pos ) * weight.bias;
        }

        UpdateBoundingBox( pos[i] );
    }
}

void Md5Renderer::GenerateIndexes( Md5Mesh& mesh )
{
    u16* indicies = reinterpret_cast<u16*>(mesh_->indices());
    for ( int i = 0; i < mesh.num_tris; i++ ) {
        for ( int j = 0; j < 3; j++ ) {
            indicies[3*i + j] = static_cast<u16>(mesh.triangles[i].index[j]);
        }
    }
}

void Md5Renderer::GenerateLightningInfo( Md5Mesh& mesh )
{
    math::vec2f* tex = mesh_->findAttributeTyped<math::vec2f>(VertexAttrs::tagTexture);
    math::vec3f* pos = mesh_->findAttributeTyped<math::vec3f>(VertexAttrs::tagPosition);

    for ( i32 i = 0; i < mesh.num_weights; i++ ) {
        Md5Weight& weight = mesh.weights[i];
        weight.normal = vec3f(0.0f);
        weight.tangent = vec3f(0.0f);
    }

    for ( i32 i = 0; i < mesh.num_tris; i++ ) {
        f32 d0[5], d1[5];
        vec3f normal, tangent;
        const vec3f* a = pos + mesh.triangles[i].index[0];
        const vec3f* b = pos + mesh.triangles[i].index[1];
        const vec3f* c = pos + mesh.triangles[i].index[2];
        const vec2f* ta = tex + mesh.triangles[i].index[0];
        const vec2f* tb = tex + mesh.triangles[i].index[1];
        const vec2f* tc = tex + mesh.triangles[i].index[2];
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
        normal = vec3f(
                     d1[1] * d0[2] - d1[2] * d0[1],
                     d1[2] * d0[0] - d1[0] * d0[2],
                     d1[0] * d0[1] - d1[1] * d0[0]
                 );
        normal = normalize( normal );
        tangent = vec3f(
                          d0[0] * d1[4] - d0[4] * d1[0],
                          d0[1] * d1[4] - d0[4] * d1[1],
                          d0[2] * d1[4] - d0[4] * d1[2]
                      );
        tangent = normalize( tangent );

        for ( int j = 0; j < 3; j++ ) {
            u32 idx = mesh.triangles[i].index[j];
            Md5Vertex& vert = mesh.vertices[idx];
            for ( int k = 0; k < vert.count; k++ ) {
                Md5Weight& weight = mesh.weights[vert.start + k];
                weight.normal += normal;
                weight.tangent += tangent;
            }
        }
    }

    for ( i32 i = 0; i < mesh.num_weights; i++ ) {
        Md5Weight& weight = mesh.weights[i];
        weight.normal = normalize( weight.normal );
        f32 d = dot( weight.tangent, weight.normal );
        weight.tangent = normalize( weight.tangent - weight.normal * d );
    }

    math::vec3f* tangent = mesh_->findAttributeTyped<math::vec3f>(VertexAttrs::tagTangent);
    math::vec3f* n = mesh_->findAttributeTyped<math::vec3f>(VertexAttrs::tagNormal);

    const Md5Joint* skeleton = md5->baseSkel;
    for ( i32 i = 0; i < mesh.num_verts; ++i ) {
        n[i] = vec3f( 0.0f );
        tangent[i] = vec3f( 0.0f );

        for ( u32 j = 0; j < mesh.vertices[i].count; j++ ) {
            const Md5Weight& weight = mesh.weights[mesh.vertices[i].start + j];
            const Md5Joint& joint = skeleton[weight.joint];
            n[i] += joint.orient.RotatePoint( weight.normal );
            tangent[i] += joint.orient.RotatePoint( weight.tangent );
        }

        n[i] = normalize( n[i] );
        tangent[i] = normalize( tangent[i] );
    }
}

void Md5Renderer::UpdateBoundingBox( math::vec3f& pos )
{
    // TODO bounding box class?
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
