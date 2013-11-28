/**
 * \file
 * \brief       OpenGL context
 * \author      Alexey Vasilyev <alexa.infra@gmail.com>
 * \copyright   MIT License
 **/
#pragma once

#include "base/types.h"
#include "render/gl_lite.h"

namespace base
{
namespace opengl
{

#ifdef _DEBUG
#define GL_ASSERT(gl) gl.Assert(__FILE__, __LINE__)
#else
#define GL_ASSERT(gl) {}
#endif

class TextureLoader;
class GLFuncLoader;
class RenderState;

class NEGINE_API DeviceContext
{
    GLFuncLoader* loader;
    TextureLoader* texture_loader_;
    RenderState* state;
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

    TextureLoader* texture_loader();

    RenderState& renderState();

    void init();
private:
    DISALLOW_COPY_AND_ASSIGN( DeviceContext );
};

} // namespace opengl
} // namespace base
