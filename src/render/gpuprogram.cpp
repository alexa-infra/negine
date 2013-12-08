/**
 * \file
 * \author      Alexey Vasilyev <alexa.infra@gmail.com>
 * \copyright   MIT License
 **/
#include "render/gpuprogram.h"
#include "math/vec4.h"
#include "math/matrix-inl.h"
#include "render/texture.h"
#include "render/renderstate.h"
#include <memory>
#include "base/log.h"

namespace base
{
namespace opengl
{

using namespace math;

GpuProgram::GpuProgram(DeviceContext& gl)
    : GpuResource(gl)
    , vertexShader_(gl)
    , pixelShader_(gl)
{
}

void GpuProgram::destroy()
{
    if ( id_ != 0 ) {
        GL.DeleteProgram( id_ );
        id_ = 0;
    }
    pixelShader_.destroy();
    vertexShader_.destroy();
}

const std::string GpuProgram::status() const
{
    if ( id_ == 0 ) {
        return "Gpu program does not exist";
    }

    GLint len;
    GL.GetProgramiv( id_, GL_INFO_LOG_LENGTH, &len );

    if ( len == 0 ) {
        return "";
    }

    std::vector<char> buf(len);
    GL.GetProgramInfoLog( id_, len, NULL, buf.data() );

    return std::string(buf.begin(), buf.end());;
}

GpuProgram::~GpuProgram()
{
    destroy();
}

void GpuProgram::bind()
{
    GL.UseProgram( id_ );
}

void GpuProgram::unbind()
{
    GL.UseProgram(0);
}

void GpuProgram::setParams(const Params& params)
{
    u32 samplerIdx = 0;
    for(UniformMap::Iterator it = uniformBinding_.iterator(); !it.isDone(); it.advance())
    {
        const UniformVar& uniform = it.value();
        /*any value;
        if (!params.tryGet(it.key(), value)) {
            ERR("Uniform '%s' is not supplied", it.name());
            return;
        }*/
        any value;
        setParam(uniform, params.get(it.key(), value), samplerIdx);
    }
}
    
void GpuProgram::setParam(const UniformVar& uniform, const any& value, u32& samplerIdx)
{
    switch(uniform.type) {
        case GL_SAMPLER_2D:
        {
            Texture* texture = any_cast<Texture*>(value);
            GL.renderState().activeTexture.set(samplerIdx);
            texture->bind();
            GL.Uniform1i( uniform.location, samplerIdx );
            samplerIdx++;
            break;
        }
        case GL_FLOAT_MAT4:
        {
            const Matrix4& m = any_cast<const Matrix4&>(value);
            GL.UniformMatrix4fv( uniform.location, 1, GL_FALSE, reinterpret_cast<const f32*>( &m ) );
            break;
        }
        case GL_FLOAT_VEC4:
        {
            const vec4f& v = any_cast<const vec4f&>(value);
            GL.Uniform4f( uniform.location, v.x, v.y, v.z, v.w );
            break;
        }
        case GL_FLOAT:
        {
            f32 v = any_cast<f32>(value);
            GL.Uniform1f ( uniform.location, v );
            break;
        }
        default:
        {
            ERR("Uniform type %#X is not supported", uniform.type);
            break;
        }
    }
}

void GpuProgram::populateUniformMap()
{
    GLint uniformCount = 0;
    GLint maxNameLength = 0;
    GL.GetProgramiv( id_, GL_ACTIVE_UNIFORMS, &uniformCount );
    GL.GetProgramiv( id_, GL_ACTIVE_UNIFORM_MAX_LENGTH, &maxNameLength );

    if ( !uniformCount || !maxNameLength ) {
        return;
    }

    uniformBinding_.clear();

    std::vector<char> buffer(maxNameLength);

    u32 index = 0;
    for ( GLint i = 0; i < uniformCount; ++i ) {
        GLsizei nameLength = 0;
        i32 typeSize = 0;
        GLenum uniformType = 0;
        GL.GetActiveUniform( id_, i, maxNameLength, &nameLength, &typeSize, &uniformType, buffer.data() );
        std::string uniformName( buffer.begin(), buffer.begin()+nameLength );
        u32 location = GL.GetUniformLocation( id_, uniformName.c_str() );
        UniformVar uni;
        uni.location = location;
        uni.type = uniformType;
        uniformBinding_[uniformName.c_str()] = uni;
    }
}

void GpuProgram::setAttribute(const std::string& name, VertexAttr attr, u32 idx)
{
    AttrVar v;
    v.attr = attr;
    v.idx = idx;
    v.location = static_cast<u32>(attributes_.size());
    attributes_[name.c_str()] = v;
}

u32 GpuProgram::getAttributeLoc(VertexAttr attr, u32 idx) const
{
    for (AttrMap::Iterator it = attributes_.iterator(); !it.isDone(); it.advance()) {
        const AttrVar& v = it.value();
        if (v.attr == attr && v.idx == idx)
            return v.location;
    }
    return u32(-1);
}

bool GpuProgram::setShaderSource(ShaderType type, const std::string& source)
{
    ASSERT(id_ == 0);
    const char* sources[1];
    sources[0] = source.c_str();
    if (type == ShaderTypes::VERTEX) {
        vertexShader_.destroy();
        bool res = vertexShader_.create(ShaderTypes::VERTEX, sources, 1);
        WARN(vertexShader_.status().c_str());
        if (!res) ERR("errors during compilation of shader");
        return res;
    } else if (type == ShaderTypes::PIXEL) {
        pixelShader_.destroy();
        bool res = pixelShader_.create(ShaderTypes::PIXEL, sources, 1);
        WARN(pixelShader_.status().c_str());
        if (!res) ERR("errors during compilation of shader");
        return res;
    }
    return false;
}

bool GpuProgram::complete()
{
    ASSERT(id_ == 0);
    ASSERT(vertexShader_.handle() != 0);
    ASSERT(pixelShader_.handle() != 0);

    id_ = GL.CreateProgram( );
    
    GL.AttachShader( id_, vertexShader_.handle() );
    GL.AttachShader( id_, pixelShader_.handle() );
    
    for (AttrMap::Iterator it = attributes_.iterator(); !it.isDone(); it.advance()) {
        const AttrVar& attr = it.value();
        u32 location = attr.location;
        const char* name = it.name();
        GL.BindAttribLocation(id_, location, name);
    }
    
    GL.LinkProgram( id_ );
    
    GLint linkStatus;
    GL.GetProgramiv( id_, GL_LINK_STATUS, &linkStatus );
    bool linked = ( linkStatus == GL_TRUE );
    WARN(status().c_str());

    if (!linked) {
        ERR("error in linkage of shader program");
        return false;
    }
    
    populateUniformMap();
    return true;
}

GpuProgram::Shader::Shader(DeviceContext& gl)
    : GpuResource(gl)
{
}

GpuProgram::Shader::~Shader()
{
    destroy();
}

void GpuProgram::Shader::destroy()
{
    if ( id_ != 0 ) {
        GL.DeleteShader( id_ );
        id_ = 0;
    }
}

bool GpuProgram::Shader::create( ShaderType type, const char* const* source, u32 len )
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

const std::string GpuProgram::Shader::status() const
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
