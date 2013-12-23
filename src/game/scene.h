#pragma once

#include "game/entity.h"

namespace base {
namespace game {

NEGINE_API CompList find_children(Entity* root, CompType type);
NEGINE_API CompList find_all(Entity* root, CompType type);
NEGINE_API CompList find_siblings(Entity* root, CompType type);
NEGINE_API CompList find_parent(Entity* item, CompType type);
NEGINE_API ComponentBase* get_parent(Entity* item, CompType type);
NEGINE_API ComponentBase* get_component(Entity* item, CompType type);

} // namespace game
} // namespace base