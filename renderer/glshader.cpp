#include "renderer/glshader.h"

namespace ext {
namespace opengl {

Shader::Shader(GLenum type) 
    : id_(0)
    , is_ok_(false)
    , compiled_(false)
    , shader_type_(type) {
    id_ = glCreateShader(shader_type_);
    is_ok_ = (glIsShader(id_) == GL_TRUE);
}

Shader::~Shader() {
    if (is_ok_)
        glDeleteShader(id_);
}

bool Shader::Compile(const std::string& source, std::string& status) {
    if (!is_ok_)
        return false;
    const char* shader_source = source.c_str();
    glShaderSource(id_, 1, &shader_source, NULL);
    glCompileShader(id_);

    GLint compile_status;
    glGetShaderiv(id_, GL_COMPILE_STATUS, &compile_status);
    if (compiled_ = (compile_status == GL_TRUE))
        return true;
    GLint log_size;
    glGetShaderiv(id_, GL_INFO_LOG_LENGTH, &log_size);
    if (log_size == 0)
        return false;
    status.resize(static_cast<u32>(log_size));
    char* buffer = const_cast<char*>(status.c_str());
    glGetInfoLogARB(id_, log_size, NULL, buffer);
    return false;
}

}
}
