/**
 * \file
 * \brief       Shader program wrapper
 * \author      Alexey Vasilyev <alexa.infra@gmail.com>
 * \copyright   MIT License
 **/
#pragma once

#include "render/gpuresource.h"
#include "render/mesh.h"
#include "base/parameter.h"

namespace base
{
namespace opengl
{

struct UniformVar
{
    std::string name;
    u32 location;
    u32 type;
};
typedef std::vector<UniformVar> UniformBinding;

class Shader;

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
class GpuProgram : public GpuResource
{
protected:    
    Shader* pixel_shader_;       //!< Attached pixel shader
    Shader* vertex_shader_;      //!< Attached vertex shader

    UniformBinding uni_binding_;
    
public:
    GpuProgram(DeviceContext& gl);
    ~GpuProgram();

    void Destroy();

    //! Gets program creation status
    bool is_ok() const;

    //! Returns status string
    const std::string status() const;

    //! Sets program to be used at current pipeline
    void Bind();

    //! Unbind program
    void Unbind();

    void setParams(const ParameterMap& params);

    bool createMeta( const std::string& filename );
    
    void create( const std::string& filename )
    {
        if (!createMeta(filename))
            Destroy();
    }

protected:
    void setParam(const UniformVar& uniform, const any& value, u32& samplerIdx);

    //! Populate list of active uniforms
    void get_uniforms_list();
private:
    DISALLOW_COPY_AND_ASSIGN( GpuProgram );
};



} // namespace opengl
} // namespace base
