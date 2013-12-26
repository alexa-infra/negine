#pragma once

#include <vector>
#include "render/mesh.h"
#include "render/material.h"

namespace base {
namespace opengl {

NEGINE_API Model* loadModel(const std::string& filename);

} // namespace opengl
} // namespace base