/**
 * \file
 * \brief       Shader program wrapper
 * \author      Alexey Vasilyev <alexa.infra@gmail.com>
 * \copyright   MIT License
 **/
#pragma once

#include "render/glcontext.h"
#include "render/shader.h"
#include <unordered_map>
#include <string>
#include "render/mesh.h"
#include "math/matrix.h"

namespace base
{
namespace opengl
{

struct UniformVar
{
    u32 location;
    u32 index;
};
typedef std::unordered_map<std::string, UniformVar> UniformBinding;

struct GpuProgramMeta
{
    std::string version;
    std::string codePath;
    std::string include;
    std::vector<std::string> defines;
    std::vector<std::string> headers;
    
    template<typename archive>
    void reflect(archive& ar)
    {
        ar.reflect("version", version, std::string("150"));
        ar.reflect("code", codePath);
        ar.reflect("include", include, std::string(""));
        ar.vreflect("define", defines);
        ar.vreflect("header", headers);
    }
};
    
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
    void set_uniform( const std::string& name, const T& val ) {
        UniformBinding::const_iterator it = uni_binding_.find( name );

        if ( it == uni_binding_.end() ) {
            ERR("uniform '%s' is not found in program", name.c_str());
            return;
        }

        set_uniform_param( it->second, val );
    }

    bool createMeta( const std::string& filename );
    
    void create( const std::string& filename )
    {
        if (!createMeta(filename))
            Destroy();
    }
    
    //! Creates program from vertex and shader source texts
    //! \param      vs              text of vertex shader
    //! \param      ps              text of pixel shader
    //! \param      status          output status string
    //! \returns    The program instance (deallocation should be handled by caller), null if fails
    bool CreateFromText( const std::string& vs, const std::string& fs, std::string& status );

protected:
    //! Set uniform helper, that wraps value by generic param
    template<typename T>
    void set_uniform_param( const UniformVar& uniform, const T& val );

    //! Populate list of active uniforms
    void get_uniforms_list();
private:
    DISALLOW_COPY_AND_ASSIGN( GpuProgram );
};



} // namespace opengl
} // namespace base
