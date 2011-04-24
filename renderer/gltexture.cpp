#include "renderer/gltexture.h"

#define STBI_FAILURE_USERMSG
#include "stb/stb_image.c"

#include <iostream>

namespace ext {
namespace opengl {

namespace PixelTypes {
    GLenum GetGLType(u32 type) {
        switch (type) {
            case Gray:      return GL_LUMINANCE;
            case GrayAlpha: return GL_LUMINANCE_ALPHA;
            case RGB:       return GL_RGB;
            case RGBA:      return GL_RGBA;
            // return GL_ALPHA;
        }
        return 0;
    }
}

namespace TextureMipmaps {
    u32 CalcMipmap(u32 width, u32 height) {
        u32 count = 0;
        do {
            if (width > 1)
                width = width/2;
            if (height > 1) height = height/2;
                count += 1;
        } while (width > 1 && height > 1);
        return count;
    }
    u32 CalcMipmap(u32 width, u32 height, u32 depth) {
        u32 count = 0;
        do {
            if (width > 1)
                width = width/2;
            if (height > 1) 
                height = height/2;
            if (depth > 1)
                depth = depth/2;
            count += 1;
        } while (width > 1 && height > 1 && depth > 1);
        return count;
    }
}

TextureInfo::TextureInfo()
    : WrapT(TextureWraps::REPEAT)
    , WrapS(TextureWraps::REPEAT)
    , WrapR(TextureWraps::REPEAT)
    , NumMipmaps(TextureMipmaps::MaxMipmaps)
    , MinFilter(TextureMinFilters::NEAREST_MIPMAP_LINEAR)
    , MagFilter(TextureMagFilters::LINEAR)
    , Type(TextureTypes::Texture2D) {
}


Texture::Texture() 
    : image_(NULL) {
}

Texture::~Texture() {
    glDeleteTextures(1, &id_);
}

void Texture::Bind() {
    glBindTexture(info_.Type, id_);
}

void Texture::Generate() {

    image_ = stbi_load(info_.Filename.c_str(), &info_.Width, &info_.Height, &info_.Depth, 0);

    if(image_ == NULL)
    {   
        assert(false);
    }
    glEnable(GL_TEXTURE_2D);

    glGenTextures(1, &id_);
    glBindTexture(GL_TEXTURE_2D, id_);
    glTexParameteri(info_.Type, GL_TEXTURE_MIN_FILTER, info_.MinFilter);
    glTexParameteri(info_.Type, GL_TEXTURE_MAG_FILTER, info_.MagFilter);
    glTexParameteri(info_.Type, GL_TEXTURE_WRAP_S, info_.WrapS);
    glTexParameteri(info_.Type, GL_TEXTURE_WRAP_T, info_.WrapT);

    glTexImage2D(
            GL_TEXTURE_2D,
            0,
            GL_RGB, 
            info_.Width, 
            info_.Height, 
            0, 
            GL_RGB,
            GL_UNSIGNED_BYTE, 
            image_);
    std::cout << "loaded: " << info_.Filename << " at " << info_.Width << "x" << info_.Height << std::endl;

    stbi_image_free(image_);
}

}
}
