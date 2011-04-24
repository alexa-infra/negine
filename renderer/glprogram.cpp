#include "renderer/glprogram.h"
#include <assert.h>
#include <iostream>
#include "base/math/vector.h"
#include "base/math/matrix.h"

namespace ext {
namespace opengl {

Program::Program()
    : program_id_(0)
    , pixel_shader_(NULL)
    , vertex_shader_(NULL)
    , linked_(false)
    , is_ok_(false) {
    program_id_ = glCreateProgram();
}

Program* Program::Create(const std::string& vs, const std::string& fs) {
    Shader* pvs = new Shader(GL_VERTEX_SHADER);
    Shader* pfs = new Shader(GL_FRAGMENT_SHADER);
    Program* pr = new Program;
    bool failed = false;
    if (!pvs->Compile(vs)) {
        std::cout << pvs->status() << std::endl;
        failed = true;
    } else {
        pr->set_vertex_shader(pvs);
    }
    if (!pfs->Compile(fs)) {
        std::cout << pfs->status() << std::endl;
        failed = true;
    } else {
        pr->set_pixel_shader(pfs);
    }
    pr->Link();
    if (!pr->is_ok()) {
        std::cout << pr->status() << std::endl;
        delete pr;
        delete pfs;
        delete pvs;
        return NULL;
    }
    return pr;
}

Program::~Program() {
    glDeleteProgram(program_id_);
}
    
void Program::Bind() {
    if (!linked_) Link();
    glUseProgram(program_id_);
}

void Program::Unbind() {
    glUseProgram(0);
}

void Program::Link() {
    if (pixel_shader_ != NULL)
        glAttachShader(program_id_, vertex_shader_->id());
    if (vertex_shader_ != NULL)
        glAttachShader(program_id_, pixel_shader_->id());
    glLinkProgram(program_id_);
    linked_ = true;    

    GLint status;
    glGetProgramiv(program_id_, GL_LINK_STATUS, &status);
    if (status != GL_FALSE) {
        is_ok_ = true;
        return;
    }
    
    GLint len;
    glGetProgramiv(program_id_, GL_INFO_LOG_LENGTH, &len);
    status_.resize(len);
    char* buf = const_cast<char*>(status_.c_str());
    glGetProgramInfoLog(program_id_, len, NULL, buf);
    is_ok_ = false;
    return;
}

void Program::Unlink() {
    if (pixel_shader_ != NULL)
        glDetachShader(program_id_, pixel_shader_->id());
    if (vertex_shader_ != NULL)
        glDetachShader(program_id_, vertex_shader_->id());
    param_list_.clear();
    linked_ = false;
}

void Program::SetParam(const std::string& name, const Vector2& v) {
} 
void Program::SetParam(const std::string& name, const Vector3& v) {}
void Program::SetParam(const std::string& name, const Vector4& v) {}
void Program::SetParam(const std::string& name, const Matrix2& m) {}
void Program::SetParam(const std::string& name, const Matrix3& m) {} 
void Program::SetParam(const std::string& name, const Matrix4& m) {} 

}
}
