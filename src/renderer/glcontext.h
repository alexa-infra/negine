/**
 * \file
 * \brief       OpenGL context
 * \author      Alexey Vasilyev <alexa.infra@gmail.com>
 * \copyright   MIT License
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