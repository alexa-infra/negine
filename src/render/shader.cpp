/**
 * \file
 * \author      Alexey Vasilyev <alexa.infra@gmail.com>
 * \copyright   MIT License
 **/
#include "render/shader.h"
#include "base/debug.h"

namespace base
{
namespace opengl
{

Shader::Shader(DeviceContext& gl)
    : GpuResource(gl)
{
}

Shader::~Shader()
{
    Destroy();
}

void Shader::Destroy()
{
    if ( id_ != 0 ) {
        GL.DeleteShader( id_ );
        id_ = 0;
    }
}

bool Shader::Create( ShaderType shaderType, const char* const* source, u32 len )
{
    ASSERT( id_ == 0 );
    shader_type_ = shaderType;
    id_ = GL.CreateShader( shader_type_ );

    if ( id_ == 0 )
        return false;

    GL.ShaderSource( id_, len, source, NULL );
    GL.CompileShader( id_ );

    GLint compile_status;
    GL.GetShaderiv( id_, GL_COMPILE_STATUS, &compile_status );
  
    bool isCompiled = ( compile_status == GL_TRUE );
    return isCompiled;
}

const std::string Shader::status() const
{
    std::string result;

    if ( id_ == 0 ) {
        return result;
    }

    GLint log_size;
    GL.GetShaderiv( id_, GL_INFO_LOG_LENGTH, &log_size );

    if ( log_size == 0 ) {
        return result;
    }

    result.resize( static_cast<u32>( log_size ) );
    char* buffer = const_cast<char*>( result.c_str() );
    GL.GetShaderInfoLog( id_, log_size, NULL, buffer );
    return result;
}

}
}
