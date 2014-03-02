#include "scene.h"
#include "foundation/murmur_hash.h"

namespace base {
namespace game {

u64 HashStr::operator()(const std::string& s) const {
    return foundation::murmur_hash_64(s.c_str(), static_cast<u32>(s.length()), seed_);
}

Scene::Scene()
: componentByName_(foundation::memory_globals::default_allocator())
, componentByObject_(foundation::memory_globals::default_allocator())
, transforms_(foundation::memory_globals::default_allocator())
, cameras_(foundation::memory_globals::default_allocator())
, renderables_(foundation::memory_globals::default_allocator())
, hash_(123)
{
}

Scene::~Scene() {
}

void Scene::attachNamed(const std::string& name, const std::string& fullname, ComponentBase* aspect) {
    aspect->setName(name);
    aspect->setScene(this);
    foundation::hash::set(componentByName_, hash_(fullname), aspect);
    foundation::multi_hash::insert(componentByObject_, hash_(name), aspect);
}

void Scene::detachNamed(const std::string& name, const std::string& fullname, ComponentBase* aspect) {
    aspect->setScene(nullptr);
    foundation::hash::remove(componentByName_, hash_(fullname));
    const foundation::Hash<ComponentBase*>::Entry* it = foundation::multi_hash::find_first(componentByObject_, hash_(name));
    while (it != nullptr) {
        if (it->value == aspect) {
            foundation::multi_hash::remove(componentByObject_, it);
            break;
        }
        it = foundation::multi_hash::find_next(componentByObject_, it);
    }
}

} // namespace game
} // namespace base