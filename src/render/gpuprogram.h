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

namespace ShaderTypes
{
enum ShaderType {
    VERTEX = GL_VERTEX_SHADER,
    PIXEL = GL_FRAGMENT_SHADER
};
}
typedef ShaderTypes::ShaderType ShaderType;

//! Shader program object
class GpuProgram : public GpuResource
{
    struct UniformVar
    {
        u32 location;
        u32 type;
    };
    typedef FixedMap<SmallString, UniformVar> UniformMap;
    typedef FixedMap<SmallString, VertexAttr> AttrMap;

    //! Shader object
    class Shader : public GpuResource
    {
    public:
        Shader(DeviceContext& gl);
        ~Shader();

        //! Compile shader from source
        bool create( ShaderType type, const char* const* source, u32 len );

        void destroy();

        //! Gets status string
        const std::string status() const;

    private:
        DISALLOW_COPY_AND_ASSIGN( Shader );
    };

public:
    GpuProgram(DeviceContext& gl);
    ~GpuProgram();

    void destroy();

    //! Sets program to be used at current pipeline
    void bind();

    //! Unbind program
    void unbind();

    void setAttribute(const std::string& name, VertexAttr attr);

    bool setShaderSource(ShaderType type, const std::string& source);

    bool complete();

    void setParams(const Params& params);
private:
    
    void setParam(const UniformVar& uniform, const any& value, u32& samplerIdx);

    //! Populate list of active uniforms
    void populateUniformMap();

    //! Returns status string
    const std::string status() const;
private:
    Shader pixelShader_;       //!< Attached pixel shader
    Shader vertexShader_;      //!< Attached vertex shader

    UniformMap uniformBinding_;
    AttrMap attributes_;
    Params uniformCache_;
private:
    DISALLOW_COPY_AND_ASSIGN( GpuProgram );
};

} // namespace opengl
} // namespace base
