#pragma once

#include "renderer/glcontext.h"
#include "renderer/glshader.h"
#include "base/param.h"
#include <map>
#include <string>

namespace base {
namespace opengl {

class Attribute {
public:
    u32 Location;
    GLenum Type;
};
typedef std::map<std::string, Attribute> AttributeList;

class Uniform {
public:
    u32 Location;
    GLenum Type;
    i32 Index;
};
typedef std::map<std::string, Uniform> UniformList;

class Program {
protected:
    GLuint program_id_;
    
    UniformList uniforms_;
    AttributeList attributes_;

    Shader* pixel_shader_;
    Shader* vertex_shader_;

    bool linked_;
    std::string status_;
    bool is_ok_;

    bool own_pixel_shader_;
    bool own_vertex_shader_;
public:
    Program();
    ~Program();

    bool is_ok() const { return is_ok_; }
    const std::string& status() const { return status_; }
    
    void set_pixel_shader(Shader* shader) { 
        if (linked_) Unlink();
        if (own_pixel_shader_) delete pixel_shader_;
        own_pixel_shader_ = false;
        pixel_shader_ = shader; 
    }
    const Shader* pixel_shader() const { 
        return pixel_shader_; 
    }
    void set_vertex_shader(Shader* shader) { 
        if (linked_) Unlink();
        if (own_vertex_shader_) delete vertex_shader_;
        own_vertex_shader_ = false;
        vertex_shader_ = shader; 
    }
    const Shader* vertex_shader() const { 
        return vertex_shader_; 
    }

    void Bind();
    void Unbind();

    AttributeList& get_attributes() {
        if (!linked_) Link();
        return attributes_;
    }

    void set_uniform(const std::string& name, const param& p);

    static Program* Create(const std::string& vs, const std::string& fs, std::string& status);
protected:
    void Link();
    void Unlink();
    void get_uniforms_list(UniformList& uniforms);
    void get_attributes_list(AttributeList& attributes);

private:
    DISALLOW_COPY_AND_ASSIGN(Program);
};

} // namespace opengl
} // namespace base
