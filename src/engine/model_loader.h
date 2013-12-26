#pragma once

#include <vector>
#include "render/model.h"
#include "render/material.h"

namespace base {
namespace opengl {

NEGINE_API Model* loadModel(const std::string& filename);

} // namespace opengl
} // namespace base