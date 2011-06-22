#pragma once

#include "renderer/glcontext.h"

namespace base {
namespace opengl {

class Shader {
private:
    u32 id_;
    bool is_ok_;
    bool compiled_;
    GLenum shader_type_;
    std::string status_;
public:
    Shader(GLenum type);
    ~Shader();

    u32 id() const { return id_; }
    bool is_ok() const { return is_ok_; }
    bool compiled() const { return compiled_; }
    GLenum shader_type() const { return shader_type_; }
    const std::string& status() const { return status_; }

    bool Compile(const std::string& source);
};

}
}
