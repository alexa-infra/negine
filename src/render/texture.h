/**
 * \file
 * \brief       Wrapper for texture object
 * \author      Alexey Vasilyev <alexa.infra@gmail.com>
 * \copyright   MIT License
 **/
#pragma once

#include "base/types.h"
#include "render/glcontext.h"
#include <map>
#include <string>

namespace base
{
namespace opengl
{

namespace GLDataTypes
{
enum GLDataType {
    Byte        = GL_BYTE,
    UByte       = GL_UNSIGNED_BYTE,
    Short       = GL_SHORT,
    UShort      = GL_UNSIGNED_SHORT,
    Int         = GL_INT,
    UInt        = GL_UNSIGNED_INT,
    Float       = GL_FLOAT,
    Double      = GL_DOUBLE
};
u32 sizeInBytes(GLDataType dt);
}
typedef GLDataTypes::GLDataType GLDataType;

namespace PixelTypes
{
//! Enumerates pixel types.
enum PixelType {
    R       = GL_RED,
    RG      = GL_RG,
    RGB     = GL_RGB,
    RGBA    = GL_RGBA,
    Depth   = GL_DEPTH_COMPONENT
};
u32 componentCount(PixelType type);
}
typedef PixelTypes::PixelType PixelType;

namespace TextureUsages
{
enum TextureUsage {
    RenderTarget,
    StaticData
};
}
typedef TextureUsages::TextureUsage TextureUsage;

namespace TextureTypes
{
//! Texture type
enum TextureType {
    Texture2D   = GL_TEXTURE_2D,
    TextureRect = GL_TEXTURE_RECTANGLE
};
};
typedef TextureTypes::TextureType TextureType;

namespace TextureMinFilters
{
//! Texture minifying function is used whenever the pixel being textured
//! maps to an area greater than one texture element.
enum TextureMinFilter {
    NEAREST                     = GL_NEAREST,
    LINEAR                      = GL_LINEAR,
    NEAREST_MIPMAP_NEAREST      = GL_NEAREST_MIPMAP_NEAREST,
    LINEAR_MIPMAP_NEAREST       = GL_LINEAR_MIPMAP_NEAREST,
    NEAREST_MIPMAP_LINEAR       = GL_NEAREST_MIPMAP_LINEAR,
    LINEAR_MIPMAP_LINEAR        = GL_LINEAR_MIPMAP_LINEAR
};
}
typedef TextureMinFilters::TextureMinFilter TextureMinFilter;

namespace TextureMagFilters
{
//! Texture magnification function is used when the pixel being textured
//! maps to an area less than or equal to one texture element.
enum TextureMagFilter {
    NEAREST     = GL_NEAREST,
    LINEAR      = GL_LINEAR
};
}
typedef TextureMagFilters::TextureMagFilter TextureMagFilter;

namespace TextureWraps
{
//! Enumerates texture wrap modes
enum TextureWrap {
    CLAMP_TO_EDGE       = GL_CLAMP_TO_EDGE,
    MIRROR_REPEAT       = GL_MIRRORED_REPEAT,
    REPEAT              = GL_REPEAT
};
}
typedef TextureWraps::TextureWrap TextureWrap;

//! Texture creation info
struct TextureInfo {
    TextureUsage Usage;                     //!< Usage
    TextureType Type;                       //!< Texture type
    TextureMagFilter MagFilter;             //!< Magnification filter
    TextureMinFilter MinFilter;             //!< Minification filter
    TextureWrap WrapT;                      //!< Wrap mode of T coordinate
    TextureWrap WrapS;                      //!< Wrap mode of S coordinate
    TextureWrap WrapR;                      //!< Wrap mode of R coordinate
    bool GenerateMipmap;

    std::string Filename;                   //!< Source file name
    i32 Width;                              //!< Width of image
    i32 Height;                             //!< Height of image
    i32 ComponentCount;                     //!< Component count
    PixelType Pixel;                        //!< Pixel format
    GLDataType DataType;
    GLenum InternalType;

    TextureInfo();

    u32 pixelSize() const { return PixelTypes::componentCount(Pixel) * GLDataTypes::sizeInBytes(DataType); }
    u32 dataSize() const { return Width * Height * pixelSize(); }
};

//! Texture object
class Texture
{
protected:
    GLuint id_;         //!< Texture name
    TextureInfo info_;  //!< Current info
public:
    Texture();
    ~Texture();

    //! Gets creation status
    bool is_ok() const {
        return id_ == 0;
    }

    //! Gets texture info
    const TextureInfo& info() {
        return info_;
    }

    //! Gets name of texture
    GLuint id() const {
        return id_;
    }

    //! Bind texture
    void Bind();

    //! Generate texture object from texture info
    void GenerateFromFile( const TextureInfo& textureinfo );

    //! Generate texture object from texture info
    void GenerateFromBuffer( const TextureInfo& textureinfo, const u8* data );

    void GenerateEmpty( const TextureInfo& textureinfo );

    void Destroy();

private:
    void FromBuffer( const u8* data );

    void setup();
private:
    DISALLOW_COPY_AND_ASSIGN( Texture );
};

class TextureLoader
{
    typedef std::map<std::string, Texture*> TextureCache;
    TextureCache cache_;

public:
    TextureLoader();
    ~TextureLoader();
    Texture* Load( const std::string& filename );
    void ClearCache();
private:
    DISALLOW_COPY_AND_ASSIGN( TextureLoader );
};

}
}
