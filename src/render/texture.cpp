/**
 * \file
 * \author      Alexey Vasilyev <alexa.infra@gmail.com>
 * \copyright   MIT License
 **/
#include "render/texture.h"

#include <string>
#include "base/log.h"
#include "base/debug.h"

namespace base {
namespace opengl {

template<>
ResourceType BaseResource<Texture>::type_ = ResourceManager::registerResource();

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

GLenum InternalTypes::toDataType(InternalType value) {
    switch (value) {
        case D16:       return GL_UNSIGNED_SHORT;
        case D24:       return GL_UNSIGNED_INT;
        case D32:       return GL_UNSIGNED_INT;
        case D32F:      return GL_FLOAT;
        case D24S8:     return GL_UNSIGNED_INT_24_8;
        case RGBA8:     return GL_UNSIGNED_BYTE;
        case RGBA16F:   return GL_HALF_FLOAT;

        default:        return GL_UNSIGNED_BYTE;
    }
}
bool InternalTypes::isColor(InternalType value) {
    return value == RGB8 || value == RGBA8 || value == RGBA16F;
}
bool InternalTypes::isStencil(InternalType value) {
    return value == S1 || value == S4 || value == S8 || value == S16;
}
bool InternalTypes::isDepth(InternalType value) {
    return value == D16 || value == D24 || value == D32 || value == D32F;	
}
bool InternalTypes::isDepthStencil(InternalType value) {
    return value == D24S8 || value == D32FS8;	
}

u32 InternalTypes::sizeInBytes(InternalType value) {
    switch (value) {
        case D16:       return 2;
        case D24:       return 4;
        case D32:       return 4;
        case D32F:      return 4;
        case D24S8:     return 4;
        case RGBA8:     return 4;
        case RGBA16F:   return 8;

        default:        return 4;
    }
}

TextureInfo::TextureInfo()
    : Type( TextureTypes::Texture2D )
    , Filtering( TextureFilters::Linear )
    , Wrap( TextureWraps::REPEAT )
    , GenerateMipmap( false )
    , Pixel( PixelTypes::RGBA )
    , InternalType( InternalTypes::RGBA8 )
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

    if( id_ == 0 )
        GL.GenTextures( 1, &id_ );

    GL.setTexture( this );
    setup();

    GL.TexImage2D(
        info_.Type,
        0,
        info_.InternalType,
        info_.Width,
        info_.Height,
        0,
        info_.Pixel,
        InternalTypes::toDataType(info_.InternalType),
        data );

    if (info_.GenerateMipmap) {
        if ( info_.Width == info_.Height )
            GL.GenerateMipmap( info_.Type );
    }
    GL_ASSERT(GL);
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
