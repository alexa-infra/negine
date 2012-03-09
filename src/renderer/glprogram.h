/**
 * @author  Alexey Vasilyev <alexa.infra@gmail.com>
 * @section LICENSE
 * 
 * This file is part of Negine (https://hg.alexadotlife.com/negine)
 * Licensed under LGPLv3 (or GPLv3) - see LICENSE file for details
 **/
#pragma once

#include "renderer/glcontext.h"
#include "renderer/glshader.h"
#include "base/param.h"
#include <map>
#include <string>
#include "renderer/mesh.h"

namespace base {
namespace opengl {

//! Program uniform information
namespace UniformVars {
    enum UniformVar {
        Diffuse,
        Projection,
        Modelview,

        Count
    };

    u32 get_tex_index(UniformVar var);
}
typedef UniformVars::UniformVar UniformVar;
typedef std::map<UniformVar, u32> UniformBinding;

//! Shader program object
class Program {
protected:
    GLuint program_id_;         //!< Name of program object
    
    Shader* pixel_shader_;      //!< Attached pixel shader
    Shader* vertex_shader_;     //!< Attached vertex shader

    std::string status_;        //!< Status string, result of linkage
    bool is_ok_;                //!< Status of program creation

    AttributeBinding attr_binding_;  //!< attributes to vertex data binding
    UniformBinding uni_binding_;
    Program();
public:
    ~Program();

    //! Gets program creation status
    bool is_ok() const { return is_ok_; }
    //! Returns status string
    const std::string& status() const { return status_; }

    //! Sets program to be used at current pipeline
    void Bind();
    
    //! Unbind program
    void Unbind();

    //! Gets binding of attributes to vertex tags
    const AttributeBinding& binding() {
        return attr_binding_;
    }

    //! Set uniform helper, that wraps value by generic param
    template<typename T>
    void set_uniform(UniformVar uniform, const T& val) {
        base::generic_param<T> param(val);
        set_uniform_param(uniform, param);
    }

    //! Sets uniform value by name
    //! \param      uniform         Variable name
    //! \param      p               Parametrized value
    void set_uniform_param(UniformVar uniform, const param& p);

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
    void get_uniforms_list();

    //! Populate list of active attributes
    void get_attributes_list();
private:
    DISALLOW_COPY_AND_ASSIGN(Program);
};

} // namespace opengl
} // namespace base
