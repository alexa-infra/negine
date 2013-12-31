#pragma once

#include "game/component.h"
#include "engine/resourceref.h"
#include "render/model.h"
#include "math/matrix.h"

namespace base {
namespace game {

class Renderable : public Component<Renderable>
{
public:
    ResourceRef model_;
    opengl::Model* model();
    math::Matrix4 world() const;
};

} // namespace game
} // namespace base