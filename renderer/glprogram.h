#pragma once

#include "renderer/glcontext.h"
#include "renderer/glshader.h"
#include <map>

namespace ext {
namespace opengl {

namespace ProgramParamTypes {
    enum ProgramParamType {
        Uniform,
        Attribute
    };
}
typedef ProgramParamTypes::ProgramParamType ProgramParamType;

struct ProgramParam {
    u32 Location;
    u32 TextureUnit;
    ProgramParamType Type;
};
typedef std::map<std::string, ProgramParam> ProgramParamList; 

class Program {
protected:
    GLuint program_id_;
    ProgramParamList param_list_;
    Shader* pixel_shader_;
    Shader* vertex_shader_;
    bool linked_;
public:
    Program();
    ~Program();
    
    void set_pixel_shader(Shader* shader) { 
        if (linked_) Unlink();
        pixel_shader_ = shader; 
    }
    const Shader* pixel_shader() const { 
        return pixel_shader_; 
    }
    void set_vertex_shader(Shader* shader) { 
        if (linked_) Unlink();
        vertex_shader_ = shader; 
    }
    const Shader* vertex_shader() const { 
        return vertex_shader_; 
    }
    const ProgramParamList& param_list() {
        if (!linked_) Link();
        return param_list_;
    }

    void Bind();
    void Unbind();

    void SetParam(const std::string& name, i32 val);
    void SetParam(const std::string& name, f32 val);
    //void SetParam(const std::string& name, const Vector2& v);
    //void SetParam(const std::string& name, const Vector3& v);
    //void SetParam(const std::string& name, const Vector4& v);
    //void SetParam(const std::string& name, const Matrix2& m);
    //void SetParam(const std::string& name, const Matrix3& m);
    //void SetParam(const std::string& name, const Matrix4& m);

protected:
    void Link();
    void Unlink();
};

}
}
