/**
 * @author  Alexey Vasilyev <alexa.infra@gmail.com>
 * @section LICENSE
 * 
 * This file is part of Negine (https://alexadotlife.com/hg/negine)
 * Licensed under LGPLv3 (or GLPv3) – see LICENSE file for details
 **/
#pragma once

#include "renderer/glcontext.h"
#include "renderer/glshader.h"
#include "base/param.h"
#include <map>
#include <string>

namespace base {
namespace opengl {

//! Program attribute information
class Attribute {
public:
    u32 Location;           //!< Location of attribute
    GLenum Type;            //!< Type of attribute
};
typedef std::map<std::string, Attribute> AttributeList;

//! Program uniform information
class Uniform {
public:
    u32 Location;           //!< Location of uniform
    GLenum Type;            //!< Type of uniform
    i32 Index;              //!< Index of sampler (-1 for other types)
};
typedef std::map<std::string, Uniform> UniformList;

//! Shader program object
class Program {
protected:
    GLuint program_id_;         //!< Name of program object
    
    UniformList uniforms_;      //!< Active uniform objects
    AttributeList attributes_;  //!< Active attribute objects

    Shader* pixel_shader_;      //!< Attached pixel shader
    Shader* vertex_shader_;     //!< Attached vertex shader

    bool linked_;               //!< Program is linked status (shaders are attached, program is linked)
    std::string status_;        //!< Status string, result of linkage
    bool is_ok_;                //!< Status of program creation

    bool own_pixel_shader_;     //!< Pixel shader is created by program, and should be destructed with program
    bool own_vertex_shader_;    //!< Vertex shader is created by program, and should be destructed with program
public:
    Program();
    ~Program();

    //! Gets program creation status
    bool is_ok() const { return is_ok_; }
    //! Returns status string
    const std::string& status() const { return status_; }

    //! Sets pixel shader
    void set_pixel_shader(Shader* shader) { 
        if (linked_) Unlink();
        if (own_pixel_shader_) delete pixel_shader_;
        own_pixel_shader_ = false;
        pixel_shader_ = shader; 
    }

    //! Gets pixel shader
    const Shader* pixel_shader() const { 
        return pixel_shader_; 
    }

    //! Sets vertex shader
    void set_vertex_shader(Shader* shader) { 
        if (linked_) Unlink();
        if (own_vertex_shader_) delete vertex_shader_;
        own_vertex_shader_ = false;
        vertex_shader_ = shader; 
    }

    //! Gets vertex shader
    const Shader* vertex_shader() const { 
        return vertex_shader_; 
    }

    //! Sets program to be used at current pipeline
    void Bind();
    
    //! Unbind program
    void Unbind();

    //! Gets list of active atributes
    AttributeList& get_attributes() {
        if (!linked_) Link();
        return attributes_;
    }

    //! Sets uniform value by name
    //! \param      name            Name of uniform
    //! \param      p               Parametrized value
    void set_uniform(const std::string& name, const param& p);

    //! Creates program from vertex and shader source texts
    //! \param      vs              text of vertex shader
    //! \param      ps              text of pixel shader
    //! \param      status          output status string
    //! \returns    The program instance (deallocation should be handled by caller), null if fails
    static Program* Create(const std::string& vs, const std::string& fs, std::string& status);

    //! Creates program by shader file (sources of vertex/pixel shaders are devided by '-- (shader type)' line)
    //! \param      filename        path to shader source file
    //! \param      status          output status string
    //! \returns    The program instance (deallocation should be handled by caller), null if fails
    static Program* Create(const std::string& filename, std::string& status);
protected:
    //! Attach shaders, and link program. Result is in 'is_ok_' and 'status' members
    void Link();
    
    //! Detach shaders from program
    void Unlink();
    
    //! Populate list of active uniforms
    void get_uniforms_list(UniformList& uniforms);

    //! Populate list of active attributes
    void get_attributes_list(AttributeList& attributes);

private:
    DISALLOW_COPY_AND_ASSIGN(Program);
};

} // namespace opengl
} // namespace base
