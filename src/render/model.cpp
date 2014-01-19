/**
 * \file
 * \author      Alexey Vasilyev <alexa.infra@gmail.com>
 * \copyright   MIT License
 **/
#include "render/model.h"
#include "math/vec4.h"
#include "base/debug.h"

using base::math::vec2f;
using base::math::vec3f;
using base::math::vec4f;

namespace base {

template<>
ResourceType ResourceBase<opengl::Model>::type_ = ResourceManager::registerResource();

namespace opengl {

Model::Model() {
    vertexSize_ = 0;
    indexSize_ = 0;
    currentSurface_ = nullptr;
}

Model::~Model() {
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

} // namespace opengl
} // namespace base
