#include "transform.h"

namespace base {
namespace game {

using namespace math;

CompType Transform::type_ = ComponentBase::registerType();

Transform::Transform() {
    world_ = Matrix4::Identity();
    dirty_ = true;
}

void Transform::updateTree(Entity* root) {
    CompList components = find_all(root, Type());
    for(auto c: components) {
        Transform* tr = dynamic_cast<Transform*>(c);
        if (tr->dirty_)
            tr->update();
    }
}

void Transform::update() {
    if (!dirty_)
        return;

    Matrix4 headRot  = Matrix4::RotationY( head_ );
    Matrix4 pitchRot = Matrix4::RotationX( pitch_ );
    Matrix4 orientation = headRot * pitchRot;

    const vec3f r( 1, 0, 0 );
    const vec3f u( 0, 1, 0 );
    const vec3f f( 0, 0, -1 );

    right_   = ( orientation * r ).xyz();
    up_      = ( orientation * u ).xyz();
    forward_ = ( orientation * f ).xyz();

    Matrix4 local = Matrix4::Translation( position_ ) * orientation;

    ComponentBase* parent = get_parent(parent_->parent(), type());
    if (parent == nullptr) {
        world_ = local;
        return;
    }

    Transform* parentTransform = dynamic_cast<Transform*>(parent);
    world_ = parentTransform->world_ * local;
    dirty_ = false; 
}

void Transform::makeDirty() {
    if (dirty_)
        return;
    dirty_ = true;
    CompList components = find_children(parent_, Type());
    for(auto c: components) {
        Transform* tr = dynamic_cast<Transform*>(c);
        tr->dirty_ = true;
    }
}

void Transform::moveForward(f32 dist) {
    position_ += forward_ * dist;
    makeDirty();
}

void Transform::moveBackward(f32 dist) {
    position_ -= forward_ * dist;
    makeDirty();
}

void Transform::moveRight(f32 dist) {
    position_ += right_ * dist;
    makeDirty();
}

void Transform::moveLeft(f32 dist) {
    position_ -= right_ * dist;
    makeDirty();
}

void Transform::turnPitch(f32 v) {
    pitch_ += v;
    makeDirty();
}

void Transform::turnHead(f32 v) {
    head_ += v;
    makeDirty();
}

void Transform::setPosition( const vec3f& v ) {
    position_ = v;
    makeDirty();
}

void Transform::setPitch( f32 v ) { 
    pitch_ = v;
    makeDirty();
}

void Transform::setHead( f32 v ) {
    head_ = v;
    makeDirty();
}

} // namespace game
} // namespace base