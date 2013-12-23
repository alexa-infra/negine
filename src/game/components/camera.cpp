#include "camera.h"
#include "transform.h"
#include "math/matrix-inl.h"

namespace base {
namespace game {

using namespace math;

CompType Camera::type_ = ComponentBase::registerType();

Camera::Camera() {
    dirty_ = true;
}

void Camera::setPerspective(f32 aspect, f32 fov, f32 zNear, f32 zFar) {
    aspect_ = aspect;
    fov_ = fov;
    zNear_ = zNear;
    zFar_ = zFar;
    dirty_ = true;
}

void Camera::setAspect( f32 v ) {
    aspect_ = v;
    dirty_ = true;
}

void Camera::setFov( f32 v ) {
    fov_ = v;
    dirty_ = true;
};

void Camera::setZNear( f32 v ) {
    zNear_ = v;
    dirty_ = true;
}
    
void Camera::setZFar( f32 v ) {
    zFar_ = v;
    dirty_ = true;
}

void Camera::update() {
    ComponentBase* component = get_parent(parent_, Transform::Type());
    Transform* parentTransform = dynamic_cast<Transform*>(component);

    const math::Matrix4& newWorld = parentTransform->world();
    if (!dirty_ && newWorld == world_)
        return;
    dirty_ = false;
    world_ = newWorld;

    projection_ = Matrix4::Perspective( fov_, aspect_, zNear_, zFar_ );
    modelview_ = OrthoInverse(world_);
        //Matrix4::LookAt( position, position + parentTransform->forward(), parentTransform->up() );
    clip_ = projection_ * modelview_;

    const math::vec4f v1 = clip_.Row( 0 );
    const math::vec4f v2 = clip_.Row( 1 );
    const math::vec4f v3 = clip_.Row( 2 );
    const math::vec4f v4 = clip_.Row( 3 );
    planes_[0].set( v4 + v1 );
    planes_[1].set( v4 - v1 );
    planes_[2].set( v4 + v2 );
    planes_[3].set( v4 - v2 );
    planes_[4].set( v4 + v3 );
    planes_[5].set( v4 - v3 );
}

void Camera::updateTree(Entity* root) {
    CompList components = find_all(root, Type());
    for(auto c: components) {
        Camera* cam = dynamic_cast<Camera*>(c);
        cam->update();
    }
}

}
}