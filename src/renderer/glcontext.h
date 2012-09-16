/**
 * \file
 * \brief       OpenGL context
 * \author      Alexey Vasilyev <alexa.infra@gmail.com>
 * \copyright   MIT License
 **/
#pragma once

#include "base/platform.h"
#include "GL/glew.h"
#ifdef OS_MAC
#include <OpenGL/gl.h>
#else
#include <GL/gl.h>
#endif

#include "base/types.h"

namespace base
{
namespace opengl
{

class TextureLoader;

class DeviceContext
{
    TextureLoader* texture_loader_;
public:
    DeviceContext();
    ~DeviceContext();
private:
    DISALLOW_COPY_AND_ASSIGN( DeviceContext );
};

} // namespace opengl
} // namespace base
