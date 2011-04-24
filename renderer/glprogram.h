#pragma once

#include "renderer/glcontext.h"
#include "renderer/glshader.h"
#include <map>

namespace base {
namespace math {
    class Vector2;
    class Vector3;
    class Vector4;
    class Matrix2;
    class Matrix3;
    class Matrix4;
}
}

namespace ext {
namespace opengl {

    using namespace base::math;

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
    std::string status_;
    bool is_ok_;
public:
    Program();
    ~Program();

    bool is_ok() const { return is_ok_; }
    const std::string& status() const { return status_; }
    
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
    void SetParam(const std::string& name, const Vector2& v);
    void SetParam(const std::string& name, const Vector3& v);
    void SetParam(const std::string& name, const Vector4& v);
    void SetParam(const std::string& name, const Matrix2& m);
    void SetParam(const std::string& name, const Matrix3& m);
    void SetParam(const std::string& name, const Matrix4& m);

    static Program* Create(const std::string& vs, const std::string& fs);
protected:
    void Link();
    void Unlink();
};

}
}
