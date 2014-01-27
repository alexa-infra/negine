#pragma once

class btBroadphaseInterface;
class btDefaultCollisionConfiguration;
class btCollisionDispatcher;
class btConstraintSolver;
class btDynamicsWorld;

namespace base {
namespace phys {

class Physics {
public:
    Physics();
    ~Physics();
private:
    btBroadphaseInterface* broadphase_;
    btDefaultCollisionConfiguration* collisionConfiguration_;
    btCollisionDispatcher* collisionDispatcher_;
    btConstraintSolver* solver_;
    btDynamicsWorld* world_;
};

}
}