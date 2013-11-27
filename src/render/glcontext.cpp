#include "render/glcontext.h"
#include "base/log.h"
#include "base/debug.h"
#include "render/texture.h"
#include "render/renderstate.h"
#include <type_traits>

#ifdef OS_WIN
    #define WIN32_LEAN_AND_MEAN
    #include <windows.h>
#endif

#ifdef OS_UNIX
    #include <dlfcn.h>
#endif

namespace base
{
namespace opengl
{

const char* glErrorToString(GLenum code)
{
    if (code == GL_INVALID_VALUE)     return "GL_INVALID_VALUE";
    if (code == GL_INVALID_ENUM)      return "GL_INVALID_ENUM";
    if (code == GL_INVALID_OPERATION) return "GL_INVALID_OPERATION";
    if (code == GL_OUT_OF_MEMORY)     return "GL_OUT_OF_MEMORY";
    return "UNKNOWN";
}

void DeviceContext::Assert(const char* file, int line)
{
    GLenum err = GetError();
    if (err != GL_NO_ERROR)
    {
        ERR("GL error: %s [%#X] @ %s, %d", glErrorToString(err), err, file, line);
        debugBreak();
    }
}

TextureLoader* DeviceContext::texture_loader()
{
    ASSERT(texture_loader_ != NULL);
    return texture_loader_;
}

RenderState& DeviceContext::renderState()
{
    ASSERT(state != NULL);
    return *state;    
}

#ifdef OS_WIN
class Library
{
public:
    Library(const char* name)
    {
        module_ = LoadLibraryA(name);
        ASSERT(module_ != 0);
    }
    ~Library()
    {
        FreeLibrary(module_);
    }
    void* getFunc(const char* func)
    {
        return reinterpret_cast<void*>(GetProcAddress(module_, func));
    }
private:
    HMODULE module_;
};
#elif defined(OS_UNIX)
class Library
{
public:
    Library(const char* name)
    {
        module_ = dlopen(name, RTLD_NOW | RTLD_GLOBAL); 
        ASSERT(module_ != 0);
    }
    ~Library()
    {
        dlclose(module_);
    }
    void* getFunc(const char* func)
    {
        return dlsym(module_, func);
    }
private:
    void* module_;
};
#endif

class GLFuncLoader
{
public:
#ifdef OS_WIN
    GLFuncLoader()
        : libgl("opengl32.dll")
    {
        glGetProcAddress = reinterpret_cast<GLGETPROCADDRESSPROC>(libgl.getFunc("wglGetProcAddress"));
        ASSERT(glGetProcAddress != NULL);
    }
#elif defined(OS_MAC)
    GLFuncLoader()
        : libgl("/System/Library/Frameworks/OpenGL.framework/Libraries/libGL.dylib")
    {
    }
#elif defined(OS_LINUX)
    GLFuncLoader()
        : libgl("libGL.so.1")
    {
    }
#endif

    template<typename Func>
    Func getPointer(const char* name) {
#ifdef OS_WIN
        void* ptr = glGetProcAddress(name);
        if (ptr == NULL)
            ptr = libgl.getFunc(name);
#elif defined(OS_UNIX)
        void* ptr = libgl.getFunc(name);
#endif
        return reinterpret_cast<Func>(ptr);
    }
    
    template<typename Func>
    void getPointerWrap(Func& ptr, const char* name) {
        typedef typename std::remove_reference<Func>::type nonref;
        ptr = getPointer<nonref>(name);
        ASSERT(ptr != NULL);
    }
private:
    Library libgl;
#ifdef OS_WIN
    typedef void* (WINAPI *GLGETPROCADDRESSPROC)(const char*);
    GLGETPROCADDRESSPROC glGetProcAddress;
#endif
};

DeviceContext::DeviceContext()
    : loader(NULL)
    , state(NULL)
{
}

DeviceContext::~DeviceContext()
{
    delete loader;
    delete state;
}

void DeviceContext::init()
{
    ASSERT(loader == NULL);
    loader = new GLFuncLoader;

    #define LOAD_GL(name) loader->getPointerWrap( name, "gl"#name );
    
    LOAD_GL(ActiveTexture                    );
    LOAD_GL(AttachShader                     );
    LOAD_GL(BindAttribLocation               );
    LOAD_GL(BindBuffer                       );
    LOAD_GL(BindBufferBase                   );
    LOAD_GL(BindBufferRange                  );
    LOAD_GL(BindTexture                      );
    LOAD_GL(BindVertexArray                  );
    LOAD_GL(BufferData                       );
    LOAD_GL(BufferSubData                    );
    LOAD_GL(CompileShader                    );
    LOAD_GL(CreateProgram                    );
    LOAD_GL(CreateShader                     );
    LOAD_GL(DeleteBuffers                    );
    LOAD_GL(DeleteProgram                    );
    LOAD_GL(DeleteShader                     );
    LOAD_GL(DeleteTextures                   );
    LOAD_GL(DeleteVertexArrays               );
    LOAD_GL(DetachShader                     );
    LOAD_GL(DisableVertexAttribArray         );
    LOAD_GL(DrawElements                     );
    LOAD_GL(EnableVertexAttribArray          );
    LOAD_GL(GenBuffers                       );
    LOAD_GL(GenTextures                      );
    LOAD_GL(GenVertexArrays                  );
    LOAD_GL(GenerateMipmap                   );
    LOAD_GL(GetActiveUniform                 );
    LOAD_GL(GetBufferSubData                 );
    LOAD_GL(GetProgramInfoLog                );
    LOAD_GL(GetProgramiv                     );
    LOAD_GL(GetShaderInfoLog                 );
    LOAD_GL(GetShaderiv                      );
    LOAD_GL(GetUniformLocation               );
    LOAD_GL(LinkProgram                      );
    LOAD_GL(ShaderSource                     );
    LOAD_GL(TexImage2D                       );
    LOAD_GL(TexParameteri                    );
    LOAD_GL(TexParameterf);
    LOAD_GL(Uniform1i                        );
    LOAD_GL(Uniform3f                        );
    LOAD_GL(Uniform4f                        );
    LOAD_GL(UniformMatrix4fv                 );
    LOAD_GL(UseProgram                       );
    LOAD_GL(VertexAttribPointer              );

    LOAD_GL(Clear                            );
    LOAD_GL(ClearColor                       );
    LOAD_GL(Enable                           );
    LOAD_GL(GetError                         );
    LOAD_GL(Disable                          );
    LOAD_GL(GetString                        );
    LOAD_GL(BlendFunc                        );
    LOAD_GL(Viewport                         );
    LOAD_GL(DepthMask                        );

    #undef LOAD_GL

    texture_loader_ = new TextureLoader(*this);
    state = new RenderState(*this);
}

}
}
