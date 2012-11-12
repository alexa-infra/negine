/**
 * \file
 * \author      Alexey Vasilyev <alexa.infra@gmail.com>
 * \copyright   MIT License
 **/
#include "renderer/mesh.h"
#include "renderer/vertexbuffer.h"

using base::math::Vector2;
using base::math::Vector3;
using base::math::Vector4;

namespace base
{
namespace opengl
{

namespace VertexAttrs
{

const Vertex test;
const uptr offsets[] = {
    ( ( uptr )&test.pos ) - ( ( uptr )&test ),
    ( ( uptr )&test.n ) - ( ( uptr )&test ),
    ( ( uptr )&test.tex ) - ( ( uptr )&test ),
    ( ( uptr )&test.tangent ) - ( ( uptr )&test ),
    ( ( uptr )&test.binormal ) - ( ( uptr )&test ),
    ( ( uptr )&test.color ) - ( ( uptr )&test )
};

uptr GetOffset( VertexAttr attr )
{
    return offsets[attr];
}
u8 GetComponentCount( VertexAttr attr )
{
    switch ( attr ) {
    case tagPosition:
        return 3;
    case tagNormal:
        return 3;
    case tagTexture:
        return 2;
    case tagTangent:
        return 3;
    case tagBinormal:
        return 3;
    case tagColor:
        return 4;
    default:
        return 0;
    }
}

} // namespace VertexAttr

Mesh::Mesh()
{
}

Mesh::~Mesh()
{
    delete[] vertexes;
    delete[] faces;
}

void Mesh::GenerateLightningInfo()
{
    for ( u32 i = 0; i < num_faces; i++ ) {
        f32 d0[5], d1[5];
        Vertex* a;
        Vertex* b;
        Vertex* c;
        math::Vector3 temp, normal, tangents[2];
        a = vertexes + faces[i].index[0];
        b = vertexes + faces[i].index[1];
        c = vertexes + faces[i].index[2];
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
            Vertex* v = vertexes + faces[i].index[j];
            v->n += normal;
            v->tangent  += tangents[0];
            v->binormal += tangents[1];
        }
    }

    for ( u32 i = 0; i < num_vertexes; i++ ) {
        vertexes[i].n.Normalize();
        f32 d = Dot( vertexes[i].tangent, vertexes[i].n );
        vertexes[i].tangent = vertexes[i].tangent - vertexes[i].n * d;
        vertexes[i].tangent.Normalize();
        d = Dot( vertexes[i].binormal, vertexes[i].n );
        vertexes[i].binormal = vertexes[i].binormal - vertexes[i].n * d;
        vertexes[i].binormal.Normalize();
    }
}

}
}
