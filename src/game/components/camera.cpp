#include "camera.h"
#include "transform.h"
#include "math/matrix-inl.h"

namespace base {
namespace game {

using namespace math;

Camera::Camera() : parentTransfrom_(nullptr) {
}

Camera::~Camera() {
    setParent(nullptr);
}

void Camera::setPerspective(f32 aspect, f32 fov, f32 zNear, f32 zFar) {
    aspect_ = aspect;
    fov_ = fov;
    zNear_ = zNear;
    zFar_ = zFar;
}

void Camera::setAspect( f32 v ) {
    aspect_ = v;
}

void Camera::setFov( f32 v ) {
    fov_ = v;
};

void Camera::setZNear( f32 v ) {
    zNear_ = v;
}
    
void Camera::setZFar( f32 v ) {
    zFar_ = v;
}

void Camera::setParent(Transform* transform) {
    if (parentTransfrom_ != nullptr)
        parentTransfrom_->signal_.disconnect(name_ + Transform::extension());
    parentTransfrom_ = transform;
    if (parentTransfrom_ != nullptr)
        parentTransfrom_->signal_.connect(name_ + Transform::extension(), std::bind(&Camera::update, this));
}

void Camera::update() {
    if (parentTransfrom_ == nullptr) {
        Transform* transform = scene_->getTyped<Transform>(name_);
        if (transform == nullptr)
            return;
        setParent(transform);
    }

    const math::Matrix4& newWorld = parentTransfrom_->world();

    projection_ = Matrix4::Perspective( fov_, aspect_, zNear_, zFar_ );
    modelview_ = OrthoInverse(newWorld);
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

}
}