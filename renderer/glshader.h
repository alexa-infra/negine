#pragma once

namespace ext {
namespace opengl {

class GLShader {
private:
    u32 id_;
    bool is_ok_;
    bool compiled_;
    GLenum shader_type_;
public:
    GLShader(GLenum type);
    ~GLShader();

    u32 id() const { return id_; }
    bool is_ok() const { return is_ok_; }
    bool compiled() const { return compiled_; }
    GLenum shader_type() const { return shader_type_; }

    bool Compile(const std::string& source, std::string& status);
};

}
}
