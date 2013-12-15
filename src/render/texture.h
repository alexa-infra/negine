/**
 * \file
 * \brief       Wrapper for texture object
 * \author      Alexey Vasilyev <alexa.infra@gmail.com>
 * \copyright   MIT License
 **/
#pragma once

#include "base/types.h"
#include "render/gpuresource.h"
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

namespace TextureFilters
{
    enum TextureFilter {
        Linear,
        Anisotropic,
        Nearest
    };
}
typedef TextureFilters::TextureFilter TextureFilter;

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
    TextureFilter Filtering;                //!< Filtering type
    TextureWrap Wrap;                       //!< Wrap texture coordinate
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
class Texture : public GpuResource
{
protected:
    TextureInfo info_;  //!< Current info
public:
    Texture(DeviceContext& gl);
    ~Texture();

    //! Gets texture info
    const TextureInfo& info() {
        return info_;
    }

    //! Bind texture
    void bind();

    void create(const TextureInfo& textureinfo);

    void destroy();
private:
    void setup();

    //! Generate texture object from texture info
    void createFromFile( const TextureInfo& textureinfo );

    //! Generate texture object from texture info
    void createFromBuffer( const TextureInfo& textureinfo, const u8* data );

    void createEmpty( const TextureInfo& textureinfo );
private:
    DISALLOW_COPY_AND_ASSIGN( Texture );
};

class TextureLoader
{
    typedef std::map<std::string, Texture*> TextureCache;
    TextureCache cache_;
    DeviceContext& context_;

public:
    TextureLoader(DeviceContext& gl);
    ~TextureLoader();
    Texture* load( const std::string& filename );
    void clearCache();
private:
    DISALLOW_COPY_AND_ASSIGN( TextureLoader );
};

}
}
