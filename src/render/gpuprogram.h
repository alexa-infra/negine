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

enum class ShaderType {
    VERTEX = GL_VERTEX_SHADER,
    PIXEL = GL_FRAGMENT_SHADER
};

//! Shader program object
class GpuProgram : public GpuResource, public ResourceBase<GpuProgram>
{
    struct UniformVar
    {
        u32 location;
        u32 type;
        Variant value;
        u32 samplerIdx;
    };
    struct AttrVar
    {
        u32 location;
        VertexAttr attr;
        u32 idx;
    };
    typedef FixedMap<SmallString, UniformVar> UniformMap;
    typedef FixedMap<SmallString, AttrVar> AttrMap;

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
    NEGINE_API GpuProgram(DeviceContext& gl);
    NEGINE_API ~GpuProgram();

    NEGINE_API void destroy();

    NEGINE_API void bind();

    NEGINE_API void unbind();

    NEGINE_API void setAttribute(const std::string& name, VertexAttr attr, u32 idx = 0);

    u32 getAttributeLoc(VertexAttr attr, u32 idx = 0) const;

    NEGINE_API bool setShaderSource(ShaderType type, const std::string& source);

    NEGINE_API bool complete();

    NEGINE_API void setParams(const Params& params);

    NEGINE_API void setParam(const std::string& paramName, const Variant& value);
private:
    
    void setParam(UniformVar& uniform, const Variant& value);

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
