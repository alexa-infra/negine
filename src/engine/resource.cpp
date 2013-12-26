#include "resource.h"
#include "base/debug.h"

namespace base {

ResourceManager* ResourceManager::instance_ = nullptr;

ResourceManager& ResourceManager::instance() {
    if (instance_ == nullptr)
        instance_ = new ResourceManager;
    return *instance_;
}

ResourceManager::ResourceManager() {
    typeCounter_ = 0;
}

Resource* ResourceManager::get(std::size_t uri) {
    return instance().selfGet(uri);
}

void ResourceManager::set(std::size_t uri, Resource* res) {
    instance().selfSet(uri, res);
}

void ResourceManager::destroy(std::size_t uri) {
    instance().selfDestroy(uri);
}

void ResourceManager::shutdown() {
    ASSERT(instance_ != nullptr);
    if (instance_ != nullptr)
        delete instance_;
    instance_ = nullptr;
}

u32 ResourceManager::registerResource() {
    return ++instance().typeCounter_;
}

void ResourceManager::addFactory(u32 type, ResourceFactoryFunc factory) {
    ResourceManager& manager = instance();
    FactoryMap& factories = manager.factories_;
    ASSERT(factories.find(type) == factories.end());
    factories[type] = factory;
}

Resource* ResourceManager::loadDefault(u32 type, std::size_t uri, const std::string& path) {
    ResourceManager& manager = instance();
    FactoryMap& factories = manager.factories_;
    FactoryMap::const_iterator it = factories.find(type);
    ASSERT(it != factories.end());
    ResourceFactory factory = it->second;
    Resource* resource = factory(path);
    set(uri, resource);
    return resource;
}

Resource* ResourceManager::selfGet(std::size_t uri) {
    ResourceMap::const_iterator it = resources_.find(uri);
    ASSERT(it != resources_.end());
    return it->second;
}

void ResourceManager::selfSet(std::size_t uri, Resource* res) {
    ASSERT(resources_.find(uri) == resources_.end());
    resources_[uri] = res;
}

void ResourceManager::selfDestroy(std::size_t uri) {
    ResourceMap::iterator it = resources_.find(uri);
    ASSERT(it != resources_.end());
    delete it->second;
    resources_.erase(it);
}


} // namespace base