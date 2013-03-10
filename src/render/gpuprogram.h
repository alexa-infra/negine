/**
 * \file
 * \brief       Shader program wrapper
 * \author      Alexey Vasilyev <alexa.infra@gmail.com>
 * \copyright   MIT License
 **/
#pragma once

#include "render/glcontext.h"
#include "render/shader.h"
#include <map>
#include <string>
#include "render/mesh.h"
#include "math/matrix.h"

namespace base
{
namespace opengl
{

//! GpuProgram uniform information
namespace UniformVars
{
enum UniformVar {
    Diffuse,
    Projection,
    Modelview,
    CameraPos,
    Bump,
    LightPos,
    Color,
    Lightmap,
    Clip,

    Count
};

u32 get_tex_index( UniformVar var );
}
typedef UniformVars::UniformVar UniformVar;
typedef std::map<UniformVar, u32> UniformBinding;

using base::math::Matrix4;

//! Shader program object
class GpuProgram
{
protected:
    GLuint program_id_;         //!< Name of program object

    Shader pixel_shader_;       //!< Attached pixel shader
    Shader vertex_shader_;      //!< Attached vertex shader

    UniformBinding uni_binding_;
    
public:
    GpuProgram();
    ~GpuProgram();

    void Create();
    void Destroy();

    //! Gets program creation status
    bool is_ok() const {
        return ( program_id_ != 0 ) && pixel_shader_.is_ok() && vertex_shader_.is_ok();
    }
    //! Returns status string
    const std::string status() const;

    //! Sets program to be used at current pipeline
    void Bind();

    //! Unbind program
    void Unbind();

    template<typename T>
    void set_uniform( UniformVar uniform, const T& val ) {
        UniformBinding::iterator it = uni_binding_.find( uniform );

        if ( it == uni_binding_.end() ) {
            return;
        }

        u32 location = it->second;
        set_uniform_param( it->first, location, val );
    }

    //! Creates program from vertex and shader source texts
    //! \param      vs              text of vertex shader
    //! \param      ps              text of pixel shader
    //! \param      status          output status string
    //! \returns    The program instance (deallocation should be handled by caller), null if fails
    bool CreateFromText( const std::string& vs, const std::string& fs, std::string& status );

    //! Creates program by shader file (sources of vertex/pixel shaders are devided by '-- (shader type)' line)
    //! \param      filename        path to shader source file
    //! \param      status          output status string
    //! \returns    The program instance (deallocation should be handled by caller), null if fails
    bool CreateFromFile( const std::string& filename, std::string& status );

    void CreateFromFileWithAssert( const std::string& filename );
protected:
    //! Set uniform helper, that wraps value by generic param
    template<typename T>
    void set_uniform_param( UniformVar uniform, u32 location, const T& val );

    //! Populate list of active uniforms
    void get_uniforms_list();

    //! Populate list of active attributes
    void get_attributes_list();
private:
    DISALLOW_COPY_AND_ASSIGN( GpuProgram );
};



} // namespace opengl
} // namespace base
