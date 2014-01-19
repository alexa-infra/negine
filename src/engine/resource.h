#pragma once

#include "base/types.h"
#include <functional>
#include <string>
#include <map>

namespace base {

typedef u32 ResourceType;

class Resource {
public:
    virtual ~Resource() {}
    virtual ResourceType type() const = 0;
};

template<typename T>
class ResourceBase : public Resource {
public:
    virtual ~ResourceBase() {}
    ResourceType type() const { return type_; }
    static ResourceType Type() { return type_; }
private:
    NEGINE_API static ResourceType type_;
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
    NEGINE_API static u32 registerResource();
    NEGINE_API static void addFactory(u32 type, ResourceFactoryFunc factory);

    NEGINE_API static Resource* loadDefault(u32 type, std::size_t uri, const std::string& path);
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

} // namespace base