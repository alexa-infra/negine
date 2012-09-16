/**
 * \file
 * \brief       Md5 mesh renderer
 * \author      Alexey Vasilyev <alexa.infra@gmail.com>
 * \copyright   MIT License
 **/
#include "renderer/md5renderer.h"

using base::resource::Md5Mesh;
using base::resource::Md5Model;
using base::resource::Md5Model;
using base::resource::Md5Joint;
using base::resource::Md5Weight;

namespace base
{
namespace opengl
{

Md5Renderer::Md5Renderer( Md5Model* model )
    : vertexArray( NULL )
    , vertexIndices( NULL )
    , vb( NULL )
    , md5( model )
{
    vb = new VertexBufferMemory;
    Md5Mesh& mesh = md5->meshes[0];
    vertexArray = new Vertex[mesh.num_verts];
    vertexIndices = new Face[mesh.num_tris];
}

Md5Renderer::~Md5Renderer()
{
    delete vb;
    delete[] vertexArray;
    delete[] vertexIndices;
}

void Md5Renderer::Draw( AttributeBinding& binding )
{
    vb->Draw( binding );
}

void Md5Renderer::Commit()
{
    Md5Mesh& mesh = md5->meshes[0];
    GenerateVertexes( mesh, vertexArray );
    GenerateIndexes( mesh, vertexIndices );
    GenerateLightningInfo( mesh, vertexArray );
    vb->SetData( vertexArray,
                 mesh.num_verts,
                 vertexIndices,
                 mesh.num_tris );
}

void Md5Renderer::GenerateVertexes( Md5Mesh& mesh, Vertex* vertexes )
{
    const Md5Joint* skeleton = md5->baseSkel;
    Vertex* currentVertex = vertexes;

    for ( i32 i = 0; i < mesh.num_verts; ++i ) {
        currentVertex->tex = mesh.vertices[i].st;
        currentVertex->pos.set( 0.0f );
        currentVertex->n.set( 0.0f );
        currentVertex->tangent.set( 0.0f );

        for ( u32 j = 0; j < mesh.vertices[i].count; j++ ) {
            const Md5Weight& weight = mesh.weights[mesh.vertices[i].start + j];
            const Md5Joint& joint = skeleton[weight.joint];
            currentVertex->pos += joint.translate( weight.pos ) * weight.bias;
            currentVertex->n += joint.orient.RotatePoint( weight.normal );
            currentVertex->tangent += joint.orient.RotatePoint( weight.tangent );
        }

        currentVertex->n.Normalize();
        currentVertex->tangent.Normalize();
        UpdateBoundingBox( *currentVertex );
        currentVertex++;
    }
}

void Md5Renderer::GenerateIndexes( Md5Mesh& mesh, Face* indexes )
{
    for ( int i = 0; i < mesh.num_tris; i++ ) {
        for ( int j = 0; j < 3; j++ ) {
            indexes[i].index[j] = ( u16 )mesh.triangles[i].index[j];
        }
    }
}

void Md5Renderer::GenerateLightningInfo( Md5Mesh& mesh, Vertex* vertexes )
{
    for ( i32 i = 0; i < mesh.num_tris; i++ ) {
        f32 d0[5], d1[5];
        math::Vector3 temp, normal, tangents[2];
        Vertex* a = vertexes + mesh.triangles[i].index[0];
        Vertex* b = vertexes + mesh.triangles[i].index[1];
        Vertex* c = vertexes + mesh.triangles[i].index[2];
        d0[0] = b->pos.x - a->pos.x;
        d0[1] = b->pos.y - a->pos.y;
        d0[2] = b->pos.z - a->pos.z;
        d0[3] = b->tex.x - a->tex.x;
        d0[4] = b->tex.y - a->tex.y;
        d1[0] = c->pos.x - a->pos.x;
        d1[1] = c->pos.y - a->pos.y;
        d1[2] = c->pos.z - a->pos.z;
        d1[3] = c->tex.x - a->tex.x;
        d1[4] = c->tex.y - a->tex.y;
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
            Vertex* v = vertexes + mesh.triangles[i].index[j];
            v->n += normal;
            v->tangent  += tangents[0];
            v->binormal += tangents[1];
        }
    }

    for ( i32 i = 0; i < mesh.num_verts; i++ ) {
        vertexes[i].n.Normalize();
        f32 d = Dot( vertexes[i].tangent, vertexes[i].n );
        vertexes[i].tangent = vertexes[i].tangent - vertexes[i].n * d;
        vertexes[i].tangent.Normalize();
        d = Dot( vertexes[i].binormal, vertexes[i].n );
        vertexes[i].binormal = vertexes[i].binormal - vertexes[i].n * d;
        vertexes[i].binormal.Normalize();
    }
}

void Md5Renderer::UpdateBoundingBox( Vertex& vertex )
{
    if ( boundingBox.min.x > vertex.pos.x ) {
        boundingBox.min.x = vertex.pos.x;
    }

    if ( boundingBox.min.y > vertex.pos.y ) {
        boundingBox.min.y = vertex.pos.y;
    }

    if ( boundingBox.min.z > vertex.pos.z ) {
        boundingBox.min.z = vertex.pos.z;
    }

    if ( boundingBox.max.x < vertex.pos.x ) {
        boundingBox.max.x = vertex.pos.x;
    }

    if ( boundingBox.max.y < vertex.pos.y ) {
        boundingBox.max.y = vertex.pos.y;
    }

    if ( boundingBox.max.z < vertex.pos.z ) {
        boundingBox.max.z  = vertex.pos.z;
    }
}

} // namespace base
} // namespace opengl
