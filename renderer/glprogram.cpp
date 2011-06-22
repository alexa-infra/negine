#include "renderer/glprogram.h"
#include <assert.h>
#include <iostream>
#include "base/math/vector.h"
#include "base/math/matrix.h"
#include "renderer/gltexture.h"
#include <memory>

namespace base {
namespace opengl {

Program::Program()
    : program_id_(0)
    , pixel_shader_(NULL)
    , vertex_shader_(NULL)
    , linked_(false)
    , is_ok_(false)
    , own_pixel_shader_(false)
    , own_vertex_shader_(false) {
    program_id_ = glCreateProgram();
}

Program* Program::Create(const std::string& vs, const std::string& fs) {
    std::auto_ptr<Shader> pvs(new Shader(GL_VERTEX_SHADER));
    std::auto_ptr<Shader> pfs(new Shader(GL_FRAGMENT_SHADER));
    std::auto_ptr<Program> pr(new Program);
    bool failed = false;
    if (!pvs->Compile(vs)) {
        std::cout << pvs->status() << std::endl;
        failed = true;
    } else {
        pr->set_vertex_shader(pvs.release());
    }
    if (!pfs->Compile(fs)) {
        std::cout << pfs->status() << std::endl;
        failed = true;
    } else {
        pr->set_pixel_shader(pfs.release());
    }
    if (failed) return NULL;
    pr->Link();
    if (!pr->is_ok()) {
        std::cout << pr->status() << std::endl;
        return NULL;
    }
    pr->own_pixel_shader_ = true;
    pr->own_vertex_shader_ = true;
    return pr.release();
}

Program::~Program() {
    if (own_pixel_shader_) delete pixel_shader_;
    if (own_vertex_shader_) delete vertex_shader_;
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
    if (vertex_shader_ != NULL)
        glAttachShader(program_id_, vertex_shader_->id());
    if (pixel_shader_ != NULL)
        glAttachShader(program_id_, pixel_shader_->id());
    glLinkProgram(program_id_);
    linked_ = true;    

    GLint status;
    glGetProgramiv(program_id_, GL_LINK_STATUS, &status);
    if (status != GL_FALSE) {
        is_ok_ = true;
        get_uniforms_list(uniforms_);
        get_attributes_list(attributes_);
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
    uniforms_.clear();
    attributes_.clear();
    linked_ = false;
}

void Program::set_uniform(const std::string& name, const param& p) {
    UniformList::iterator it = uniforms_.find(name);
    if (it == uniforms_.end()) return;

    Uniform uniform = it->second;
    
    switch (p.get_type()) {
        case Types::Texture:
        {
            Texture* t = p.get<Texture*>();
            t->Bind();
            glUniform1iARB(uniform.Location, uniform.Index);
        }
        break;
        case Types::Matrix4:
        {
            math::Matrix4 const& m = p.get<math::Matrix4>();
            glUniformMatrix4fv(uniform.Location, 1, GL_TRUE, m.array1d);
        }
        default:
        {
        }
        break;
    }    
} 

void Program::get_uniforms_list(UniformList& uniforms) {
    GLint uniform_count, max_name_length = 0;
    glGetObjectParameterivARB(program_id_, GL_OBJECT_ACTIVE_UNIFORMS_ARB, &uniform_count);
    glGetObjectParameterivARB(program_id_, GL_OBJECT_ACTIVE_UNIFORM_MAX_LENGTH_ARB, &max_name_length);

    if (!uniform_count || !max_name_length) return;

    char *buffer = new char[max_name_length];
    i32 textureUnit = 0;

    for (GLint i = 0; i < uniform_count; ++i) 
    {
        GLsizei name_length = 0;
        i32 type_size = 0;
        GLenum uniform_type = 0;
        glGetActiveUniform(program_id_, i, max_name_length, &name_length, &type_size, &uniform_type, buffer);

        std::string uniformName(buffer);
        u32 location = glGetUniformLocation(program_id_, uniformName.c_str());

        Uniform uniform;
        uniform.Location = location;
        uniform.Type = uniform_type;
        if (uniform_type == GL_SAMPLER_2D)
            uniform.Index = textureUnit++;
        else
            uniform.Index = -1;

        uniforms[uniformName] = uniform;
    }

    delete[] buffer;
}

void Program::get_attributes_list(AttributeList& attributes) {
    GLint attrib_count, max_name_len = 0;
    glGetObjectParameterivARB(program_id_, GL_OBJECT_ACTIVE_ATTRIBUTES_ARB, &attrib_count);
    glGetObjectParameterivARB(program_id_, GL_OBJECT_ACTIVE_ATTRIBUTE_MAX_LENGTH_ARB, &max_name_len);

    if (attrib_count == 0 || max_name_len == 0) return;

    char* attrName = new char[max_name_len];
    for (i32 i = 0; i < attrib_count; ++i)
    {
        GLsizei attrNameLen = 0;
        i32 attrSize = 0;
        GLenum attrGLType = 0;
        glGetActiveAttribARB(program_id_, i, max_name_len, &attrNameLen, &attrSize, &attrGLType, attrName);
                
        std::string name(attrName);
        i32 location = glGetAttribLocationARB(program_id_, attrName);
        Attribute attr;
        attr.Location = location;
        attr.Type = attrGLType;
        attributes[name] = attr;
    }

    delete [] attrName;
}


}
}
