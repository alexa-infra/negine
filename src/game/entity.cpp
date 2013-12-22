#include "entity.h"

namespace base {
namespace game {

Entity::~Entity() {
    for(auto c: components_) {
        delete c.second;
    }
}

bool Entity::has(CompType type) const {
    return mask_.test(type);
}

ComponentBase* Entity::get(CompType type) const {
    CompMap::const_iterator it = components_.find(type);
    return it->second;
}

void Entity::addChild(Entity* e) {
    e->parent_ = this;
    children_.push_back(e);
}

void Entity::add(ComponentBase* c) {
    mask_.set(c->type());
    c->parent_ = this;
    components_[c->type()] = c;
}

} // namespace game
} // namespace base