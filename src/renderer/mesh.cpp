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

u32 GetAttributeLocation( VertexAttr attr )
{
    return (u32)attr;
}

u32 GetGLType( VertexAttr attr )
{
    return GL_FLOAT;
}

u32 GetSize( VertexAttr attr )
{
    switch ( attr ) {
    case tagPosition:
        return sizeof(Vector3);
    case tagNormal:
        return sizeof(Vector3);
    case tagTexture:
        return sizeof(Vector2);
    case tagTangent:
        return sizeof(Vector3);
    case tagBinormal:
        return sizeof(Vector2);
    case tagColor:
        return sizeof(Vector4);
    default:
        return 0;
    }
}

} // namespace VertexAttr

}
}
