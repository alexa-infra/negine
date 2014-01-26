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

} // namespace game
} // namespace base