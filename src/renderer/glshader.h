/**
 * \file
 * \brief       Wrapper for shader object
 * \author      Alexey Vasilyev <alexa.infra@gmail.com>
 * \copyright   MIT License
 **/
#pragma once

#include "renderer/glcontext.h"

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
class Shader
{
private:
    u32 id_;                    //!< Name of shader
    bool is_ok_;                //!< Creation status
    bool compiled_;             //!< Compilation status
    GLenum shader_type_;        //!< Type of shader
    std::string status_;        //!< Status string
public:
    Shader( ShaderType shaderType );
    ~Shader();

    //! Gets name of shader
    u32 id() const {
        return id_;
    }

    //! Gets creation status
    bool is_ok() const {
        return is_ok_;
    }

    //! Gets compilation status
    bool compiled() const {
        return compiled_;
    }

    //! Gets shader type
    GLenum shader_type() const {
        return shader_type_;
    }

    //! Gets status string
    const std::string& status() const {
        return status_;
    }

    //! Compile shader from source
    bool Compile( const std::string& source );

private:
    DISALLOW_COPY_AND_ASSIGN( Shader );
};

}
}
