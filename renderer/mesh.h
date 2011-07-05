#pragma once

#include "base/types.h"
#include "base/math/vector.h"
#include <list>
#include <vector>

namespace base {
namespace opengl {

using namespace base::math;

struct Vertex
{
    Vector3 pos;
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
    u8 GetOffset(VertexAttr attr);
    u8 GetComponentCount(VertexAttr attr);
}
typedef VertexAttrs::VertexAttr VertexAttr;

typedef std::list< std::pair<VertexAttr, u32> > AttributeBinding;

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

class Mesh
{
public:
    std::string name;

    Vertex* vertexes;
    Face*   faces;
    u32     num_vertexes;
    u32     num_faces;

    Mesh();
    ~Mesh();
};


} // namespace opengl
} // base
