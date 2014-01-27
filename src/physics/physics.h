#pragma once

#include "base/types.h"
#include "math/vec3.h"

class btBroadphaseInterface;
class btDefaultCollisionConfiguration;
class btCollisionDispatcher;
class btConstraintSolver;
class btDynamicsWorld;

class btCollisionShape;
class btRigidBody;

namespace base {
namespace game {
    class Transform;
}
}

namespace base {
namespace phys {

class Physics {
    friend class Body;
public:
    Physics();
    ~Physics();

    NEGINE_API void simulate(f32 dt);
private:
    btBroadphaseInterface* broadphase_;
    btDefaultCollisionConfiguration* collisionConfiguration_;
    btCollisionDispatcher* collisionDispatcher_;
    btConstraintSolver* solver_;
    btDynamicsWorld* world_;
};

class MotionState;

class Body {
public:
    enum Shape { Sphere, Plane };
    NEGINE_API Body(Physics& phys, Shape shape, f32 mass, const math::vec3f& pos);
    NEGINE_API ~Body();

    NEGINE_API void setTransform(game::Transform* t);
private:
    Physics& physics_;
    MotionState* motionState_;
    btRigidBody* rigidBody_;
    btCollisionShape* collisionShape_;
    game::Transform* transfrom_;
};

}
}