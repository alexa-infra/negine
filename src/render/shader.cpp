/**
 * \file
 * \author      Alexey Vasilyev <alexa.infra@gmail.com>
 * \copyright   MIT License
 **/
#include "render/shader.h"
#include "base/debug.h"
#include <vector>

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
    destroy();
}

void Shader::destroy()
{
    if ( id_ != 0 ) {
        GL.DeleteShader( id_ );
        id_ = 0;
    }
}

bool Shader::create( ShaderType type, const char* const* source, u32 len )
{
    if ( id_ == 0 ) {
        id_ = GL.CreateShader( type );
        ASSERT( id_ != 0 );
    }

    GL.ShaderSource( id_, len, source, NULL );
    GL.CompileShader( id_ );

    GLint compileStatus;
    GL.GetShaderiv( id_, GL_COMPILE_STATUS, &compileStatus );
  
    bool isCompiled = ( compileStatus == GL_TRUE );
    return isCompiled;
}

const std::string Shader::status() const
{
    if ( id_ == 0 ) {
        return "Shader does not exist";
    }

    GLint logSize;
    GL.GetShaderiv( id_, GL_INFO_LOG_LENGTH, &logSize );

    if ( logSize == 0 ) {
        return "";
    }

    std::vector<char> buf(logSize);
    GL.GetShaderInfoLog( id_, logSize, NULL, buf.data() );
    return std::string(buf.begin(), buf.end());
}

}
}
