#include "resourceref.h"

namespace base {

ResourceRef::ResourceRef() {
    hash_ = 0;
}

ResourceRef::ResourceRef(const std::string& uri) {
    std::hash<std::string> h;
    hash_ = h(uri);
}

ResourceRef::ResourceRef(const std::string& uri, Resource* res) {
    std::hash<std::string> h;
    hash_ = h(uri);
    setResource(res);
}

ResourceRef& ResourceRef::operator=(const ResourceRef& r) {
    hash_ = r.hash_;
    return *this;
}

Resource* ResourceRef::resource() {
    return ResourceManager::get(hash_);
}

void ResourceRef::setResource(Resource* res) {
    ResourceManager::set(hash_, res);
}

void ResourceRef::destroy() {
    ResourceManager::destroy(hash_);
}

} // namespace base