/**
 * \file
 * \brief       Camera class, provides projection and modelview matrix
 * \author      Alexey Vasilyev <alexa.infra@gmail.com>
 * \copyright   MIT License
 **/
#pragma once

#include "base/types.h"
#include "base/math/vector.h"
#include "base/math/matrix.h"
#include "base/math/plane.h"

namespace base {
namespace opengl {

using base::math::Matrix4;
using base::math::Vector3;
using base::math::Plane;

class Camera {
private:
    Plane planes_[6];
    Matrix4 projection_;
    Matrix4 modelview_;
    Matrix4 clip_;

    Vector3 forward_;
    Vector3 right_;
    Vector3 up_;
    
    Vector3 position_;
    
    f32 pitch_;
    f32 head_;
        
    f32 aspect_;
    f32 fov_;
    f32 zNear_;
    f32 zFar_;
public:
    Camera();

    const Vector3& forward() const { return forward_; }
    const Vector3& right() const { return right_; }
    const Vector3& up() const{ return up_; }

    void set_position(const Vector3& v) { position_ = v; }
    const Vector3& position() const { return position_; }
    
    void set_pitch(f32 v) { pitch_ = v; }
    f32 pitch() const { return pitch_; }
    void set_head(f32 v) { head_ = v; }
    f32 head() const { return head_; }
        
    void set_aspect(f32 v) { aspect_ = v; }
    f32 aspect() const { return aspect_; }
    
    void set_fov(f32 v) { fov_ = v; };
    f32 fov() const { return fov_; }

    void set_zNear(f32 v) { zNear_ = v; }
    f32 zNear() const { return zNear_; }

    void set_zFar(f32 v) { zFar_ = v; }
    f32 zFar() const { return zFar_; }

    const Plane* planes() const { return planes_; }

    const Matrix4& GetProjection() const {
        return projection_;
    }
    const Matrix4& GetModelView() const {
        return modelview_;
    }
    const Matrix4& GetClipMatrix() const {
        return clip_;
    }

    bool PointIsInFrustum(const Vector3& point) const;
    bool BoxIsInFrustumFast(const Vector3& a, const Vector3& b) const;
    bool BoxIsInFrustumFull(const Vector3& a, const Vector3& b) const;

    void Update();
private:
    void UpdateOrientation();
    void UpdateFrustum();

    Matrix4 Projection() const;
    Matrix4 ModelView() const;

    void UpdateVectors(const Matrix4& orient);
    static void ConvertBoundingBox(const Vector3& min_point, const Vector3& max_point, const Matrix4& m, Vector3* points);
private:
    DISALLOW_COPY_AND_ASSIGN(Camera);
};

}
}
