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

class Entity {
public:
    NEGINE_API ~Entity();
    NEGINE_API bool has(CompType type) const;
    NEGINE_API ComponentBase* get(CompType type) const;
    NEGINE_API void addChild(Entity* e);
    NEGINE_API void add(ComponentBase* c);

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