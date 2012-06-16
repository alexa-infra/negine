/**
 * \file
 * \author      Alexey Vasilyev <alexa.infra@gmail.com>
 * \copyright   MIT License
 **/
#include "renderer/glprogram.h"
#include <assert.h>
#include <iostream>
#include "base/math/vector.h"
#include "base/math/matrix.h"
#include "renderer/gltexture.h"
#include <memory>
#include "base/stream.h"
#include "base/stringmap.h"

namespace base {
namespace opengl {

StringMap<VertexAttr, VertexAttrs::Count>::Entry attr_map_str[VertexAttrs::Count] = {
    { "position",  VertexAttrs::tagPosition },
    { "tex",       VertexAttrs::tagTexture },
    { "col",       VertexAttrs::tagColor },
    { "n",         VertexAttrs::tagNormal },
    { "t",         VertexAttrs::tagTangent },
    { "bi",        VertexAttrs::tagBinormal }
};
StringMap<VertexAttr, VertexAttrs::Count> attr_map(attr_map_str);

StringMap<UniformVar, UniformVars::Count>::Entry uni_map_str[UniformVars::Count] = {
    { "diffuse",            UniformVars::Diffuse },
    { "projection_matrix",  UniformVars::Projection },
    { "modelview_matrix",   UniformVars::Modelview },
    { "camera_pos",         UniformVars::CameraPos },
    { "bump",               UniformVars::Bump },
    { "light_pos",          UniformVars::LightPos },
    { "color",              UniformVars::Color }
};
StringMap<UniformVar, UniformVars::Count> uni_map(uni_map_str);

u32 UniformVars::get_tex_index(UniformVar var) {
    switch (var) {
        case UniformVars::Diffuse: return 0;
        case UniformVars::Bump: return 1;
        default:
            return 0;
    }
}

Program::Program()
    : program_id_(0)
    , pixel_shader_(NULL)
    , vertex_shader_(NULL)
    , is_ok_(false) {
    program_id_ = glCreateProgram();
}

Program* Program::Create(const std::string& vs, const std::string& fs, std::string& status) {
    std::auto_ptr<Shader> pvs(new Shader(ShaderTypes::VERTEX));
    std::auto_ptr<Shader> pfs(new Shader(ShaderTypes::PIXEL));
    std::auto_ptr<Program> pr(new Program);
    bool failed = false;
    if (!pvs->Compile(vs)) {
        status = pvs->status();
        failed = true;
    } else {
        pr->vertex_shader_ = pvs.release();
    }
    if (!pfs->Compile(fs)) {
        if (!failed)
            status = pfs->status();
        else
            status = status + pfs->status();
        failed = true;
    } else {
        pr->pixel_shader_ = pfs.release();
    }
    if (failed) return NULL;
    pr->Link();
    if (!pr->is_ok()) {
        status = pr->status();
        return NULL;
    }
    return pr.release();
}

Program* Program::Create(const std::string& filename, std::string& status) {
    FileText file(filename);
    std::vector<std::string> lines = file.read_lines();
    if (lines.size() == 0)
        return NULL;

    u32 vertexBegin = 0;
    u32 pixelBegin = 0;
    for (u32 i=0; i<lines.size(); i++) {
        if (lines[i] == "-- vertex")
            vertexBegin = i + 1;
        if (lines[i] == "-- pixel")
            pixelBegin = i + 1;
    }
    if (vertexBegin == pixelBegin)
        return NULL;

    u32 vertexEnd = 0;
    u32 pixelEnd = 0;
    if (vertexBegin > pixelBegin) {
        vertexEnd = lines.size();
        pixelEnd = vertexBegin - 1;
    } else {
        vertexEnd = pixelBegin - 1;
        pixelEnd = lines.size();
    }
    std::string vertexSrc;
    for (u32 i=vertexBegin; i<vertexEnd; i++)
        vertexSrc = vertexSrc + lines[i] + "\n";
    std::string pixelSrc;
    for (u32 i=pixelBegin; i<pixelEnd; i++)
        pixelSrc = pixelSrc + lines[i] + "\n";
    return Create(vertexSrc, pixelSrc, status);
}

Program::~Program() {
    delete pixel_shader_;
    delete vertex_shader_;
    glDeleteProgram(program_id_);
}
    
void Program::Bind() {
    glUseProgram(program_id_);
    AttributeBinding::const_iterator it;
    for(it = attr_binding_.begin(); it != attr_binding_.end(); ++it) {
        glEnableVertexAttribArray(it->second);
    }
}

void Program::Unbind() {
    AttributeBinding::const_iterator it;
    for(it = attr_binding_.begin(); it != attr_binding_.end(); ++it) {
        glDisableVertexAttribArray(it->second);
    }
//    glUseProgram(0);
}

void Program::Link() {
    if (vertex_shader_ != NULL)
        glAttachShader(program_id_, vertex_shader_->id());
    if (pixel_shader_ != NULL)
        glAttachShader(program_id_, pixel_shader_->id());
    glLinkProgram(program_id_);

    GLint status;
    glGetProgramiv(program_id_, GL_LINK_STATUS, &status);
    if (status != GL_FALSE) {
        is_ok_ = true;
        get_uniforms_list();
        get_attributes_list();
        return;
    }
    
    GLint len;
    glGetProgramiv(program_id_, GL_INFO_LOG_LENGTH, &len);
    status_.resize(len);
    char* buf = const_cast<char*>(status_.c_str());
    glGetProgramInfoLog(program_id_, len, NULL, buf);
    is_ok_ = false;
}

void Program::Unlink() {
    if (pixel_shader_ != NULL)
        glDetachShader(program_id_, pixel_shader_->id());
    if (vertex_shader_ != NULL)
        glDetachShader(program_id_, vertex_shader_->id());
    uni_binding_.clear();
    attr_binding_.clear();
}

template<>
void Program::set_uniform_param<Texture*>(UniformVar uniform, u32 location, Texture* const& t) {
    u32 sampler_index = get_tex_index(uniform);
    glActiveTexture(GL_TEXTURE0 + sampler_index);
    t->Bind();
    glUniform1i(location, sampler_index);
}

template<>
void Program::set_uniform_param<Matrix4>(UniformVar uniform, u32 location, const Matrix4& m) {
    glUniformMatrix4fv(location, 1, GL_TRUE, m.array1d);
}

template<>
void Program::set_uniform_param<Vector4>(UniformVar uniform, u32 location, const Vector4& v) {
    glUniform4f(location, v.x, v.y, v.z, v.w);
}

template<>
void Program::set_uniform_param<Vector3>(UniformVar uniform, u32 location, const Vector3& v) {
    glUniform3f(location, v.x, v.y, v.z);
}

void Program::get_uniforms_list() {
    GLint uniform_count, max_name_length = 0;
    glGetProgramiv(program_id_, GL_ACTIVE_UNIFORMS, &uniform_count);
    glGetProgramiv(program_id_, GL_ACTIVE_UNIFORM_MAX_LENGTH, &max_name_length);

    if (!uniform_count || !max_name_length) return;

    char *buffer = new char[max_name_length];

    for (GLint i = 0; i < uniform_count; ++i) 
    {
        GLsizei name_length = 0;
        i32 type_size = 0;
        GLenum uniform_type = 0;
        glGetActiveUniform(program_id_, i, max_name_length, &name_length, &type_size, &uniform_type, buffer);

        std::string uniformName(buffer);
        u32 location = glGetUniformLocation(program_id_, uniformName.c_str());

        UniformVar uni;
        if (!uni_map.from_string(uniformName, uni)) {
            std::cout << "uniform not found. name: " << uniformName 
                << ", type: " << uniform_type << std::endl;
            continue;
        }
        uni_binding_[uni] = location;
    }

    delete[] buffer;
}

void Program::get_attributes_list() {
    GLint attrib_count, max_name_len = 0;
    glGetProgramiv(program_id_, GL_ACTIVE_ATTRIBUTES, &attrib_count);
    glGetProgramiv(program_id_, GL_ACTIVE_ATTRIBUTE_MAX_LENGTH, &max_name_len);

    if (attrib_count == 0 || max_name_len == 0) return;

    char* attrName = new char[max_name_len];
    for (i32 i = 0; i < attrib_count; ++i)
    {
        GLsizei attrNameLen = 0;
        i32 attrSize = 0;
        GLenum attrGLType = 0;
        glGetActiveAttrib(program_id_, i, max_name_len, &attrNameLen, &attrSize, &attrGLType, attrName);
                
        std::string name(attrName);
        i32 location = glGetAttribLocation(program_id_, attrName);

        VertexAttr va;
        if (!attr_map.from_string(name, va)) {
            std::cout << "vertex attr not found. name: " << name 
                << ", type: " << attrGLType << std::endl;
            continue;
        }
        attr_binding_.insert(std::make_pair(va, location));
    }

    delete [] attrName;
}

Program* LoadProgram(const std::string& filename) {
    std::string status;
    if (!base::file_exists(filename)) {
        std::cout 
            << "File is not exists: "
            << filename << '\n'
            << std::endl;
        assert(false);
        return NULL;
    }
    Program* program = Program::Create(filename, status);
    if (program == NULL) {
        std::cout
            << "Error on load shader program "
            << filename << std::endl
            << status << std::endl;
        assert(false);
    }
    return program;
}

}
}
