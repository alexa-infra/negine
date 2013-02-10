#include "render/glcontext.h"

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

void glAssert(const char* file, int line)
{
    GLenum err = glGetError();
    if (err != GL_NO_ERROR)
    {
        ERR("GL error: %s [%x] @ %s, %d\n", glErrorToString(err), err, file, line);
        abort();
    }
}

}
}
