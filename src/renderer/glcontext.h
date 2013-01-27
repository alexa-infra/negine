/**
 * \file
 * \brief       OpenGL context
 * \author      Alexey Vasilyev <alexa.infra@gmail.com>
 * \copyright   MIT License
 **/
#pragma once

#include "base/platform.h"
#ifndef GLEW_STATIC
#define GLEW_STATIC
#endif
#include "GL/glew.h"
#ifdef OS_MAC
#include <OpenGL/gl.h>
#else
#include <GL/gl.h>
#endif

#ifdef OS_WIN
#include "GL/wglew.h"
#elif defined(OS_LINUX)
#include "GL/glxew.h"
#endif

#ifdef __APPLE__
#undef glGenVertexArrays
#undef glDeleteVertexArrays
#undef glBindVertexArray
#define glGenVertexArrays      glGenVertexArraysAPPLE
#define glDeleteVertexArrays   glDeleteVertexArraysAPPLE
#define glBindVertexArray      glBindVertexArrayAPPLE
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
