/**
 * \file
 * \author      Alexey Vasilyev <alexa.infra@gmail.com>
 * \copyright   MIT License
 **/
#include "renderer/camera.h"

namespace base {
namespace opengl {

Camera::Camera()
{

}

void Camera::Update()
{
    projection_ = Projection();
    UpdateOrientation();
    modelview_ = ModelView();
    clip_ = projection_ * modelview_;
    UpdateFrustum();
}

Matrix4 Camera::Projection() const {
    Matrix4 projection = Matrix4::GetPerspective(fov_, aspect_, zNear_, zFar_);
    return projection;
}

Matrix4 Camera::ModelView() const {
    Vector3 lookAt = position_ + forward_;

    Matrix4 modelView = Matrix4::LookAt(position_, lookAt, up_);
    return modelView; 
}

void Camera::UpdateOrientation()
{
    Matrix4 headRot  = Matrix4::GetRotationY(head_);
    Matrix4 pitchRot = Matrix4::GetRotationX(pitch_);
    Matrix4 orientation = pitchRot * headRot;
    UpdateVectors(orientation);
}

void Camera::UpdateVectors(const Matrix4& orient)
{
    const Vector3 r(1, 0, 0); 
    const Vector3 u(0, 1, 0);
    const Vector3 f(0, 0, 1);

    right_   = orient * r;
    up_      = orient * u;
    forward_ = orient * f;
}

void Camera::UpdateFrustum()
{
    const Matrix4& m = clip_;

    // right
    planes_[0].set(m[3] - m[0], m[7] - m[4], m[11] - m[8],  m[15] - m[12]);
    // left
    planes_[1].set(m[3] + m[0], m[7] + m[4], m[11] + m[8],  m[15] + m[12]);
    // bottom
    planes_[2].set(m[3] + m[1], m[7] + m[5], m[11] + m[9],  m[15] + m[13]);
    // top
    planes_[3].set(m[3] - m[1], m[7] - m[5], m[11] - m[9],  m[15] - m[13]);
    // far
    planes_[4].set(m[3] - m[2], m[7] - m[6], m[11] - m[10], m[15] - m[14]);
    // near
    planes_[5].set(m[3] + m[2], m[7] + m[6], m[11] + m[10], m[15] + m[14]);
}

void Camera::ConvertBoundingBox(const Vector3& min_point, const Vector3& max_point, const Matrix4& m, Vector3* points)
{
    points[0] = m*Vector3(min_point.x, min_point.y, min_point.z);
    points[1] = m*Vector3(max_point.x, min_point.y, min_point.z);
    points[2] = m*Vector3(max_point.x, max_point.y, min_point.z);
    points[3] = m*Vector3(max_point.x, min_point.y, max_point.z);
    points[4] = m*Vector3(max_point.x, max_point.y, max_point.z);
    points[5] = m*Vector3(min_point.x, max_point.y, min_point.z);
    points[6] = m*Vector3(min_point.x, max_point.y, max_point.z);
    points[7] = m*Vector3(min_point.x, min_point.y, max_point.z);
}

// TODO: fix me
bool Camera::PointIsInFrustum(const Vector3& point) const
{
    for(u32 i=0; i<6; i++) {
        if (planes_[i].Distance(point) <= 0.f)
            return false;
    }
    return true;
}

bool Camera::BoxIsInFrustumFull(const Vector3& mmin, const Vector3& mmax) const
{
    const Matrix4& m = GetClipMatrix();
    Vector3 bbox[8];
    ConvertBoundingBox(mmin, mmax, m, bbox);
    for (u32 i=0; i<8; i++) {
        if (PointIsInFrustum(bbox[i]))
            return true;
    }
    return false;
}

// TODO: Fast and buggy
bool Camera::BoxIsInFrustumFast(const Vector3& mmin, const Vector3& mmax) const
{
    const Matrix4& m = GetClipMatrix();
    Vector3 bbox[8];
    ConvertBoundingBox(mmin, mmax, m, bbox);
    const Vector3 axis1 = bbox[1] - bbox[0];
    const Vector3 axis2 = bbox[5] - bbox[0];
    const Vector3 axis3 = bbox[7] - bbox[0];
    bool intersect = false;
    for(int i=0; i<6; i++)
    {
        const Vector3& n = planes_[i].Normal();
        const Vector3 nb = Vector3(Dot(n, axis1), Dot(n, axis2), Dot(n, axis3));
        Vector3 nn, np;
        if (nb.x < 0)
        {
            if (nb.y < 0)
            {
                if (nb.z < 0)
                {
                    np = bbox[0];
                    nn = bbox[4];
                }
                else
                {
                    np = bbox[7];
                    nn = bbox[2];
                }
            }
            else
            {
                if (nb.z < 0)
                {
                    np = bbox[5];
                    nn = bbox[3];
                }
                else
                {
                    np = bbox[6];
                    nn = bbox[1];
                }
            }
        }
        else
        {
            if (nb.y < 0)
            {
                if (nb.z < 0)
                {
                    np = bbox[1];
                    nn = bbox[6];
                }
                else
                {
                    np = bbox[3];
                    nn = bbox[5];
                }
            }
            else
            {
                if (nb.z < 0)
                {
                    np = bbox[2];
                    nn = bbox[7];
                }
                else
                {
                    np = bbox[4];
                    nn = bbox[0];
                }
            }
        }
        if (planes_[i].Distance(nn) > 0)
            return false;
        if (planes_[i].Distance(np) > 0)
            return true;
    }
    //if (!intersect) then inside!!
    return true;
}

} // namespace opengl
} // namespace base
