#pragma once

namespace ext {

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
    GLenum GetGLType(TextureUsage usage);
}
typedef TextureUsages::TextureUsage TextureUsage;

namespace TextureTypes {
    //! Texture type
    enum TextureType {
        1D, 
        2D, 
        3D, 
        Cube
    };
    GLenum GetGLType(TextureType type);
};
typedef TextureTypes::TextureType TextureType;

namespace TextureMipmaps {
    const u32 MaxMipmaps = 0xffff;  //!< Maximum number of mipmaps, 
                                    //!< if used then choose max number of 
                                    //!< mipmaps for width/height
    u32 CalcMipmap(u32 width, u32 height);
    u32 CalcMipmap(u32 width, u32 height, u32 depth);
}

namespace TextureMinFilters {
    //! Texture minifying function is used whenever the pixel being textured 
    //! maps to an area greater than one texture element.
    enum TextureMinFilter {
        NEAREST,                //!< Returns the value of the texture element 
                                //!< that is nearest (in Manhattan distance) 
                                //!< to the center of the pixel being textured.
        LINEAR,                 //!< Returns the weighted average of the four 
                                //!< texture elements that are closest to 
                                //!< the center of the pixel being textured.
        NEAREST_MIPMAP_NEAREST, //!< Chooses the mipmap that most closely 
                                //!< matches the size of the pixel being textured 
                                //!< and uses the NEAREST criterion
        LINEAR_MIPMAP_NEAREST,  //!< Chooses the mipmap that most closely matches 
                                //!< the size of the pixel being textured and 
                                //!< uses the LINEAR criterion
        NEAREST_MIPMAP_LINEAR,  //!< Chooses the two mipmaps that most closely 
                                //!< match the size of the pixel being textured 
                                //!< and uses the NEAREST criterion, returns
                                //!< wighted average of two values
        LINEAR_MIPMAP_LINEAR    //!< Chooses the two mipmaps that most closely 
                                //!< match the size of the pixel being textured 
                                //!< and uses the LINEAR criterion, returns 
                                //!< wighted average of two values
    };
    GLenum GetGLType(TextureMinFilter filter);
}
typedef TextureMinFilters::TextureMinFilter TextureMinFilter;

namespace TextureMagFilters {
    //! Texture magnification function is used when the pixel being textured 
    //! maps to an area less than or equal to one texture element.
    enum TextureMagFilter {
        NEAREST,    //!< Returns the value of the texture element that is 
                    //!< nearest (in Manhattan distance) to the center of 
                    //!< the pixel being textured.
        LINEAR      //!< Returns the weighted average of the four texture 
                    //!< elements that are closest to the center of 
                    //!< the pixel being textured.
    };
    GLenum GetGLType(TextureMagFilter filter);
}
typedef TextureMagFilters::TextureMagFilter TextureMagFilter;

namespace TextureWraps {
    //! Enumerates texture wrap modes
    enum TextureWrap {
        CLAMP_TO_EDGE,  //!< coordinates are clamped to the range 1/2 texel 
                        //!< inside [0, 1]
        MIRROR_REPEAT,  //!< if integer part is even then use fractional part 
                        //!< if integer part is odd theb use (1-x) of fractional 
        REPEAT          //!< uses fractional part of coordinates only
    };
    GLenum GetGLType(TextureWrap wrap);
}

struct TextureInfo {
    TextureUsage Usage;
    TextureType Type;
    u32 NumMipmap;
    TextureMagFilter MagFilter;
    TextureMinFilter MinFilter;
    TextureWrap WrapT, WrapS, WrapR;
    
    TextureInfo();
};

class GLTexture {
protected:
    GLuint id_;
    const TextureInfo info_;
    const Image* image_;
    bool is_ok_;
public:
    GLTexture(TextureInfo& info, Image* image);
    ~GLTexture();
    
    bool is_ok() const { return is_ok_; }
    const TextureInfo& info() const { return info_; }
    const Image* data() const { return image_; }

    void Bind();
    void Generate();
};

}
