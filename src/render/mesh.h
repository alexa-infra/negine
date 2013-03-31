/**
 * \file
 * \brief       defines mesh-related things (vertexes, etc)
 * \author      Alexey Vasilyev <alexa.infra@gmail.com>
 * \copyright   MIT License
 **/
#pragma once

#include "base/types.h"
#include <vector>

namespace base
{
namespace opengl
{

namespace VertexAttrs
{
//! Tags for vertex components
enum VertexAttr {
    tagPosition,
    tagNormal,
    tagTexture,
    tagTangent,
    tagBinormal,
    tagColor,

    Count
};

//! Gets component count in attribute
u8 GetComponentCount( VertexAttr attr );

u32 GetAttributeLocation( VertexAttr attr );

u32 GetGLType( VertexAttr attr );

u32 GetSize( VertexAttr attr );
}
typedef VertexAttrs::VertexAttr VertexAttr;

struct MeshLayer
{
    VertexAttr attr_;
    u32 start_;
    u32 stride_;
    bool valid_;

    MeshLayer();
    MeshLayer(VertexAttr attr, u32 start, u32 stride);
};

class MeshBuilder
{
public:
    std::vector<VertexAttr> attributes_;

    enum Type
    {
        Static,
        Dynamic
    };
    Type type_;

    MeshBuilder();
    MeshBuilder& addAttribute(VertexAttr attr);
};

class MeshExt
{
private:
    u32 numVertexes_;
    u32 numIndexes_;
    MeshLayer attributes_[VertexAttrs::Count];
    std::vector<u8> attributeBuffer_;
    std::vector<u16> indices_;
    u32 rawSize_;
public:
    MeshExt(const MeshBuilder& builder, u32 nVertexes, u32 nIndexes);
    ~MeshExt();

    u32 numVertexes() const { return numVertexes_; }
    u32 numIndexes() const { return numIndexes_; }
    u32 rawSize() const { return rawSize_; }
    void* data() { return &attributeBuffer_.front(); }
    const MeshLayer* attributes() const { return attributes_; }
    u16* indices() { return &indices_.front(); }

    u32 stride(VertexAttr attr) const;

    template<typename T>
    T* findAttributeTyped(VertexAttr attr) {
        return reinterpret_cast<T*>(findAttributeRaw(attr));
    }

    template<typename T>
    T* findElement(VertexAttr attr, u32 idx) {
        return reinterpret_cast<T*>(findElementRaw(attr, idx));
    }

    void reserve(u32 vertexCount, u32 indexCount);
private:
    u8* findAttributeRaw(VertexAttr attr);

    u8* findElementRaw(VertexAttr attr, u32 idx);
private:
    DISALLOW_COPY_AND_ASSIGN(MeshExt);
};


} // namespace opengl
} // base
