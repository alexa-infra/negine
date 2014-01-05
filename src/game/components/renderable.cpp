#include "renderable.h"
#include "game/scene.h"
#include "game/components/transform.h"

namespace base {
namespace game {

template<>
CompType Component<Renderable>::type_ = ComponentBase::registerType();

opengl::Model* Renderable::model() {
    return model_.resourceAs<opengl::Model>();
}

math::Matrix4 Renderable::world() const {
    ComponentBase* c = get_parent(parent(), Transform::Type());
    Transform* tr = dynamic_cast<Transform*>(c);
    if (tr == nullptr)
        return math::Matrix4::Identity();
    return tr->world();
}

} // namespace game
} // namespace base