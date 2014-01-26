#include "renderable.h"
#include "game/scene.h"
#include "game/components/transform.h"
#include "math/matrix-inl.h"

namespace base {
namespace game {

opengl::Model* Renderable::model() {
    return model_.resourceAs<opengl::Model>();
}

math::Matrix4 Renderable::world() const {
    Transform* tr = scene_->getTyped<Transform>(name_);
    if (tr == nullptr)
        return math::Matrix4::Identity();
    return tr->world();
}

} // namespace game
} // namespace base