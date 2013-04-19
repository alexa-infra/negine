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

class DeviceContext
{
    GLFuncLoader* loader;
    TextureLoader* texture_loader_;
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
    PFNGLMAPBUFFERPROC          MapBuffer;
    PFNGLSHADERSOURCEPROC       ShaderSource;
    PFNGLTEXIMAGE2DPROC         TexImage2D;
    PFNGLTEXPARAMETERIPROC      TexParameteri;
    PFNGLUNIFORM1IPROC          Uniform1i;
    PFNGLUNIFORM3FPROC          Uniform3f;
    PFNGLUNIFORM4FPROC          Uniform4f;
    PFNGLUNIFORMMATRIX4FVPROC   UniformMatrix4fv;
    PFNGLUNMAPBUFFERPROC        UnmapBuffer;
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

    void Assert(const char* file, int line);

    TextureLoader* texture_loader();

    void init();
private:
    DISALLOW_COPY_AND_ASSIGN( DeviceContext );
};

} // namespace opengl
} // namespace base
