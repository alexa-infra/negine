#pragma once

#include "base/types.h"
#include <string>
#include <map>
#include <functional>

namespace base {
namespace opengl {

class Resource {
public:
    virtual ~Resource() {}
    virtual u32 type() const = 0;
};

template<typename T>
class BaseResource : public Resource {
public:
    virtual ~BaseResource() {}
    u32 type() const { return type_; }
    static u32 Type() { return type_; }
private:
    static u32 type_;
};

typedef Resource* (*ResourceFactoryFunc) (const std::string& name);
typedef std::function<Resource*(const std::string& name)> ResourceFactory;

class ResourceManager {
public:
    static Resource* get(std::size_t uri);
    static void set(std::size_t uri, Resource* res);
    static void destroy(std::size_t uri);

    static void init();
    static void shutdown();
    static u32 registerResource();
    static void addFactory(u32 type, ResourceFactoryFunc factory);

    static Resource* loadDefault(u32 type, std::size_t uri, const std::string& path);
private:
    static ResourceManager& instance();
    ResourceManager();

    Resource* selfGet(std::size_t uri);
    void selfSet(std::size_t uri, Resource* res);
    void selfDestroy(std::size_t uri);

    typedef std::map<std::size_t, Resource*> ResourceMap;
    ResourceMap resources_;

    typedef std::map<u32, ResourceFactory> FactoryMap;
    FactoryMap factories_;

    u32 typeCounter_;
    static ResourceManager* instance_;

private:
    ResourceManager(const ResourceManager&);
    void operator=(const ResourceManager&);
};

class ResourceRef {
public:
    explicit ResourceRef(const std::string& uri);
    ResourceRef(const std::string& uri, Resource* res);
    
    Resource* resource();
    void setResource(Resource* res);
    void destroy();

    template<class T>
    T* resourceAs() {
        return dynamic_cast<T*>(resource());
    }

    template<class T>
    void loadDefault(const std::string& path) {
        ResourceManager::loadDefault(T::Type(), hash_, path);
    }
private:
    std::size_t hash_;
};

} // namespace opengl
} // namespace base