/**
 * \file
 * \brief       Camera class, provides projection and modelview matrix
 * \author      Alexey Vasilyev <alexa.infra@gmail.com>
 * \copyright   MIT License
 **/
#pragma once

#include "base/types.h"
#include "math/vec3.h"
#include "math/matrix.h"
#include "math/plane.h"

namespace base
{
namespace opengl
{

class NEGINE_API Camera
{
public:
    Camera();

    inline const math::vec3f& forward() const { return forward_; }
    inline const math::vec3f& right() const { return right_; }
    inline const math::vec3f& up() const { return up_; }

    inline const math::vec3f& position() const { return position_; }
    inline f32 pitch() const { return pitch_; }
    inline f32 head() const { return head_; }
    inline f32 aspect() const { return aspect_; }
    inline f32 fov() const { return fov_; }
    inline f32 zNear() const { return zNear_; }
    inline f32 zFar() const { return zFar_; }

    inline const math::Plane* planes() const { return planes_; }
    inline const math::Matrix4& projection() const { return projection_; }
    inline const math::Matrix4& modelView() const { return modelview_; }
    inline const math::Matrix4& clipMatrix() const { return clip_; }

    void moveForward(f32 dist);
    void moveBackward(f32 dist);
    void moveRight(f32 dist);
    void moveLeft(f32 dist);
    void turnPitch(f32 radians);
    void turnHead(f32 radians);

    void setPerspective(f32 aspect, f32 fov, f32 zNear, f32 zFar);
    void setPosition(const math::vec3f& v );
    void setPitch(f32 radians);
    void setHead(f32 radians);
    void setAspect(f32 ratio);
    void setFov(f32 radians);
    void setZNear(f32 dist);
    void setZFar(f32 dist);

    void Update();
private:
    void UpdateOrientation();
    void UpdateFrustum();

    bool dirty_;
    math::Plane planes_[6];
    math::Matrix4 projection_;
    math::Matrix4 modelview_;
    math::Matrix4 clip_;

    math::vec3f forward_;
    math::vec3f right_;
    math::vec3f up_;

    math::vec3f position_;

    f32 pitch_;
    f32 head_;

    f32 aspect_;
    f32 fov_;
    f32 zNear_;
    f32 zFar_;

private:
    DISALLOW_COPY_AND_ASSIGN( Camera );
};

}
}
