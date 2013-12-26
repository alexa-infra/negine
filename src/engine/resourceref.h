#pragma once

#include "base/types.h"
#include "engine/resource.h"

namespace base {

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

} // namespace base