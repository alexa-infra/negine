/**
 * \file
 * \brief       MD3 model loader
 * \author      Alexey Vasilyev <alexa.infra@gmail.com>
 * \copyright   MIT License
 **/
#pragma once

#include "base/types.h"
#include "renderer/mesh.h"

#include <vector>

namespace base {
namespace opengl {

std::vector<Mesh*> load_md3(const std::string& name);
std::vector<Mesh*> load_md3_se(const std::string& name);

}
}