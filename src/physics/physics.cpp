#include "physics/physics.h"
#include <btBulletDynamicsCommon.h>

namespace base {
namespace phys {

Physics::Physics()
{
    broadphase_ = new btDbvtBroadphase;
    collisionConfiguration_ = new btDefaultCollisionConfiguration;
    collisionDispatcher_ = new btCollisionDispatcher(collisionConfiguration_);
    solver_ = new btSequentialImpulseConstraintSolver;
    world_ = new btDiscreteDynamicsWorld(collisionDispatcher_, broadphase_, solver_, collisionConfiguration_);

    world_->setGravity(btVector3(0, -10, 0));
}

Physics::~Physics()
{
    delete world_;
    delete solver_;
    delete collisionDispatcher_;
    delete collisionConfiguration_;
    delete broadphase_;
}

}
}