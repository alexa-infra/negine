/**
 * \file
 * \brief       defines mesh-related things (vertexes, etc)
 * \author      Alexey Vasilyev <alexa.infra@gmail.com>
 * \copyright   MIT License
 **/
#pragma once

#include "base/types.h"
#include <vector>
#include "render/gl_lite.h"
#include "engine/resourceref.h"
#include "base/parameter.h"

namespace base {
namespace opengl {

namespace VertexAttrs
{
//! Tags for vertex components
enum VertexAttr {
    tagPosition,
    tagNormal,
    tagTexture,
    tagTangent,
    tagBitangent,
    tagColor,

    Count
};

//! Gets component count in attribute
u8 GetComponentCount( VertexAttr attr );

u32 GetGLType( VertexAttr attr );

u32 GetSize( VertexAttr attr );
}
typedef VertexAttrs::VertexAttr VertexAttr;

struct MeshAttribute
{
    VertexAttr attr_;
    u32 idx_;
    u32 start_;
    u32 stride_;
    bool valid_;

    MeshAttribute();
    MeshAttribute(VertexAttr attr, u32 idx, u32 start, u32 stride);
};

namespace IndexTypes
{
    enum IndexType {
        UInt16 = GL_UNSIGNED_SHORT,
        UInt32 = GL_UNSIGNED_INT
    };
}
typedef IndexTypes::IndexType IndexType;

class Mesh
{
public:
    NEGINE_API Mesh();
    NEGINE_API ~Mesh();

    NEGINE_API Mesh& addAttribute(VertexAttr attr);
    NEGINE_API Mesh& vertexCount(u32 nVertexes);
    NEGINE_API Mesh& indexCount(u32 nIndexes, IndexType type);
    NEGINE_API void complete();

    inline u32 numVertexes() const { return numVertexes_; }
    inline u32 numIndexes() const { return numIndexes_; }
    inline u32 rawSize() const { return rawSize_; }
    inline void* data() { return attributeBuffer_.data(); }
    inline const std::vector<MeshAttribute>& attributes() const { return attributes_; }
    inline void* indices() { return indices_.data(); }
    inline IndexType indexType() const { return indexType_; }

    ResourceRef material_;
    Params params_;

    template<typename T>
    T* findAttribute(VertexAttr attr, u32 idx = 0) const {
        return reinterpret_cast<T*>(findAttributeRaw(attr, idx));
    }

    const MeshAttribute& getLayer(VertexAttr attr, u32 idx) const;
private:
    u8* findAttributeRaw(VertexAttr attr, u32 idx) const;

private:
    std::vector<VertexAttr> attr_;
    u32 numVertexes_;
    u32 numIndexes_;
    std::vector<MeshAttribute> attributes_;
    std::vector<u8> attributeBuffer_;
    std::vector<u8> indices_;
    u32 rawSize_;
    IndexType indexType_;
};

} // namespace opengl
} // namespace base
