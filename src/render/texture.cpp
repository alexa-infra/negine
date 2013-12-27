/**
 * \file
 * \author      Alexey Vasilyev <alexa.infra@gmail.com>
 * \copyright   MIT License
 **/
#include "render/texture.h"

#include <string>
#include "base/log.h"
#include "base/debug.h"

namespace base
{

ResourceType opengl::Texture::type_ = ResourceManager::registerResource();

namespace opengl
{

u32 GLDataTypes::sizeInBytes(GLDataType dt)
{
    switch(dt)
    {
    case GLDataTypes::Byte:
    case GLDataTypes::UByte:    return 1;
    case GLDataTypes::Short:
    case GLDataTypes::UShort:   return 2;
    case GLDataTypes::Int:
    case GLDataTypes::UInt:
    case GLDataTypes::Float:    return 4;
    case GLDataTypes::Double:   return 8;
    default: return 0;
    }
}

u32 PixelTypes::componentCount(PixelType type)
{
    switch(type)
    {
    case PixelTypes::R:         return 1;
    case PixelTypes::RG:        return 2;
    case PixelTypes::RGB:       return 3;
    case PixelTypes::RGBA:      return 4;
    case PixelTypes::Depth:     return 1;
    default: return 0;
    }
}

TextureInfo::TextureInfo()
    : Type( TextureTypes::Texture2D )
    , Filtering( TextureFilters::Linear )
    , Wrap( TextureWraps::REPEAT )
    , GenerateMipmap( false )
    , Pixel( PixelTypes::RGBA )
    , DataType( GLDataTypes::UByte )
    , InternalType( GL_RGBA )
{
}

Texture::Texture(DeviceContext& gl)
    : GpuResource(gl)
{
}

Texture::~Texture()
{
    destroy();
}

void Texture::destroy()
{
    if ( id_ != 0 ) {
        GL.DeleteTextures( 1, &id_ );
        id_ = 0;
    }
}

void Texture::bind()
{
    GL.BindTexture( info_.Type, id_ );
}

void Texture::unbind()
{
    GL.BindTexture( info_.Type, 0 );
}

void Texture::createFromBuffer( const TextureInfo& textureinfo, const u8* data )
{
    info_ = textureinfo;

    ASSERT( id_ == 0 );
    GL.GenTextures( 1, &id_ );

    GL.BindTexture( info_.Type, id_ );
    setup();

    GL.TexImage2D(
        info_.Type,
        0,
        info_.InternalType,
        info_.Width,
        info_.Height,
        0,
        info_.Pixel,
        info_.DataType,
        data );

    if (info_.GenerateMipmap) {
        if ( info_.Width == info_.Height )
            GL.GenerateMipmap( info_.Type );
    }

    GL.BindTexture( info_.Type, 0 );
}

void Texture::createEmpty( const TextureInfo& textureinfo )
{
    createFromBuffer(textureinfo, nullptr);
}

void Texture::setup()
{
    GLenum minFilter;
    GLenum magFilter;
    switch (info_.Filtering) {
        case TextureFilters::Linear:
        {
            if (info_.GenerateMipmap)
                minFilter = GL_LINEAR_MIPMAP_LINEAR;
            else
                minFilter = GL_LINEAR;
            magFilter = GL_LINEAR;
            break;
        }
        case TextureFilters::Nearest:
        {
            if (info_.GenerateMipmap)
                minFilter = GL_NEAREST_MIPMAP_LINEAR;
            else
                minFilter = GL_NEAREST;
            magFilter = GL_NEAREST;
            break;
        }
        case TextureFilters::Anisotropic:
        {
            if (info_.GenerateMipmap)
                minFilter = GL_LINEAR_MIPMAP_LINEAR;
            else
                minFilter = GL_LINEAR;
            magFilter = GL_LINEAR;
            GL.TexParameterf(info_.Type, GL_TEXTURE_MAX_ANISOTROPY_EXT, 8.0f);
            break;
        }
    }

    GL.TexParameteri( info_.Type, GL_TEXTURE_MIN_FILTER, minFilter );
    GL.TexParameteri( info_.Type, GL_TEXTURE_MAG_FILTER, magFilter );
    GL.TexParameteri( info_.Type, GL_TEXTURE_WRAP_S, info_.Wrap );
    GL.TexParameteri( info_.Type, GL_TEXTURE_WRAP_T, info_.Wrap );
}

}
}
