/**
 * \file
 * \brief       Wrapper for texture object
 * \author      Alexey Vasilyev <alexa.infra@gmail.com>
 * \copyright   MIT License
 **/
#pragma once

#include "base/types.h"
#include "renderer/glcontext.h"
#include <unordered_map>

namespace base {
namespace opengl {

namespace PixelTypes {
    //! Enumerates pixel types.
    enum PixelType {
        R = (1 << 1),
        G = (1 << 2),
        B = (1 << 3),
        A = (1 << 4),
    };

    const u32 Alpha = A;
    const u32 Gray = R;
    const u32 GrayAlpha = R | A;
    const u32 RGB = R | G | B;
    const u32 RGBA = R | G | B | A;
    const u32 Depth = R;

    GLenum GetGLType(u32 type);
}
typedef u32 PixelType;

namespace TextureUsages {
    enum TextureUsage {
        RenderTarget,
        StaticData
    };
}
typedef TextureUsages::TextureUsage TextureUsage;

namespace TextureTypes {
    //! Texture type
    enum TextureType {
        Texture1D = GL_TEXTURE_1D, 
        Texture2D = GL_TEXTURE_2D,
        Texture3D = GL_TEXTURE_3D,
        TextureCube = GL_TEXTURE_CUBE_MAP
    };
};
typedef TextureTypes::TextureType TextureType;

namespace TextureMinFilters {
    //! Texture minifying function is used whenever the pixel being textured 
    //! maps to an area greater than one texture element.
    enum TextureMinFilter {
        NEAREST = GL_NEAREST,   //!< Returns the value of the texture element 
                                //!< that is nearest (in Manhattan distance) 
                                //!< to the center of the pixel being textured.
        LINEAR = GL_LINEAR,     //!< Returns the weighted average of the four 
                                //!< texture elements that are closest to 
                                //!< the center of the pixel being textured.
        NEAREST_MIPMAP_NEAREST = GL_NEAREST_MIPMAP_NEAREST, //!< Chooses the 
                                //!< mipmap that most closely matches the size 
                                //!< of the pixel being textured 
                                //!< and uses the NEAREST criterion
        LINEAR_MIPMAP_NEAREST = GL_LINEAR_MIPMAP_NEAREST,  //!< Chooses the 
                                //!< mipmap that most closely matches 
                                //!< the size of the pixel being textured and 
                                //!< uses the LINEAR criterion
        NEAREST_MIPMAP_LINEAR = GL_NEAREST_MIPMAP_LINEAR,  //!< Chooses the 
                                //!< two mipmaps that most closely 
                                //!< match the size of the pixel being textured 
                                //!< and uses the NEAREST criterion, returns
                                //!< wighted average of two values
        LINEAR_MIPMAP_LINEAR = GL_LINEAR_MIPMAP_LINEAR     //!< Chooses the
                                //!< two mipmaps that most closely 
                                //!< match the size of the pixel being textured 
                                //!< and uses the LINEAR criterion, returns 
                                //!< wighted average of two values
    };
}
typedef TextureMinFilters::TextureMinFilter TextureMinFilter;

namespace TextureMagFilters {
    //! Texture magnification function is used when the pixel being textured 
    //! maps to an area less than or equal to one texture element.
    enum TextureMagFilter {
        NEAREST = GL_NEAREST,   //!< Returns the value of the texture element
                                //!< that is nearest (in Manhattan distance) to the center of 
                                //!< the pixel being textured.
        LINEAR = GL_LINEAR      //!< Returns the weighted average of the four
                                //!< texture elements that are closest to the center of 
                                //!< the pixel being textured.
    };
}
typedef TextureMagFilters::TextureMagFilter TextureMagFilter;

namespace TextureWraps {
    //! Enumerates texture wrap modes
    enum TextureWrap {
        CLAMP_TO_EDGE = GL_CLAMP_TO_EDGE,   //!< coordinates are clamped to the
                                            //!< range 1/2 texel inside [0, 1]
        MIRROR_REPEAT = GL_MIRRORED_REPEAT, //!< if integer part is even then
                                            //!< use fractional part if integer part is odd then
                                            //!< use (1-x) of fractional 
        REPEAT = GL_REPEAT                  //!< uses fractional part of
                                            //!< coordinates only
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
    i32 Depth;                              //!< Depth color
    PixelType Pixel;                        //!< Pixel format
    
    TextureInfo();
};

//! Texture object
class Texture {
protected:
    GLuint id_;         //!< Texture name
    TextureInfo info_;  //!< Current info
    bool is_ok_;        //!< Creation status
public:
    Texture();
    ~Texture();
    
    //! Gets creation status
    bool is_ok() const { return is_ok_; }

    //! Gets texture info
    const TextureInfo& info() { return info_; }

    //! Gets name of texture
    GLuint id() const { return id_; }

    //! Bind texture
    void Bind();

    //! Generate texture object from texture info
    void GenerateFromFile(const TextureInfo& textureinfo);

    //! Generate texture object from texture info
    void GenerateFromBuffer(const TextureInfo& textureinfo, const u8* data);

private:
    void FromBuffer(const u8* data);

private:
    DISALLOW_COPY_AND_ASSIGN(Texture);
};

class TextureLoader {
    std::unordered_map<std::string, Texture*, hash_string> cache_;
public:
    TextureLoader();
    ~TextureLoader();
    Texture* Load(const std::string& filename);
    void ClearCache();
private:
    DISALLOW_COPY_AND_ASSIGN(TextureLoader);
};

}
}
