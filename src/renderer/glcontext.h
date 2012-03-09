/**
 * @author  Alexey Vasilyev <alexa.infra@gmail.com>
 * @section LICENSE
 * 
 * This file is part of Negine (https://hg.alexadotlife.com/negine)
 * Licensed under LGPLv3 (or GPLv3) - see LICENSE file for details
 **/
#pragma once

#include "GL/glew.h"
#include <GL/gl.h>

#include "base/types.h"

namespace base {
namespace opengl {

class TextureLoader;

class DeviceContext {
    TextureLoader* texture_loader_;
public:
    DeviceContext();
    ~DeviceContext();
private:
    DISALLOW_COPY_AND_ASSIGN(DeviceContext);
};

} // namespace opengl
} // namespace base