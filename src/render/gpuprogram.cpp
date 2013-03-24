/**
 * \file
 * \author      Alexey Vasilyev <alexa.infra@gmail.com>
 * \copyright   MIT License
 **/
#include "render/gpuprogram.h"
#include "math/vector.h"
#include "math/matrix-inl.h"
#include "render/texture.h"
#include <memory>
#include "base/stringmap.h"
#include "render/statistics.h"
#include "render/shader.h"
#include "base/log.h"

#include "base/reflect.h"
#include <sstream>
#include <fstream>

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

GpuProgram::GpuProgram(DeviceContext& gl)
    : GpuResource(gl)
{
    vertex_shader_ = new Shader(gl);
    pixel_shader_ = new Shader(gl);
}

bool GpuProgram::is_ok() const {
    return ( id_ != 0 ) && pixel_shader_->is_ok() && vertex_shader_->is_ok();
}

void GpuProgram::Destroy()
{
    if ( is_ok() ) {
        GL.DetachShader( id_, vertex_shader_->handle() );
        GL.DetachShader( id_, pixel_shader_->handle() );
    }

    if ( id_ != 0 ) {
        pixel_shader_->Destroy();
        vertex_shader_->Destroy();

        GL.DeleteProgram( id_ );
        id_ = 0;
    }
}

const std::string GpuProgram::status() const
{
    std::string result;
    if ( id_ == 0 ) {
        return result;
    }

    GLint len;
    GL.GetProgramiv( id_, GL_INFO_LOG_LENGTH, &len );

    if ( len == 0 ) {
        return result;
    }

    result.resize( len );
    char* buf = const_cast<char*>( result.c_str() );
    GL.GetProgramInfoLog( id_, len, NULL, buf );

    return result;
}

GpuProgram::~GpuProgram()
{
    Destroy();
    delete vertex_shader_;
    delete pixel_shader_;
}

void GpuProgram::Bind()
{
    GL.UseProgram( id_ );
    Stats::instance()->inc_program_switches();
}

void GpuProgram::Unbind()
{
    GL.UseProgram(0);
}

template<>
void GpuProgram::set_uniform_param<Texture*>( const UniformVar& uniform, Texture* const& t )
{
    GL.ActiveTexture( GL_TEXTURE0 + uniform.index );
    t->Bind();
    GL.Uniform1i( uniform.location, uniform.index );
}

template<>
void GpuProgram::set_uniform_param<Matrix4>( const UniformVar& uniform, const Matrix4& m )
{
    GL.UniformMatrix4fv( uniform.location, 1, GL_FALSE, reinterpret_cast<const f32*>( &m ) );
}

template<>
void GpuProgram::set_uniform_param<Vector4>( const UniformVar& uniform, const Vector4& v )
{
    GL.Uniform4f( uniform.location, v.x, v.y, v.z, v.w );
}

template<>
void GpuProgram::set_uniform_param<Vector3>( const UniformVar& uniform, const Vector3& v )
{
    GL.Uniform3f( uniform.location, v.x, v.y, v.z );
}

void GpuProgram::get_uniforms_list()
{
    GLint uniform_count = 0;
    GLint max_name_length = 0;
    GL.GetProgramiv( id_, GL_ACTIVE_UNIFORMS, &uniform_count );
    GL.GetProgramiv( id_, GL_ACTIVE_UNIFORM_MAX_LENGTH, &max_name_length );

    if ( !uniform_count || !max_name_length ) {
        return;
    }

    char* buffer = new char[max_name_length];

    u32 index = 0;
    for ( GLint i = 0; i < uniform_count; ++i ) {
        GLsizei name_length = 0;
        i32 type_size = 0;
        GLenum uniform_type = 0;
        GL.GetActiveUniform( id_, i, max_name_length, &name_length, &type_size, &uniform_type, buffer );
        std::string uniformName( buffer );
        u32 location = GL.GetUniformLocation( id_, uniformName.c_str() );
        UniformVar uni;
        uni.location = location;
        switch(uniform_type)
        {
            case GL_SAMPLER_2D:
                uni.index = index++;
                break;
        }
        uni_binding_.emplace(uniformName, uni);
    }

    delete[] buffer;
}
    
bool GpuProgram::createMeta( const std::string& filename )
{
    ASSERT(id_ == 0);
    
    GpuProgramMeta meta;
    iarchive::deserializeFile(filename, meta);
    std::ostringstream ss;
    ss << "#version " << meta.version << '\n';
    for(u32 i=0; i<meta.defines.size(); i++)
        ss << "#define " << meta.defines[i] << '\n';
    for(u32 i=0; i<meta.headers.size(); i++)
        ss << meta.headers[i] << '\n';
    std::string header = ss.str();
    
    const char* source[4];
    source[0] = header.c_str();
    
    std::ifstream file(meta.codePath.c_str());
    if (!file.is_open() || !file.good())
    {
        ERR("in meta file '%s', could not open file '%s'",
            filename.c_str(),
            meta.codePath.c_str());
        return false;
    }
    std::string text((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    source[3] = text.c_str();
    
    std::string includeText;
    if (!meta.include.empty())
    {
        std::ifstream includeFile(meta.include.c_str());
        if (!file.is_open() || !file.good())
        {
            ERR("in meta file '%s', could not open file '%s'",
                filename.c_str(),
                meta.include.c_str());
            return false;
        }
        includeText = std::string((std::istreambuf_iterator<char>(includeFile)), std::istreambuf_iterator<char>());
    }
    source[1] = includeText.c_str();
    
    source[2] = "#define PIXEL_SHADER\n ";
    if (!pixel_shader_->Create(ShaderTypes::PIXEL, source, 4))
    {
        ERR("in meta file '%s', for pixel shader compilation error\n%s",
            filename.c_str(),
            pixel_shader_->status().c_str());
        return false;
    }
    
    source[2] = "#define VERTEX_SHADER\n ";
    if (!vertex_shader_->Create(ShaderTypes::VERTEX, source, 4))
    {
        ERR("in meta file '%s', for vertex shader compilation error\n%s",
            filename.c_str(),
            vertex_shader_->status().c_str());
        return false;
    }
    
    id_ = GL.CreateProgram( );
    
    GL.AttachShader( id_, vertex_shader_->handle() );
    GL.AttachShader( id_, pixel_shader_->handle() );
    
    for (u32 i=0; i<VertexAttrs::Count; i++)
    {
        VertexAttr attr = (VertexAttr)i;
        std::string name;
        attr_map.to_string(attr, name);
        GL.BindAttribLocation(id_,
                             VertexAttrs::GetAttributeLocation(attr),
                             name.c_str());
    }
    
    GL.LinkProgram( id_ );
    
    GLint linkStatus;
    GL.GetProgramiv( id_, GL_LINK_STATUS, &linkStatus );
    bool linked = ( linkStatus == GL_TRUE );
    if (!linked)
    {
        ERR("in meta file '%s', linkage error\n%s",
            filename.c_str(), status().c_str());
        return false;
    }
    
    get_uniforms_list();
    
    return true;
}

}
}
