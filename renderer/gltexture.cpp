/**
 * @author  Alexey Vasilyev <alexa.infra@gmail.com>
 * @section LICENSE
 * 
 * This file is part of Negine (https://alexadotlife.com/hg/negine)
 * Licensed under LGPLv3 (or GPLv3) - see LICENSE file for details
 **/
#include "renderer/gltexture.h"

#define STBI_FAILURE_USERMSG
#include "stb/stb_image.c"

#include <iostream>

namespace base {
namespace opengl {

namespace PixelTypes {
    GLenum GetGLType(u32 type) {
        switch (type) {
            case Alpha:     return GL_ALPHA;
            case Gray:      return GL_LUMINANCE;
            case GrayAlpha: return GL_LUMINANCE_ALPHA;
            case RGB:       return GL_RGB;
            case RGBA:      return GL_RGBA;
            // return GL_ALPHA;
        }
        return 0;
    }
}

TextureInfo::TextureInfo()
    : WrapT(TextureWraps::REPEAT)
    , WrapS(TextureWraps::REPEAT)
    , WrapR(TextureWraps::REPEAT)
    , MinFilter(TextureMinFilters::NEAREST_MIPMAP_LINEAR)
    , MagFilter(TextureMagFilters::LINEAR)
    , Type(TextureTypes::Texture2D)
    , Pixel(PixelTypes::RGBA)
    , GenerateMipmap(true) {
}


Texture::Texture() 
    : id_(0) {
    glGenTextures(1, &id_);
}

Texture::~Texture() {
    glDeleteTextures(1, &id_);
}

void Texture::Bind() {
    glBindTexture(info_.Type, id_);
}

void Texture::GenerateFromBuffer(const TextureInfo& textureinfo, const u8* data) {
    info_ = textureinfo;
    FromBuffer(data);
}

void Texture::GenerateFromFile(const TextureInfo& textureinfo) {
    info_ = textureinfo;

    u8* image = stbi_load(info_.Filename.c_str(), &info_.Width, &info_.Height, &info_.Depth, 0);

    if(image == NULL)
    {   
        std::cout << stbi_failure_reason() << std::endl;
        assert(false);
    }

    FromBuffer(image);
    
    stbi_image_free(image);
}

void Texture::FromBuffer(const u8* data) {
    if (!info_.GenerateMipmap)
    {
        assert(info_.MinFilter == TextureMinFilters::LINEAR
            || info_.MinFilter == TextureMinFilters::NEAREST);
    }

    glBindTexture(info_.Type, id_);
    glTexParameteri(info_.Type, GL_TEXTURE_MIN_FILTER, info_.MinFilter);
    glTexParameteri(info_.Type, GL_TEXTURE_MAG_FILTER, info_.MagFilter);
    glTexParameteri(info_.Type, GL_TEXTURE_WRAP_S, info_.WrapS);
    glTexParameteri(info_.Type, GL_TEXTURE_WRAP_T, info_.WrapT);

    glTexImage2D(
            GL_TEXTURE_2D,
            0,
            GL_RGBA, 
            info_.Width, 
            info_.Height, 
            0, 
            PixelTypes::GetGLType(info_.Pixel),
            GL_UNSIGNED_BYTE, 
            data);

//    if (info_.GenerateMipmap)
//        glGenerateMipmap(GL_TEXTURE_2D);

}

}
}
