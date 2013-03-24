/**
 * \file
 * \brief       Wrapper for shader object
 * \author      Alexey Vasilyev <alexa.infra@gmail.com>
 * \copyright   MIT License
 **/
#pragma once

#include "render/gpuresource.h"
#include <string>

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

//! Shader object
class Shader : public GpuResource
{
private:
    GLenum shader_type_;        //!< Type of shader
public:
    Shader(DeviceContext& gl);
    ~Shader();

    //! Compile shader from source
    bool Create( ShaderType shaderType, const char* const* source, u32 len );

    void Destroy();

    //! Gets creation status
    bool is_ok() const {
        return id_ != 0;
    }

    //! Gets shader type
    GLenum shader_type() const {
        return shader_type_;
    }

    //! Gets status string
    const std::string status() const;

private:
    DISALLOW_COPY_AND_ASSIGN( Shader );
};

}
}