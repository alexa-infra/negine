/**
 * @author  Alexey Vasilyev <alexa.infra@gmail.com>
 * @section LICENSE
 * 
 * This file is part of Negine (https://alexadotlife.com/hg/negine)
 * Licensed under LGPLv3 (or GLPv3) – see LICENSE file for details
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