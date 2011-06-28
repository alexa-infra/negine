#pragma once

#include "base/types.h"
#include "base/math/vector.h"

namespace base {
namespace opengl {

using namespace base::math;

struct Vertex
{
    Vector4 pos;
    Vector3 n;
    Vector2 tex;
    Vector4 color;
};

namespace VertexAttrs
{
    enum VertexAttr
    {
        tagPosition,
        tagNormal,
        tagTexture,
        tagColor
    };
}

struct Face
{
    u32 index[3];
    Vector3 n;
};

struct Edge
{
    u32 a, b;
    u32 c, d;
};

struct Mesh
{
	std::vector<Vertex>     vertices;
	std::vector<Face>       faces;
	std::vector<Edge>       edges;
};


} // namespace opengl
} // base
