#include "renderer/glprogram.h"
#include <assert.h>

namespace ext {
namespace opengl {

Program::Program()
    : program_id_(0)
    , pixel_shader_(NULL)
    , vertex_shader_(NULL)
    , linked_(false) {
    program_id_ = glCreateProgram();
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
    if (status != GL_FALSE)
        return;
    
    GLint len;
    glGetProgramiv(program_id_, GL_INFO_LOG_LENGTH, &len);
    char* buf = new char[len];
    glGetProgramInfoLog(program_id_, len, NULL, buf);
    assert(false);
    delete[] buf;
}

void Program::Unlink() {
    if (pixel_shader_ != NULL)
        glDetachShader(program_id_, pixel_shader_->id());
    if (vertex_shader_ != NULL)
        glDetachShader(program_id_, vertex_shader_->id());
    param_list_.clear();
    linked_ = false;
}

}
}
