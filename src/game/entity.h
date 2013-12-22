#pragma once

#include "game/componentbase.h"
#include <bitset>
#include <map>
#include <deque>

namespace base {
namespace game {

typedef std::bitset<20> CompMask;

class Entity;
typedef std::deque<ComponentBase*> CompList;
typedef std::map<CompType, ComponentBase*> CompMap;
typedef std::deque<Entity*> EntityList;

class NEGINE_API Entity {
public:
    ~Entity();
    bool has(CompType type) const;
    ComponentBase* get(CompType type) const;
    void addChild(Entity* e);
    void add(ComponentBase* c);

    inline Entity* parent() const { return parent_; }
    inline const EntityList& children() const { return children_; }
private:
    Entity* parent_;
    EntityList children_;
    CompMap components_;
    CompMask mask_;
};


} // namespace game
} // namespace base