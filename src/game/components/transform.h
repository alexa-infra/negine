#pragma once

#include "game/component.h"
#include "math/matrix.h"
#include "game/scene.h"

namespace base {
namespace game {

class Transform : public Component<Transform>
{
public:
    Transform();

    static void updateTree(Entity* root);

    inline const math::vec3f& forward() const { return forward_; }
    inline const math::vec3f& right() const { return right_; }
    inline const math::vec3f& up() const { return up_; }

    inline const math::vec3f& position() const { return position_; }
    inline f32 pitch() const { return pitch_; }
    inline f32 head() const { return head_; }

    inline const math::Matrix4& world() const { return world_; }

    void moveForward(f32 dist);
    void moveBackward(f32 dist);
    void moveRight(f32 dist);
    void moveLeft(f32 dist);
    void turnPitch(f32 radians);
    void turnHead(f32 radians);

    void setPosition(const math::vec3f& v );
    void setPitch(f32 radians);
    void setHead(f32 radians);
private:
    void update();
    void makeDirty();
private:
    bool dirty_;
    math::Matrix4 world_;
    f32 pitch_, head_;
    math::vec3f forward_, right_, up_;
    math::vec3f position_;
};

} // namespace game
} // namespace base