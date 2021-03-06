#pragma once

#include "game/componentbase.h"
#include "math/matrix.h"
#include "game/scene.h"

namespace base {
namespace game {

class Transform : public ComponentBase
{
public:
    NEGINE_API Transform();
    NEGINE_API ~Transform();

    inline const math::vec3f& forward() const { return forward_; }
    inline const math::vec3f& right() const { return right_; }
    inline const math::vec3f& up() const { return up_; }

    inline const math::vec3f& position() const { return position_; }
    inline f32 pitch() const { return pitch_; }
    inline f32 head() const { return head_; }

    inline const math::Matrix4& world() const { return world_; }

    NEGINE_API void moveForward(f32 dist);
    NEGINE_API void moveBackward(f32 dist);
    NEGINE_API void moveRight(f32 dist);
    NEGINE_API void moveLeft(f32 dist);
    NEGINE_API void turnPitch(f32 radians);
    NEGINE_API void turnHead(f32 radians);

    NEGINE_API void setPosition(const math::vec3f& v );
    NEGINE_API void setPitch(f32 radians);
    NEGINE_API void setHead(f32 radians);

    static const char* extension() { return ".transform"; }

    NEGINE_API void setParent(Transform* parent);
    NEGINE_API void update();

    Signal signal_;
private:
    math::Matrix4 world_;
    f32 pitch_, head_;
    math::vec3f forward_, right_, up_;
    math::vec3f position_;
    Transform* parentTransform_;
};

} // namespace game
} // namespace base