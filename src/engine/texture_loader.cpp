#include "texture_loader.h"

#define STBI_FAILURE_USERMSG
#include "stb/stb_image.c"

#include "base/env.h"
#include "base/path.h"
#include "base/log.h"
#include "base/debug.h"

namespace base {

using namespace opengl;

struct StbiImage {
    StbiImage(const std::string& path, TextureInfo& info) : buffer(NULL) {
        buffer = stbi_load( path.c_str(), &info.Width, &info.Height, &info.ComponentCount, 0 );
    }
    ~StbiImage() {
        if (buffer != NULL)
            stbi_image_free(buffer);
    }
    u8* buffer;
    bool isOk() const { return buffer != NULL; }
};

Texture* loadTexture(opengl::DeviceContext& GL, const TextureInfo& defaultInfo, const std::string& path) {
    TextureInfo info = defaultInfo;
    StbiImage image(path, info);
    if (!image.isOk()) {
        ERR("Failed load image: %s", stbi_failure_reason());
        return nullptr;
    }

    switch( info.ComponentCount ) {
        case 1: info.Pixel = PixelTypes::R;    break;
        case 2: info.Pixel = PixelTypes::RG;   break;
        case 3: info.Pixel = PixelTypes::RGB;  break;
        case 4: info.Pixel = PixelTypes::RGBA; break;
    }
    info.Filtering = TextureFilters::Linear;
    info.GenerateMipmap = false;

    Texture* texture = new Texture(GL);
    texture->createFromBuffer(info, image.buffer);
    return texture;
}

} // namespace base