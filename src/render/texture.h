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

namespace InternalTypes
{
    enum InternalType {
        RGB8        = GL_RGB8,
        RGBA8       = GL_RGBA8,
        RGBA16F     = GL_RGBA16F,

        D24S8       = GL_DEPTH24_STENCIL8,
        D32FS8      = GL_DEPTH32F_STENCIL8,

        D16         = GL_DEPTH_COMPONENT16,
        D24         = GL_DEPTH_COMPONENT24, 
        D32         = GL_DEPTH_COMPONENT32,
        D32F        = GL_DEPTH_COMPONENT32F,

        S1          = GL_STENCIL_INDEX1,
        S4          = GL_STENCIL_INDEX4,
        S8          = GL_STENCIL_INDEX8,
        S16         = GL_STENCIL_INDEX16
    };
    GLenum toDataType(InternalType value);
    bool isColor(InternalType value);
    bool isStencil(InternalType value);
    bool isDepth(InternalType value);
    bool isDepthStencil(InternalType value);
    u32 sizeInBytes(InternalType value);
}
typedef InternalTypes::InternalType InternalType;

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
    InternalTypes::InternalType InternalType;

    TextureInfo();

    u32 pixelSize() const { return PixelTypes::componentCount(Pixel) * InternalTypes::sizeInBytes(InternalType); }
    u32 dataSize() const { return Width * Height * pixelSize(); }
};

//! Texture object
class Texture : public GpuResource, public ResourceBase<Texture>
{
public:
    Texture(DeviceContext& gl);

    ~Texture();

    inline const TextureInfo& info() const { return info_; }

    void bind();

    void unbind();

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
