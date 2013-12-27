/**
 * \file
 * \brief       OpenGL context
 * \author      Alexey Vasilyev <alexa.infra@gmail.com>
 * \copyright   MIT License
 **/
#pragma once

#include "base/types.h"
#include "render/gl_lite.h"
#include "math/vec4.h"

namespace base {
namespace opengl {

#ifdef _DEBUG
#define GL_ASSERT(gl) gl.Assert(__FILE__, __LINE__)
#else
#define GL_ASSERT(gl) {}
#endif

class GLFuncLoader;
class RenderState;
class GpuProgram;
class BufferObject;

class NEGINE_API DeviceContext
{
public:
    DeviceContext();
    ~DeviceContext();

    PFNGLACTIVETEXTUREPROC      ActiveTexture;
    PFNGLATTACHSHADERPROC       AttachShader;
    PFNGLBINDATTRIBLOCATIONPROC BindAttribLocation;
    PFNGLBINDBUFFERPROC         BindBuffer;
    PFNGLBINDBUFFERBASEPROC     BindBufferBase;
    PFNGLBINDBUFFERRANGEPROC    BindBufferRange;
    PFNGLBINDTEXTUREPROC        BindTexture;
    PFNGLBINDVERTEXARRAYPROC    BindVertexArray;
    PFNGLBUFFERDATAPROC         BufferData;
    PFNGLBUFFERSUBDATAPROC      BufferSubData;
    PFNGLCOMPILESHADERPROC      CompileShader;
    PFNGLCREATEPROGRAMPROC      CreateProgram;
    PFNGLCREATESHADERPROC       CreateShader;
    PFNGLDELETEBUFFERSPROC      DeleteBuffers;
    PFNGLDELETEPROGRAMPROC      DeleteProgram;
    PFNGLDELETESHADERPROC       DeleteShader;
    PFNGLDELETETEXTURESPROC     DeleteTextures;
    PFNGLDELETEVERTEXARRAYSPROC DeleteVertexArrays;
    PFNGLDETACHSHADERPROC       DetachShader;
    PFNGLDISABLEVERTEXATTRIBARRAYPROC   DisableVertexAttribArray;
    PFNGLENABLEVERTEXATTRIBARRAYPROC    EnableVertexAttribArray;
    PFNGLDRAWELEMENTSPROC       DrawElements;
    PFNGLDRAWARRAYSPROC         DrawArrays;
    PFNGLGENBUFFERSPROC         GenBuffers;
    PFNGLGENTEXTURESPROC        GenTextures;
    PFNGLGENVERTEXARRAYSPROC    GenVertexArrays;
    PFNGLGENERATEMIPMAPPROC     GenerateMipmap;
    PFNGLGETACTIVEUNIFORMPROC   GetActiveUniform;
    PFNGLGETBUFFERSUBDATAPROC   GetBufferSubData;
    PFNGLGETPROGRAMINFOLOGPROC  GetProgramInfoLog;
    PFNGLGETPROGRAMIVPROC       GetProgramiv;
    PFNGLGETSHADERINFOLOGPROC   GetShaderInfoLog;
    PFNGLGETSHADERIVPROC        GetShaderiv;
    PFNGLGETUNIFORMLOCATIONPROC GetUniformLocation;
    PFNGLLINKPROGRAMPROC        LinkProgram;
    PFNGLSHADERSOURCEPROC       ShaderSource;
    PFNGLTEXIMAGE2DPROC         TexImage2D;
    PFNGLTEXPARAMETERIPROC      TexParameteri;
    PFNGLTEXPARAMETERFPROC      TexParameterf;
    PFNGLUNIFORM1IPROC          Uniform1i;
    PFNGLUNIFORM1FPROC          Uniform1f;
    PFNGLUNIFORM3FPROC          Uniform3f;
    PFNGLUNIFORM4FPROC          Uniform4f;
    PFNGLUNIFORMMATRIX4FVPROC   UniformMatrix4fv;
    PFNGLUSEPROGRAMPROC         UseProgram;
    PFNGLVERTEXATTRIBPOINTERPROC        VertexAttribPointer;

    PFNGLCLEARPROC              Clear;
    PFNGLCLEARCOLORPROC         ClearColor;
    PFNENABLEPROC               Enable;
    PFNGLGETERRORPROC           GetError;
    PFNGLDISABLEPROC            Disable;
    PFNGETSTRINGPROC            GetString;
    PFNBLENDFUNCPROC            BlendFunc;
    PFNGLVIEWPORTPROC           Viewport;
    PFNGLDEPTHMASKPROC          DepthMask;

    void Assert(const char* file, int line);

    void init();

    void setCullface(bool enable);
    void setDepthTest(bool enable);
    void setBlend(bool enable);
    void setDepthWrite(bool enable);
    void setViewport(const math::vec4f& area);
    void setProgram(GpuProgram* pr);
    void setVertexBuffer(BufferObject* buffer);
    void setIndexBuffer(BufferObject* buffer);
    void setTextureUnit(u32 id);

    RenderState& renderState();
private:
    GLFuncLoader* loader;
    RenderState* state;

private:
    DISALLOW_COPY_AND_ASSIGN( DeviceContext );
};

} // namespace opengl
} // namespace base
