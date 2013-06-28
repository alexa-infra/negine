/**
 * \file
 * \author      Alexey Vasilyev <alexa.infra@gmail.com>
 * \copyright   MIT License
 **/
#include "render/texture.h"

#define STBI_FAILURE_USERMSG
#include "stb/stb_image.c"

#include <iostream>
#include "render/statistics.h"
#include <string>
#include "base/log.h"
#include "base/debug.h"
#include "base/env.h"

namespace base
{
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
    Stats::inc_texture_switches();
}

void Texture::createFromFile( const TextureInfo& textureinfo )
{
    info_ = textureinfo;
    u8* image = stbi_load( info_.Filename.c_str(), &info_.Width, &info_.Height, &info_.ComponentCount, 0 );

    if( image == NULL ) {
        ERR("Failed load image: %s", stbi_failure_reason());
        return;
    }

    // stbi - only 8bit formats are supported
    info_.DataType = GLDataTypes::UByte;

    switch( info_.ComponentCount ) {
    case 1:
        info_.Pixel = PixelTypes::R;
        break;
    case 2:
        info_.Pixel = PixelTypes::RG;
        break;
    case 3:
        info_.Pixel = PixelTypes::RGB;
        break;
    case 4:
        info_.Pixel = PixelTypes::RGBA;
        break;
    }

    createFromBuffer( info_, image );
    stbi_image_free( image );
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

TextureLoader::TextureLoader(DeviceContext& gl)
    : context_(gl)
{
}

TextureLoader::~TextureLoader()
{
    clearCache();
}

void TextureLoader::clearCache()
{
    for( TextureCache::iterator it = cache_.begin(); it != cache_.end(); ++it ) {
        delete it->second;
    }

    cache_.clear();
}

Texture* TextureLoader::load( const std::string& filename )
{
    TextureCache::iterator found = cache_.find( filename );
    if ( found != cache_.end() ) {
        return found->second;
    }

    std::string name;

    if ( fileExists( filename ) ) {
        name = filename;
    } else if ( fileExists( filename + ".jpg" ) ) {
        name = filename + ".jpg";
    } else if ( fileExists( filename + ".tga" ) ) {
        name = filename + ".tga";
    } else if ( fileExists( filename + ".png" ) ) {
        name = filename + ".png";
    } else {
        WARN("%s not found", filename.c_str());
        cache_[filename] = NULL;
        return NULL;
    }

    LOG("load texture: %s", name.c_str());
    TextureInfo tex_info;
    tex_info.Filename = name;
    tex_info.Filtering = TextureFilters::Anisotropic;
    tex_info.GenerateMipmap = true;
    Texture* t = new Texture(context_);
    t->createFromFile( tex_info );
    cache_[filename] = t;
    return t;
}

}
}
