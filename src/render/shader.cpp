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

Shader::Shader( )
    : id_( 0 )
{
}

Shader::~Shader()
{
    Destroy();
}

void Shader::Destroy()
{
    if ( id_ != 0 ) {
        glDeleteShader( id_ );
        id_ = 0;
    }
}

bool Shader::Create( ShaderType shaderType, const char** source, u32 len )
{
    ASSERT( id_ == 0 );
    shader_type_ = shaderType;
    id_ = glCreateShader( shader_type_ );

    if ( id_ == 0 )
        return false;

    glShaderSource( id_, len, source, NULL );
    glCompileShader( id_ );

    GLint compile_status;
    glGetShaderiv( id_, GL_COMPILE_STATUS, &compile_status );
  
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
    glGetShaderiv( id_, GL_INFO_LOG_LENGTH, &log_size );

    if ( log_size == 0 ) {
        return result;
    }

    result.resize( static_cast<u32>( log_size ) );
    char* buffer = const_cast<char*>( result.c_str() );
    glGetShaderInfoLog( id_, log_size, NULL, buffer );
    return result;
}

}
}
