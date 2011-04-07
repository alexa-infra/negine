#include "renderer/gltexture.h"

namespace ext {

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

namespace TextureTypes {
    GLenum GetGLType(TextureType type) {
        switch (type) {
            case Texture1D:    return GL_TEXTURE_1D;
            case Texture2D:    return GL_TEXTURE_2D;
            case Texture3D:    return GL_TEXTURE_3D;
            case TextureCube:  return GL_TEXTURE_CUBE;
        }
        return 0;
    }
};

namespace TextureMinFilters {
    GLenum GetGLType(TextureMinFilter filter) {
        switch (filter) {
            case NEAREST:                   return GL_NEAREST;
            case LINEAR:                    return GL_LINEAR;
            case NEAREST_MIPMAP_NEAREST:    return GL_NEAREST_MIPMAP_NEAREST;
            case LINEAR_MIPMAP_NEAREST:     return GL_LINEAR_MIPMAP_NEAREST;
            case NEAREST_MIPMAP_LINEAR:     return GL_NEAREST_MIPMAP_LINEAR;
            case LINEAR_MIPMAP_LINEAR:      return GL_LINEAR_MIPMAP_LINEAR;
        }
        return 0;
    }
}

namespace TextureMagFilters {
    GLenum GetGLType(TextureMagFilter filter) {
        switch (filter) {
            case NEAREST: return GL_NEAREST;
            case LINEAR: return GL_LINEAR;
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

namespace TextureWraps {
    GLenum GetGLType(TextureWrap wrap) {
        switch (wrap) {
            case CLAMP_TO_EDGE: return GL_CLAMP_TO_EDGE;
            case MIRROR_REPEAT: return GL_MIRROR_REPEAT;
            case REPEAT:        return GL_REPEAT;
        }
        return 0;
    }
}

TextureInfo::TextureInfo()
    : WrapT(TextureWraps::REPEAT)
    , WrapS(TextureWraps::REPEAT)
    , WrapR(TextureWraps::REPEAT)
    , NumMipmaps(TextureMipmaps::MaxMipmaps)
    , MinFilter(TextureMinFilters::NEAREST_MIPMAP_LINEAR)
    , MagFilter(TextureMagFilters::LINEAR) {
}


GLTexture::GLTexture(TextureInfo& info) 
    : info_(info) {
    glGenTextures(1, &id_);
    is_ok_ = (glIsTexture(id_) == GL_TRUE);
}

GLTexture::~GLTexture() {
    if (!is_ok_)
        return;
    glDeleteTextures(1, &id_);
}

GLTexture::Bind() {
    glBindTexture(TextureType::GetGLType(info_.Type), id_);
}

GLTexture::Generate() {
    u32 maxmipap = TextureMipmaps::CalcMipmaps();
}

}
