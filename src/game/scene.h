#pragma once

#include "game/entity.h"

namespace base {
namespace game {

CompList NEGINE_API find_children(Entity* root, CompType type);
CompList NEGINE_API find_all(Entity* root, CompType type);
CompList NEGINE_API find_siblings(Entity* root, CompType type);
CompList NEGINE_API find_parent(Entity* item, CompType type);
ComponentBase* NEGINE_API get_parent(Entity* item, CompType type);
ComponentBase* NEGINE_API get_component(Entity* item, CompType type);

} // namespace game
} // namespace base