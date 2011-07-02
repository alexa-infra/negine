#pragma once

#include "base/types.h"
#include "renderer/mesh.h"

#include <vector>

namespace base {
namespace opengl {

std::vector<Mesh*> load_md3(const std::string& name);

}
}