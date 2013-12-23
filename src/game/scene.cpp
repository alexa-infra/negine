#include "scene.h"
#include <queue>

namespace base {
namespace game {

CompList find_children(Entity* root, CompType type) {
    CompList result;

    std::queue<Entity*> current;
    current.push(root);
    while(!current.empty()) {
        Entity* cur = current.front();
        current.pop();
        for(auto child: cur->children()) {
            if (child->has(type))
                result.push_back(child->get(type));
            current.push(child);
        }
    }
    return result;
}

CompList find_all(Entity* root, CompType type) {
    CompList children = find_children(root, type);
    if (root->has(type))
        children.push_front(root->get(type));
    return children;
}

CompList find_siblings(Entity* root, CompType type) {
    CompList result;

    Entity* parent = root->parent();
    if (parent != nullptr) {
        for(auto sibling: parent->children()) {
            if (sibling != root && sibling->has(type))
                result.push_back(sibling->get(type));
        }
    }
    return result;
}

CompList find_parent(Entity* item, CompType type) {
    CompList result;
    Entity* it = item;
    while(it != nullptr) {
        if (it->has(type))
            result.push_back(it->get(type));
        it = it->parent();
    }
    return result;
}

ComponentBase* get_parent(Entity* item, CompType type) {
    Entity* it = item;
    while(it != nullptr) {
        if (it->has(type))
            return it->get(type);
        it = it->parent();
    }
    return nullptr;
}

ComponentBase* get_component(Entity* item, CompType type) {
    if (item->has(type))
        return item->get(type);
    return nullptr;
}

} // namespace game
} // namespace base