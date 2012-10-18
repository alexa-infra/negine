/**
 * \file
 * \author      Alexey Vasilyev <alexa.infra@gmail.com>
 * \copyright   MIT License
 **/
#include "renderer/gltexture.h"

#define STBI_FAILURE_USERMSG
#include "stb/stb_image.c"

#include "base/stream.h"
#include <iostream>
#include "renderer/statistics.h"

namespace base
{
namespace opengl
{

namespace PixelTypes
{
GLenum GetGLType( u32 type )
{
    switch ( type ) {
    case Alpha:
        return GL_RED;
    case Gray:
        return GL_LUMINANCE;
    case GrayAlpha:
        return GL_LUMINANCE_ALPHA;
    case RGB:
        return GL_RGB;
    case RGBA:
        return GL_RGBA;
        // return GL_ALPHA;
    }

    return 0;
}
}

TextureInfo::TextureInfo()
    : Type( TextureTypes::Texture2D )
    , MagFilter( TextureMagFilters::LINEAR )
    , MinFilter( TextureMinFilters::NEAREST_MIPMAP_LINEAR )
    , WrapT( TextureWraps::REPEAT )
    , WrapS( TextureWraps::REPEAT )
    , WrapR( TextureWraps::REPEAT )
    , GenerateMipmap( true )
    , Pixel( PixelTypes::RGBA )
{
}


Texture::Texture()
    : id_( 0 )
{
}

Texture::~Texture()
{
    Destroy();
}

void Texture::Destroy()
{
    if ( id_ != 0 ) {
        glDeleteTextures( 1, &id_ );
        id_ = 0;
    }
}

void Texture::Bind()
{
    glBindTexture( info_.Type, id_ );
    Stats::inc_texture_switches();
}

void Texture::GenerateFromBuffer( const TextureInfo& textureinfo, const u8* data )
{
    info_ = textureinfo;
    FromBuffer( data );
}

void Texture::GenerateFromFile( const TextureInfo& textureinfo )
{
    info_ = textureinfo;
    u8* image = stbi_load( info_.Filename.c_str(), &info_.Width, &info_.Height, &info_.Depth, 0 );

    if( image == NULL ) {
        std::cout << stbi_failure_reason() << std::endl;
        return;
    }

    switch( info_.Depth ) {
    case 1:
        info_.Pixel = PixelTypes::Alpha;
        break;
    case 3:
        info_.Pixel = PixelTypes::RGB;
        break;
    case 4:
        info_.Pixel = PixelTypes::RGBA;
        break;
    }

    FromBuffer( image );
    stbi_image_free( image );
}

void Texture::FromBuffer( const u8* data )
{
    assert( id_ == 0 );
    glGenTextures( 1, &id_ );

    if ( !info_.GenerateMipmap ) {
        assert( info_.MinFilter == TextureMinFilters::LINEAR
                || info_.MinFilter == TextureMinFilters::NEAREST );
    }

    glBindTexture( info_.Type, id_ );
    glTexParameteri( info_.Type, GL_TEXTURE_MIN_FILTER, info_.MinFilter );
    glTexParameteri( info_.Type, GL_TEXTURE_MAG_FILTER, info_.MagFilter );
    glTexParameteri( info_.Type, GL_TEXTURE_WRAP_S, info_.WrapS );
    glTexParameteri( info_.Type, GL_TEXTURE_WRAP_T, info_.WrapT );
    glTexImage2D(
        GL_TEXTURE_2D,
        0,
        GL_RGBA,
        info_.Width,
        info_.Height,
        0,
        PixelTypes::GetGLType( info_.Pixel ),
        GL_UNSIGNED_BYTE,
        data );
//    if (info_.GenerateMipmap)
//        glGenerateMipmap(GL_TEXTURE_2D);
}

TextureLoader::TextureLoader()
{
}

TextureLoader::~TextureLoader()
{
    ClearCache();
}

void TextureLoader::ClearCache()
{
    for( TextureCache::iterator it = cache_.begin(); it != cache_.end(); ++it ) {
        delete it->second;
    }

    cache_.clear();
}

Texture* TextureLoader::Load( const std::string& filename )
{
    TextureCache::iterator found = cache_.find( filename );

    if ( found != cache_.end() ) {
        return found->second;
    }

    std::string name;

    if ( file_exists( filename ) ) {
        name = filename;
    } else if ( file_exists( filename + ".jpg" ) ) {
        name = filename + ".jpg";
    } else if ( file_exists( filename + ".tga" ) ) {
        name = filename + ".tga";
    } else if ( file_exists( filename + ".png" ) ) {
        name = filename + ".png";
    } else {
        std::cout << filename << " not found" << std::endl;
        cache_[filename] = NULL;
        return NULL;
    }

    std::cout << "load texture: " << name << std::endl;
    TextureInfo tex_info;
    tex_info.Filename = name;
    tex_info.MinFilter = TextureMinFilters::LINEAR;
    tex_info.GenerateMipmap = true;
    Texture* t = new Texture();
    t->GenerateFromFile( tex_info );
    cache_[filename] = t;
    return t;
}

}
}
