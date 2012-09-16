/**
 * \file
 * \author      Alexey Vasilyev <alexa.infra@gmail.com>
 * \copyright   MIT License
 **/
#include "renderer/camera.h"
#include "base/math/vector.h"
#include <cassert>

namespace base
{

using namespace math;

namespace opengl
{

Camera::Camera()
{
}

void Camera::Update()
{
    projection_ = Matrix4::Perspective( fov_, aspect_, zNear_, zFar_ );
    UpdateOrientation();
    modelview_ = Matrix4::LookAt( position_, position_ + forward_, up_ );
    clip_ = projection_ * modelview_;
//    clip_ = modelview_ * projection_;
    UpdateFrustum();
}

Matrix4 Camera::Projection() const
{
    return projection_;
}

Matrix4 Camera::ModelView() const
{
    return modelview_;
}

void Camera::UpdateOrientation()
{
    Matrix4 headRot  = Matrix4::RotationZ( head_ );
    Matrix4 pitchRot = Matrix4::RotationX( pitch_ );
    Matrix4 orientation = pitchRot * headRot;
    UpdateVectors( orientation );
}

void Camera::UpdateVectors( const Matrix4& orient )
{
    const Vector3 r( 1, 0, 0 );
    const Vector3 u( 0, 0, 1 );
    const Vector3 f( 0, -1, 0 );
    right_   = Vector3( orient * r );
    up_      = Vector3( orient * u );
    forward_ = Vector3( orient * f );
}

void Camera::UpdateFrustum()
{
    const Matrix4& m = clip_;
    const math::Vector4 v1 = m.Row( 0 );
    const math::Vector4 v2 = m.Row( 1 );
    const math::Vector4 v3 = m.Row( 2 );
    const math::Vector4 v4 = m.Row( 3 );
    planes_[0].set( v4 + v1 );
    planes_[1].set( v4 - v1 );
    planes_[2].set( v4 + v2 );
    planes_[3].set( v4 - v2 );
    planes_[4].set( v4 + v3 );
    planes_[5].set( v4 - v3 );
}

void Camera::ConvertBoundingBox( const Vector3& min_point, const Vector3& max_point, const Matrix4& m, Vector3* points )
{
}

// TODO: fix me
bool Camera::PointIsInFrustum( const Vector3& point ) const
{
    return true;
}

bool Camera::BoxIsInFrustumFull( const Vector3& mmin, const Vector3& mmax ) const
{
    return false;
}

// TODO: Fast and buggy
bool Camera::BoxIsInFrustumFast( const Vector3& mmin, const Vector3& mmax ) const
{
    return true;
}

} // namespace opengl
} // namespace base
