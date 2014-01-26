#pragma once

#include "game/componentbase.h"
#include "engine/resourceref.h"
#include "render/model.h"
#include "math/matrix.h"

namespace base {
namespace game {

class Renderable : public ComponentBase
{
public:
    ResourceRef model_;
    opengl::Model* model();
    math::Matrix4 world() const;
    const char* extension() const { return ".model"; }
};

} // namespace game
} // namespace base