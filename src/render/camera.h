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
private:
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
public:
    Camera();

    const math::vec3f& forward() const {
        return forward_;
    }
    const math::vec3f& right() const {
        return right_;
    }
    const math::vec3f& up() const {
        return up_;
    }

    void set_position( const math::vec3f& v ) {
        position_ = v;
    }
    const math::vec3f& position() const {
        return position_;
    }

    void set_pitch( f32 v ) {
        pitch_ = v;
    }
    f32 pitch() const {
        return pitch_;
    }
    void set_head( f32 v ) {
        head_ = v;
    }
    f32 head() const {
        return head_;
    }

    void set_aspect( f32 v ) {
        aspect_ = v;
    }
    f32 aspect() const {
        return aspect_;
    }

    void set_fov( f32 v ) {
        fov_ = v;
    };
    f32 fov() const {
        return fov_;
    }

    void set_zNear( f32 v ) {
        zNear_ = v;
    }
    f32 zNear() const {
        return zNear_;
    }

    void set_zFar( f32 v ) {
        zFar_ = v;
    }
    f32 zFar() const {
        return zFar_;
    }

    const math::Plane* planes() const {
        return planes_;
    }

    const math::Matrix4& GetProjection() const {
        return projection_;
    }
    const math::Matrix4& GetModelView() const {
        return modelview_;
    }
    const math::Matrix4& GetClipMatrix() const {
        return clip_;
    }

    void Update();
private:
    void UpdateOrientation();
    void UpdateFrustum();

private:
    DISALLOW_COPY_AND_ASSIGN( Camera );
};

}
}
