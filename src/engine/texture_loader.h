#pragma once

#include "base/types.h"
#include "render/texture.h"

namespace base {

NEGINE_API opengl::Texture* loadTexture(opengl::DeviceContext& GL, const opengl::TextureInfo& info, const std::string& path);

} // namespace base