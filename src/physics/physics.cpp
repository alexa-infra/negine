#include "physics/physics.h"
#include <btBulletDynamicsCommon.h>
#include "game/components/transform.h"

#include <iostream>

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

void Physics::simulate(f32 dt)
{
    const f32 timeStep = 1 / 60.f;
    int numSteps = (int)(dt / timeStep) + 1;
    world_->stepSimulation(dt, numSteps);
}

class MotionState : public btMotionState {
public:
    MotionState(const math::vec3f& pos) : transform_(nullptr) {
        position_.setIdentity();
        position_.setOrigin(btVector3(pos.x, pos.y, pos.z));
    }

    ~MotionState() {
    }

    void getWorldTransform(btTransform& worldTrans) const {
        worldTrans = position_;
    }

    void setWorldTransform(const btTransform& worldTrans) {
        position_ = worldTrans;

        if (transform_ != nullptr) {
            const btVector3& pos = worldTrans.getOrigin();
            transform_->setPosition(math::vec3f(pos.x(), pos.y(), pos.z()));
            transform_->update();
        }
    }

    void setTransform(game::Transform* t) {
        transform_ = t;
    }
protected:
    btTransform position_;
    game::Transform* transform_;
};

Body::Body(Physics& phys, Shape shape, f32 mass, const math::vec3f& pos)
: physics_(phys)
, transfrom_(nullptr)
{
    if (shape == Sphere)
        collisionShape_ = new btSphereShape(1);
    else if (shape == Plane)
        collisionShape_ = new btStaticPlaneShape(btVector3(0, 1, 0), 1);

    motionState_ = new MotionState(pos);

    btVector3 inertia(0, 0, 0);
    if (mass != 0.0f) {
        collisionShape_->calculateLocalInertia(mass, inertia);
    }

    btRigidBody::btRigidBodyConstructionInfo rigidBodyCI(mass, motionState_, collisionShape_, inertia);
    rigidBody_ = new btRigidBody(rigidBodyCI);
    physics_.world_->addRigidBody(rigidBody_);
}

Body::~Body()
{
    physics_.world_->removeRigidBody(rigidBody_);
    delete rigidBody_->getMotionState();
    delete rigidBody_;
    delete collisionShape_;
}

void Body::setTransform(game::Transform* t) {
    transfrom_ = t;
    motionState_->setTransform(t);
}

}
}