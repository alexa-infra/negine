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

class MeshLayer
{
public:
    VertexAttr attr_;
    u32 start_;
    u32 size_;
    u32 stride_;

    MeshLayer(VertexAttr attr, u32 start, u32 size, u32 stride)
         : attr_(attr), start_(start), size_(size), stride_(stride)
    {}
    ~MeshLayer() {}

private:
    DISALLOW_COPY_AND_ASSIGN(MeshLayer);
};

class MeshBuilder
{
public:
    std::vector<VertexAttr> attributes_;

    MeshBuilder& addAttribute(VertexAttr attr)
    {
        attributes_.push_back(attr);
        return *this;
    }
};

class MeshExt
{
private:
    u32 numVertexes_;
    u32 numIndexes_;
    std::vector<MeshLayer*> attributes_;
    u8* attributeBuffer_;
    u16* indices_;
    u32 rawSize_;
public:
    MeshExt(const MeshBuilder& builder, u32 nVertexes, u32 nIndexes)
        : numVertexes_(nVertexes)
        , numIndexes_(nIndexes)
        , rawSize_(0)
    {
        attributes_.reserve(builder.attributes_.size());
        for(u32 i=0; i<builder.attributes_.size(); i++)
        {
            VertexAttr attr = builder.attributes_[i];
            u32 size = VertexAttrs::GetSize(attr);
            MeshLayer* layer = new MeshLayer(attr, rawSize_, numVertexes_ * size, size);
            rawSize_ += size * numVertexes_;
            attributes_.push_back(layer);
        }
        attributeBuffer_ = new u8[rawSize_];
        indices_ = new u16[numIndexes_];
    }

    ~MeshExt()
    {
        for(size_t i=0; i<attributes_.size(); i++)
        {
            delete attributes_[i];
        }
        delete[] indices_;
        delete[] attributeBuffer_;
    }

    u32 numVertexes() const { return numVertexes_; }
    u32 numIndexes() const { return numIndexes_; }
    u32 rawSize() const { return rawSize_; }
    void* data() { return attributeBuffer_; }
    const std::vector<MeshLayer*>& attributes() const { return attributes_; }
    u16* indices() { return indices_; }

    template<typename T>
    T* findAttributeTyped(VertexAttr attr)
    {
        return (T*)findAttribute(attr);
    }

    void* findAttribute(VertexAttr attr)
    {
        for(size_t i=0; i<attributes_.size(); i++)
        {
            if (attributes_[i]->attr_ == attr)
            {
                return attributeBuffer_ + attributes_[i]->start_;
            }
        }
        return nullptr;
    }

private:
    DISALLOW_COPY_AND_ASSIGN(MeshExt);
};


} // namespace opengl
} // base
