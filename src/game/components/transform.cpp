#include "transform.h"
#include "math/matrix-inl.h"
#include "game/scene.h"

namespace base {
namespace game {

using namespace math;

Transform::Transform() {
    world_ = Matrix4::Identity();
    pitch_ = head_ = 0;
    forward_ = math::vec3f(0, 0, -1);
    right_ = math::vec3f(1, 0, 0);
    up_ = math::vec3f(0, 1, 0);
    position_ = math::vec3f(0, 0, 0);
    parentTransform_ = nullptr;
}

Transform::~Transform() {
    setParent(nullptr);
}

void Transform::update() {
    Matrix4 headRot  = Matrix4::RotationY( head_ );
    Matrix4 pitchRot = Matrix4::RotationX( pitch_ );
    Matrix4 orientation = headRot * pitchRot;

    const vec3f r( 1, 0, 0 );
    const vec3f u( 0, 1, 0 );
    const vec3f f( 0, 0, -1 );

    right_   = ( orientation * r ).xyz();
    up_      = ( orientation * u ).xyz();
    forward_ = ( orientation * f ).xyz();

    Matrix4 local = Matrix4::Translation( position_ ) * orientation;

    if (parentTransform_ == nullptr) {
        world_ = local;
    } else {
        world_ = parentTransform_->world_ * local;
    }

    signal_.emit();
}

void Transform::moveForward(f32 dist) {
    position_ += forward_ * dist;
}

void Transform::moveBackward(f32 dist) {
    position_ -= forward_ * dist;
}

void Transform::moveRight(f32 dist) {
    position_ += right_ * dist;
}

void Transform::moveLeft(f32 dist) {
    position_ -= right_ * dist;
}

void Transform::turnPitch(f32 v) {
    pitch_ += v;
}

void Transform::turnHead(f32 v) {
    head_ += v;
}

void Transform::setPosition( const vec3f& v ) {
    position_ = v;
}

void Transform::setPitch( f32 v ) { 
    pitch_ = v;
}

void Transform::setHead( f32 v ) {
    head_ = v;
}

void Transform::setParent(Transform* parent) {
    if (parentTransform_ != nullptr)
        parentTransform_->signal_.disconnect(name_ + Transform::extension());
    parentTransform_ = parent;
    if (parentTransform_ != nullptr)
        parent->signal_.connect(name_ + Transform::extension(), std::bind(&Transform::update, this));
}

} // namespace game
} // namespace base