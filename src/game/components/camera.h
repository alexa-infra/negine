#pragma once

#include "game/component.h"
#include "math/matrix.h"
#include "math/plane.h"

namespace base {
namespace game {

class Camera : public Component<Camera>
{
public:
    Camera();

    inline f32 aspect() const { return aspect_; }
    inline f32 fov() const { return fov_; }
    inline f32 zNear() const { return zNear_; }
    inline f32 zFar() const { return zFar_; }

    inline const math::Plane* planes() const { return planes_; }
    inline const math::Matrix4& projection() const { return projection_; }
    inline const math::Matrix4& modelView() const { return modelview_; }
    inline const math::Matrix4& clipMatrix() const { return clip_; }

    void setPerspective(f32 aspect, f32 fov, f32 zNear, f32 zFar);
    void setAspect(f32 ratio);
    void setFov(f32 radians);
    void setZNear(f32 dist);
    void setZFar(f32 dist);
private:
    void update();

    bool dirty_;
    math::Plane planes_[6];
    math::Matrix4 projection_;
    math::Matrix4 modelview_;
    math::Matrix4 clip_;
    math::Matrix4 world_;

    f32 aspect_;
    f32 fov_;
    f32 zNear_;
    f32 zFar_;
};

} // namespace game
} // namespace base