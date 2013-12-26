/**
 * \file
 * \author      Alexey Vasilyev <alexa.infra@gmail.com>
 * \copyright   MIT License
 **/
#include "render/mesh.h"
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
    case tagBitangent:
        return 3;
    case tagColor:
        return 4;
    default:
        return 0;
    }
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
    case tagBitangent:
        return sizeof(vec3f);
    case tagColor:
        return sizeof(vec4f);
    default:
        return 0;
    }
}

} // namespace VertexAttr

MeshAttribute::MeshAttribute()
    : valid_(false)
{
}

MeshAttribute::MeshAttribute(VertexAttr attr, u32 idx, u32 start, u32 stride)
    : attr_(attr)
    , idx_(idx)
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
    u32 attrCounter[VertexAttrs::Count];
    memset(attrCounter, 0, VertexAttrs::Count * sizeof(u32));

    #define MESH_ATTR_IN_ARRAY
        rawSize_ = 0;
    #ifdef MESH_ATTR_IN_ARRAY
        for(u32 i=0; i<attr_.size(); i++) {
            VertexAttr attr = attr_[i];
            u32& idx = attrCounter[static_cast<u32>(attr)];
            u32 size = VertexAttrs::GetSize(attr);
            attributes_.push_back(MeshAttribute(attr, idx, rawSize_, size));
            rawSize_ += size * numVertexes_;
            idx++;
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
            u32& idx = attrCounter[static_cast<u32>(attr)];
            u32 size = VertexAttrs::GetSize(attr);
            attributes_.push_back(MeshAttribute(attr, idx, pos, vertexSize));
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

u32 Mesh::stride(VertexAttr attr, u32 idx) const
{
    const MeshAttribute& layer = getLayer(attr, idx);
    ASSERT(layer.valid_);
    return layer.stride_;
}

const MeshAttribute& Mesh::getLayer(VertexAttr attr, u32 idx) const {
    std::size_t size = attributes_.size();
    for(std::size_t i=0; i<size; i++) {
        const MeshAttribute& layer = attributes_[i];
        if (attr == layer.attr_ && idx == layer.idx_) {
            return layer;
        }
    }
    ASSERT(false);
    static MeshAttribute invalidLayer;
    return invalidLayer;
}

u8* Mesh::findAttributeRaw(VertexAttr attr, u32 idx) const
{
    const MeshAttribute& layer = getLayer(attr, idx);
    ASSERT(layer.valid_);
    return const_cast<u8*>(&attributeBuffer_[layer.start_]);
}

u32 Model::type_ = ResourceManager::registerResource();

Model::Model() {
    vertexSize_ = 0;
    indexSize_ = 0;
    currentSurface_ = nullptr;
}

size_t Model::surfaceCount() const {
    return surfaces_.size();
}

const Model::Surface& Model::surfaceAt(size_t i) const {
    return surfaces_.at(i);
}

Model::Surface& Model::beginSurface() {
    if (currentSurface_ != nullptr)
        endSurface();
    Surface m;
    surfaces_.push_back(m);
    currentSurface_ = &surfaces_.back();
    currentSurface_->vertexStart = vertexSize_;
    currentSurface_->indexStart = indexSize_;
    return *currentSurface_;
}

void Model::endSurface() {
    if (currentSurface_ == nullptr)
        return;
    const Mesh& mesh = currentSurface_->mesh;
    vertexSize_ += mesh.rawSize();
    if (mesh.indexType() == IndexTypes::UInt32)
        indexSize_ += mesh.numIndexes() * 4;
    else if (mesh.indexType() == IndexTypes::UInt16)
        indexSize_ += mesh.numIndexes() * 2;
    currentSurface_ = nullptr;
}

void Model::done() {
    if (currentSurface_ != nullptr)
        endSurface();
}

}
}
