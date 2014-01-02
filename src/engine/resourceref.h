#pragma once

#include "base/types.h"
#include "engine/resource.h"

namespace base {

class ResourceRef {
public:
    NEGINE_API ResourceRef();
    NEGINE_API explicit ResourceRef(const std::string& uri);
    NEGINE_API ResourceRef(const std::string& uri, Resource* res);
    NEGINE_API ResourceRef& operator=(const ResourceRef& r);

    NEGINE_API Resource* resource();
    NEGINE_API void setResource(Resource* res);
    NEGINE_API void destroy();

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