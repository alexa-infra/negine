#pragma once

#include <vector>
#include "render/mesh.h"
#include "render/material.h"

namespace base {
namespace opengl {

class ModelLoader
{
public:
    std::vector<Mesh*> subMeshes;

    NEGINE_API static Model* load(const std::string& filename);
private:
};

} // namespace opengl
} // namespace base