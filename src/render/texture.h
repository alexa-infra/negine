/**
 * \file
 * \brief       Wrapper for texture object
 * \author      Alexey Vasilyev <alexa.infra@gmail.com>
 * \copyright   MIT License
 **/
#pragma once

#include "base/types.h"
#include "render/gpuresource.h"
#include "engine/resource.h"

namespace base {
namespace opengl {

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

namespace TextureTypes
{
    enum TextureType {
        Texture2D   = GL_TEXTURE_2D,
        TextureRect = GL_TEXTURE_RECTANGLE
    };
}
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
    enum TextureWrap {
        CLAMP_TO_EDGE       = GL_CLAMP_TO_EDGE,
        MIRROR_REPEAT       = GL_MIRRORED_REPEAT,
        REPEAT              = GL_REPEAT
    };
}
typedef TextureWraps::TextureWrap TextureWrap;

struct TextureInfo
{
    TextureType Type;
    TextureFilter Filtering;
    TextureWrap Wrap;
    bool GenerateMipmap;

    i32 Width;
    i32 Height;
    i32 ComponentCount;
    PixelType Pixel;
    GLDataType DataType;
    GLenum InternalType;

    TextureInfo();

    u32 pixelSize() const { return PixelTypes::componentCount(Pixel) * GLDataTypes::sizeInBytes(DataType); }
    u32 dataSize() const { return Width * Height * pixelSize(); }
};

//! Texture object
class Texture : public GpuResource, public BaseResource<Texture>
{
public:
    Texture(DeviceContext& gl);

    ~Texture();

    inline const TextureInfo& info() const { return info_; }

    void bind();

    void createFromBuffer( const TextureInfo& textureinfo, const u8* data );

    void createEmpty( const TextureInfo& textureinfo );

    void destroy();
private:
    void setup();

    TextureInfo info_;
private:
    DISALLOW_COPY_AND_ASSIGN( Texture );
};

} // namespace opengl
} // namespace base
