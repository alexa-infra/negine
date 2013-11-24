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

}
}
