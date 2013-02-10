/**
 * \file
 * \author      Alexey Vasilyev <alexa.infra@gmail.com>
 * \copyright   MIT License
 **/
#include "render/gltexture.h"

#define STBI_FAILURE_USERMSG
#include "stb/stb_image.c"

#include "base/stream.h"
#include <iostream>
#include "render/statistics.h"

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
    , MagFilter( TextureMagFilters::LINEAR )
    , MinFilter( TextureMinFilters::NEAREST_MIPMAP_LINEAR )
    , WrapT( TextureWraps::REPEAT )
    , WrapS( TextureWraps::REPEAT )
    , WrapR( TextureWraps::REPEAT )
    , GenerateMipmap( true )
    , Pixel( PixelTypes::RGBA )
    , DataType( GLDataTypes::UByte )
    , InternalType( GL_RGBA )
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

    FromBuffer( image );
    stbi_image_free( image );
}

void Texture::FromBuffer( const u8* data )
{
    ASSERT( id_ == 0 );
    glGenTextures( 1, &id_ );

    glBindTexture( info_.Type, id_ );
    setup();

    glTexImage2D(
        info_.Type,
        0,
        info_.InternalType,
        info_.Width,
        info_.Height,
        0,
        info_.Pixel,
        info_.DataType,
        data );
//    if (info_.GenerateMipmap)
//        glGenerateMipmap(GL_TEXTURE_2D);
}

void Texture::GenerateEmpty( const TextureInfo& textureinfo )
{
    ASSERT( id_ == 0 );
    glGenTextures( 1, &id_ );

    glBindTexture( info_.Type, id_ );
    setup();

    u8* data = new u8[info_.dataSize()];

    glTexImage2D(
        info_.Type,
        0,
        info_.InternalType,
        info_.Width,
        info_.Height,
        0,
        info_.Pixel,
        info_.DataType,
        data );
    delete[] data;
}

void Texture::setup()
{
    if ( !info_.GenerateMipmap ) {
        ASSERT( info_.MinFilter == TextureMinFilters::LINEAR
                || info_.MinFilter == TextureMinFilters::NEAREST );
    }

    glTexParameteri( info_.Type, GL_TEXTURE_MIN_FILTER, info_.MinFilter );
    glTexParameteri( info_.Type, GL_TEXTURE_MAG_FILTER, info_.MagFilter );
    glTexParameteri( info_.Type, GL_TEXTURE_WRAP_S, info_.WrapS );
    glTexParameteri( info_.Type, GL_TEXTURE_WRAP_T, info_.WrapT );
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
        WARN("%s not found", filename.c_str());
        cache_[filename] = NULL;
        return NULL;
    }

    LOG("load texture: %s", name.c_str());
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
