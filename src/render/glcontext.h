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

void glAssert(const char* file, int line);
#ifdef _DEBUG
#define GL_ASSERT() ::base::opengl::glAssert(__FILE__, __LINE__)
#else
#define GL_ASSERT() {}
#endif

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
