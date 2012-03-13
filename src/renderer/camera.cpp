#include "renderer/camera.h"

namespace base {
namespace opengl {

Camera::Camera()
{

}

Matrix4 Camera::GetProjection() const {
    Matrix4 projection = Matrix4::GetPerspective(fov, aspect, zNear, zFar);
    return projection;
}

Matrix4 Camera::GetModelView() const {
    Vector3 lookAt = position + forward;
    Matrix4 modelView = Matrix4::LookAt(position, lookAt, up);
    return modelView; 
}

void Camera::UpdateOrientation()
{
    Matrix4 headRot = Matrix4::GetRotationY(head);
    Matrix4 pitchRot = Matrix4::GetRotationX(pitch);
    orientation_ = headRot * pitchRot;
    UpdateVectors();
}

void Camera::UpdateVectors()
{
    const Vector3 r(1, 0, 0); 
    const Vector3 u(0, 1, 0);
    const Vector3 f(0, 0, 1);

    right = orientation_ * r;
    up = orientation_ * u;
    forward = orientation_ * f;
}

} // namespace opengl
} // namespace base
