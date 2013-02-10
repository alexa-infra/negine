/**
 * \file
 * \author      Alexey Vasilyev <alexa.infra@gmail.com>
 * \copyright   MIT License
 **/
#include "render/glprogram.h"
#include <iostream>
#include "base/math/vector.h"
#include "base/math/matrix.h"
#include "render/gltexture.h"
#include <memory>
#include "base/stream.h"
#include "base/stringmap.h"
#include "render/statistics.h"

namespace base
{
namespace opengl
{

using namespace math;

StringMap<VertexAttr, VertexAttrs::Count>::Entry attr_map_str[VertexAttrs::Count] = {
    { "position",  VertexAttrs::tagPosition },
    { "tex",       VertexAttrs::tagTexture },
    { "col",       VertexAttrs::tagColor },
    { "n",         VertexAttrs::tagNormal },
    { "t",         VertexAttrs::tagTangent },
    { "bi",        VertexAttrs::tagBinormal }
};
StringMap<VertexAttr, VertexAttrs::Count> attr_map( attr_map_str );

StringMap<UniformVar, UniformVars::Count>::Entry uni_map_str[UniformVars::Count] = {
    { "diffuse",            UniformVars::Diffuse },
    { "projection_matrix",  UniformVars::Projection },
    { "modelview_matrix",   UniformVars::Modelview },
    { "camera_pos",         UniformVars::CameraPos },
    { "bump",               UniformVars::Bump },
    { "light_pos",          UniformVars::LightPos },
    { "color",              UniformVars::Color },
    { "lightmap",           UniformVars::Lightmap },
    { "clip_matrix",        UniformVars::Clip }
};
StringMap<UniformVar, UniformVars::Count> uni_map( uni_map_str );

u32 UniformVars::get_tex_index( UniformVar var )
{
    switch ( var ) {
    case UniformVars::Diffuse:
        return 0;
    case UniformVars::Lightmap:
        return 1;
    case UniformVars::Bump:
        return 2;
    default:
        return 0;
    }
}

Program::Program()
    : program_id_( 0 )
{
}

void Program::Destroy()
{
    if ( is_ok() ) {
        glDetachShader( program_id_, vertex_shader_.id() );
        glDetachShader( program_id_, pixel_shader_.id() );
    }

    if ( program_id_ != 0 ) {
        pixel_shader_.Destroy();
        vertex_shader_.Destroy();

        glDeleteProgram( program_id_ );
        program_id_ = 0;
    }
}

bool Program::CreateFromText( const std::string& vs, const std::string& fs, std::string& statusStr )
{
    ASSERT( program_id_ == 0 );
    program_id_ = glCreateProgram();

    if ( program_id_ == 0 )
        return false;

    bool failed = false;

    if ( !vertex_shader_.Create( ShaderTypes::VERTEX, vs ) ) {
        statusStr = vertex_shader_.status();
        failed = true;
    }

    if ( !pixel_shader_.Create( ShaderTypes::PIXEL, fs ) ) {
        statusStr = statusStr + pixel_shader_.status();
        failed = true;
    }

    if ( failed ) {
        vertex_shader_.Destroy();
        pixel_shader_.Destroy();
        return false;
    }

    glAttachShader( program_id_, vertex_shader_.id() );
    glAttachShader( program_id_, pixel_shader_.id() );

    for (u32 i=0; i<VertexAttrs::Count; i++)
    {
        VertexAttr attr = (VertexAttr)i;
        std::string name;
        attr_map.to_string(attr, name);
        glBindAttribLocation(program_id_, 
            VertexAttrs::GetAttributeLocation(attr), 
            name.c_str());
    }

    glLinkProgram( program_id_ );
    
    GLint linkStatus;
    glGetProgramiv( program_id_, GL_LINK_STATUS, &linkStatus );
    bool linked = ( linkStatus == GL_TRUE );
    
    if ( linked ){
        get_uniforms_list();
        get_attributes_list();
    } else {
        statusStr = this->status();

        glDetachShader( program_id_, vertex_shader_.id() );
        glDetachShader( program_id_, pixel_shader_.id() );
        vertex_shader_.Destroy();
        pixel_shader_.Destroy();
    }

    return linked;
}

const std::string Program::status() const
{
    std::string result;
    if ( program_id_ == 0 ) {
        return result;
    }

    GLint len;
    glGetProgramiv( program_id_, GL_INFO_LOG_LENGTH, &len );

    if ( len == 0 ) {
        return result;
    }

    result.resize( len );
    char* buf = const_cast<char*>( result.c_str() );
    glGetProgramInfoLog( program_id_, len, NULL, buf );

    return result;
}

bool Program::CreateFromFile( const std::string& filename, std::string& status )
{
    FileText file( filename );
    std::vector<std::string> lines = file.read_lines();

    if ( lines.size() == 0 ) {
        return false;
    }

    u32 vertexBegin = 0;
    u32 pixelBegin = 0;

    for ( u32 i = 0; i < lines.size(); i++ ) {
        if ( lines[i] == "-- vertex" ) {
            vertexBegin = i + 1;
        }

        if ( lines[i] == "-- pixel" ) {
            pixelBegin = i + 1;
        }
    }

    if ( vertexBegin == pixelBegin ) {
        return false;
    }

    u32 vertexEnd = 0;
    u32 pixelEnd = 0;

    if ( vertexBegin > pixelBegin ) {
        vertexEnd = lines.size();
        pixelEnd = vertexBegin - 1;
    } else {
        vertexEnd = pixelBegin - 1;
        pixelEnd = lines.size();
    }

    std::string vertexSrc;

    for ( u32 i = vertexBegin; i < vertexEnd; i++ ) {
        vertexSrc = vertexSrc + lines[i] + "\n";
    }

    std::string pixelSrc;

    for ( u32 i = pixelBegin; i < pixelEnd; i++ ) {
        pixelSrc = pixelSrc + lines[i] + "\n";
    }

    return CreateFromText( vertexSrc, pixelSrc, status );
}

Program::~Program()
{
    Destroy();
}

void Program::Bind()
{
    glUseProgram( program_id_ );
    Stats::instance()->inc_program_switches();
}

void Program::Unbind()
{
    glUseProgram(0);
}

template<>
void Program::set_uniform_param<Texture*>( UniformVar uniform, u32 location, Texture* const& t )
{
    u32 sampler_index = get_tex_index( uniform );
    glActiveTexture( GL_TEXTURE0 + sampler_index );
    t->Bind();
    glUniform1i( location, sampler_index );
}

template<>
void Program::set_uniform_param<Matrix4>( UniformVar uniform, u32 location, const Matrix4& m )
{
    glUniformMatrix4fv( location, 1, GL_FALSE, reinterpret_cast<const f32*>( &m ) );
}

template<>
void Program::set_uniform_param<Vector4>( UniformVar uniform, u32 location, const Vector4& v )
{
    glUniform4f( location, v.x, v.y, v.z, v.w );
}

template<>
void Program::set_uniform_param<Vector3>( UniformVar uniform, u32 location, const Vector3& v )
{
    glUniform3f( location, v.x, v.y, v.z );
}

void Program::get_uniforms_list()
{
    GLint uniform_count, max_name_length = 0;
    glGetProgramiv( program_id_, GL_ACTIVE_UNIFORMS, &uniform_count );
    glGetProgramiv( program_id_, GL_ACTIVE_UNIFORM_MAX_LENGTH, &max_name_length );

    if ( !uniform_count || !max_name_length ) {
        return;
    }

    char* buffer = new char[max_name_length];

    for ( GLint i = 0; i < uniform_count; ++i ) {
        GLsizei name_length = 0;
        i32 type_size = 0;
        GLenum uniform_type = 0;
        glGetActiveUniform( program_id_, i, max_name_length, &name_length, &type_size, &uniform_type, buffer );
        std::string uniformName( buffer );
        u32 location = glGetUniformLocation( program_id_, uniformName.c_str() );
        UniformVar uni;

        if ( !uni_map.from_string( uniformName, uni ) ) {
            ERR("Uniform not found: '%s' (%d)", uniformName.c_str(), uniform_type);
            continue;
        }

        uni_binding_[uni] = location;
    }

    delete[] buffer;
}

void Program::get_attributes_list()
{
}

void Program::CreateFromFileWithAssert( const std::string& filename )
{
    std::string status;

    if ( !base::file_exists( filename ) ) {
        ERR("File not found %s", filename.c_str());
        abort();
    }

    if ( !CreateFromFile( filename, status ) ) {
        ERR("Error on load shader: %s @ %s", status.c_str(), filename.c_str());
        abort();
    }
}

}
}
