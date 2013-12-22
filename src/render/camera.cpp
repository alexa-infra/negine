/**
 * \file
 * \author      Alexey Vasilyev <alexa.infra@gmail.com>
 * \copyright   MIT License
 **/
#include "render/camera.h"
#include "math/matrix-inl.h"

namespace base
{

using namespace math;

namespace opengl
{

Camera::Camera()
{
    dirty_ = false;
}

void Camera::Update()
{
    if (!dirty_)
        return;
    dirty_ = false;

    UpdateOrientation();

    projection_ = Matrix4::Perspective( fov_, aspect_, zNear_, zFar_ );
    modelview_ = Matrix4::LookAt( position_, position_ + forward_, up_ );
    clip_ = projection_ * modelview_;

    UpdateFrustum();
}

void Camera::UpdateOrientation()
{
    Matrix4 headRot  = Matrix4::RotationY( head_ );
    Matrix4 pitchRot = Matrix4::RotationX( pitch_ );
    Matrix4 orientation = headRot * pitchRot;

    const vec3f r( 1, 0, 0 );
    const vec3f u( 0, 1, 0 );
    const vec3f f( 0, 0, -1 );

    right_   = ( orientation * r ).xyz();
    up_      = ( orientation * u ).xyz();
    forward_ = ( orientation * f ).xyz();
}

void Camera::UpdateFrustum()
{
    const Matrix4& m = clip_;
    const math::vec4f v1 = m.Row( 0 );
    const math::vec4f v2 = m.Row( 1 );
    const math::vec4f v3 = m.Row( 2 );
    const math::vec4f v4 = m.Row( 3 );
    planes_[0].set( v4 + v1 );
    planes_[1].set( v4 - v1 );
    planes_[2].set( v4 + v2 );
    planes_[3].set( v4 - v2 );
    planes_[4].set( v4 + v3 );
    planes_[5].set( v4 - v3 );
}

void Camera::moveForward(f32 dist) {
    position_ += forward_ * dist;
    dirty_ = true;
}

void Camera::moveBackward(f32 dist) {
    position_ -= forward_ * dist;
    dirty_ = true;
}

void Camera::moveRight(f32 dist) {
    position_ += right_ * dist;
    dirty_ = true;
}

void Camera::moveLeft(f32 dist) {
    position_ -= right_ * dist;
    dirty_ = true;
}

void Camera::setPosition( const math::vec3f& v ) {
    position_ = v;
    dirty_ = true;
}

void Camera::setPitch( f32 v ) { 
    pitch_ = v;
    dirty_ = true;
}

void Camera::setHead( f32 v ) {
    head_ = v;
    dirty_ = true;
}

void Camera::turnPitch(f32 v) {
    pitch_ += v;
    dirty_ = true;
}

void Camera::turnHead(f32 v) {
    head_ += v;
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


} // namespace opengl
} // namespace base
