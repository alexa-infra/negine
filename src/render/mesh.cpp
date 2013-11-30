/**
 * \file
 * \author      Alexey Vasilyev <alexa.infra@gmail.com>
 * \copyright   MIT License
 **/
#include "render/mesh.h"
#include "render/vertexbuffer.h"
#include "render/glcontext.h"
#include "math/vec4.h"
#include "base/debug.h"

using base::math::vec2f;
using base::math::vec3f;
using base::math::vec4f;

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
    return static_cast<u32>(attr);
}

u32 GetGLType( VertexAttr attr )
{
    return GL_FLOAT;
}

u32 GetSize( VertexAttr attr )
{
    switch ( attr ) {
    case tagPosition:
        return sizeof(vec3f);
    case tagNormal:
        return sizeof(vec3f);
    case tagTexture:
        return sizeof(vec2f);
    case tagTangent:
        return sizeof(vec3f);
    case tagBinormal:
        return sizeof(vec3f);
    case tagColor:
        return sizeof(vec4f);
    default:
        return 0;
    }
}

} // namespace VertexAttr

MeshLayer::MeshLayer()
    : valid_(false)
{
}

MeshLayer::MeshLayer(VertexAttr attr, u32 start, u32 stride)
    : attr_(attr)
    , start_(start)
    , stride_(stride)
    , valid_(true)
{
}

Mesh::Mesh()
    : numVertexes_(0)
    , numIndexes_(0)
    , rawSize_(0)
{
}

Mesh::~Mesh()
{
}

Mesh& Mesh::addAttribute(VertexAttr attr)
{
    attr_.push_back(attr);
    return *this;
}

Mesh& Mesh::vertexCount(u32 nVertexes)
{
    numVertexes_ = nVertexes;
    return *this;
}

Mesh& Mesh::indexCount(u32 nIndexes, IndexType type)
{
    indexType_ = type;
    numIndexes_ = nIndexes;
    return *this;    
}

void Mesh::complete()
{
    #define MESH_ATTR_IN_ARRAY
        rawSize_ = 0;
        for(u32 i=0; i<VertexAttrs::Count; i++) {
            attributes_[i] = MeshLayer();
        }
    #ifdef MESH_ATTR_IN_ARRAY
        for(u32 i=0; i<attr_.size(); i++) {
            VertexAttr attr = attr_[i];
            u32 size = VertexAttrs::GetSize(attr);
            attributes_[attr] = MeshLayer(attr, rawSize_, size);
            rawSize_ += size * numVertexes_;
        }
    #else
        u32 vertexSize = 0;
        for(u32 i=0; i<attr_.size(); i++) {
            VertexAttr attr = attr_[i];
            vertexSize += VertexAttrs::GetSize(attr);
        }
        u32 pos = 0;
        for(u32 i=0; i<attr_.size(); i++) {
            VertexAttr attr = attr_[i];
            u32 size = VertexAttrs::GetSize(attr);
            attributes_[attr] = MeshLayer(attr, pos, vertexSize);
            pos += size;
        }
        rawSize_ += vertexSize * numVertexes_;
    #endif
    attributeBuffer_.resize(rawSize_);
    if (numIndexes_ != 0) {
        if (indexType_ == IndexTypes::UInt16)
            indices_.resize(numIndexes_ * 2);
        else if (indexType_ == IndexTypes::UInt32)
            indices_.resize(numIndexes_ * 4);
    }
}

u32 Mesh::stride(VertexAttr attr) const
{
    const MeshLayer& layer = attributes_[attr];
    ASSERT(layer.valid_);
    return layer.stride_;
}

u8* Mesh::findAttributeRaw(VertexAttr attr)
{
    const MeshLayer& layer = attributes_[attr];
    ASSERT(layer.valid_);
    return &attributeBuffer_[layer.start_];
}

u8* Mesh::findElementRaw(VertexAttr attr, u32 idx)
{
    ASSERT(idx < numVertexes_);
    const MeshLayer& layer = attributes_[attr];
    ASSERT(layer.valid_);
    return &attributeBuffer_[layer.start_ + layer.stride_ * idx];
}

}
}
