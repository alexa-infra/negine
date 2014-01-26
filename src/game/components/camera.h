#pragma once

#include "game/componentbase.h"
#include "math/matrix.h"
#include "math/plane.h"

namespace base {
namespace game {

class Transform;

class Camera : public ComponentBase
{
public:
    NEGINE_API Camera();
    NEGINE_API ~Camera();

    inline f32 aspect() const { return aspect_; }
    inline f32 fov() const { return fov_; }
    inline f32 zNear() const { return zNear_; }
    inline f32 zFar() const { return zFar_; }

    inline const math::Plane* planes() const { return planes_; }
    inline const math::Matrix4& projection() const { return projection_; }
    inline const math::Matrix4& modelView() const { return modelview_; }
    inline const math::Matrix4& clipMatrix() const { return clip_; }

    NEGINE_API void setPerspective(f32 aspect, f32 fov, f32 zNear, f32 zFar);
    NEGINE_API void setAspect(f32 ratio);
    NEGINE_API void setFov(f32 radians);
    NEGINE_API void setZNear(f32 dist);
    NEGINE_API void setZFar(f32 dist);

    const char* extension() const { return ".camera"; }

    NEGINE_API void update();
    NEGINE_API void setParent(Transform* transform);
private:
    Transform* parentTransfrom_;
    math::Plane planes_[6];
    math::Matrix4 projection_;
    math::Matrix4 modelview_;
    math::Matrix4 clip_;

    f32 aspect_;
    f32 fov_;
    f32 zNear_;
    f32 zFar_;
};

} // namespace game
} // namespace base