/**
 * @author  Alexey Vasilyev <alexa.infra@gmail.com>
 * @section LICENSE
 * 
 * This file is part of Negine (https://hg.alexadotlife.com/negine)
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
            case Alpha:     return GL_RED;
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
        return;
    }

    switch(info_.Depth) {
        case 1: info_.Pixel = PixelTypes::Alpha; break;
        case 3: info_.Pixel = PixelTypes::RGB; break;
        case 4: info_.Pixel = PixelTypes::RGBA; break;
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

size_t hash_string::operator()(const std::string& x) const {
    const char* str = x.c_str();
    u32 hash = *str;
    if(hash) {
        for(str += 1; *str != '\0'; ++str)
            hash = (hash << 5) - hash + *str;
    }
    return hash;
}

TextureLoader::TextureLoader() {
}

TextureLoader::~TextureLoader() {
    ClearCache();
}

void TextureLoader::ClearCache() {
    for(auto it = cache_.begin(); it != cache_.end(); ++it) {
        delete it->second;
    }
    cache_.clear();
}

Texture* TextureLoader::Load(const std::string& filename) {
    auto found = cache_.find(filename);
    if (found != cache_.end()) {
        return found->second;
    }
    TextureInfo tex_info;
    tex_info.Filename = filename;
    tex_info.MinFilter = TextureMinFilters::LINEAR;
    tex_info.GenerateMipmap = true;

    Texture* t = new Texture();
    t->GenerateFromFile(tex_info);
    cache_[filename] = t;
    return t;
}

}
}
