#pragma once

#include "base/types.h"
#include "foundation/collection_types.h"
#include "foundation/hash.h"
#include "game/componentbase.h"
#include <string>
#include <functional>
#include <map>

namespace base {
namespace game {

class Transform;
class Camera;
class Renderable;

class HashStr {
public:
    HashStr(u64 seed) : seed_(seed) {}
    NEGINE_API u64 operator()(const std::string& s) const;
private:
    u64 seed_;
};

class Signal {
public:
    typedef std::function<void()> Slot;

    Signal() {}

    void connect(const std::string& name, Slot slot) {
        slots_[name] = slot;
    }
    void emit() {
        for (auto it: slots_) {
            Slot slot = it.second;
            slot();
        }
    }
    void disconnect(const std::string& name) {
        slots_.erase(slots_.find(name));
    }
private:
    std::map<std::string, Slot> slots_;
};

class Scene {
public:
    NEGINE_API Scene();
    NEGINE_API ~Scene();

    template<class T>
    T* attach(const std::string& name, T* aspect) {
        attachNamed(name, name + T::extension(), aspect);
        attachTyped<T>(name, aspect);
        return aspect;
    }

    template<class T>
    void detach(const std::string& name, T* aspect) {
        detachNamed(name, name + T::extension(), aspect);
        detachTyped<T>(name, aspect);
    }

    NEGINE_API void attachNamed(const std::string& name, const std::string& fullname, ComponentBase* aspect);

    NEGINE_API void detachNamed(const std::string& name, const std::string& fullname, ComponentBase* aspect);

    template<class T>
    T* getTyped(const std::string& name);

    template<>
    Transform* getTyped<Transform>(const std::string& name) {
        return foundation::hash::get(transforms_, hash_(name), (Transform*)nullptr);
    }
private:
    template<class T>
    void attachTyped(const std::string& name, T* aspect);

    template<>
    void attachTyped<Transform>(const std::string& name, Transform* aspect) {
        foundation::hash::set(transforms_, hash_(name), aspect);
    }

    template<>
    void attachTyped<Camera>(const std::string& name, Camera* aspect) {
        foundation::hash::set(cameras_, hash_(name), aspect);
    }

    template<>
    void attachTyped<Renderable>(const std::string& name, Renderable* aspect) {
        foundation::hash::set(renderables_, hash_(name), aspect);
    }
public:
    foundation::Hash<ComponentBase*> componentByName_;
    foundation::Hash<ComponentBase*> componentByObject_;

    foundation::Hash<Transform*> transforms_;
    foundation::Hash<Camera*> cameras_;
    foundation::Hash<Renderable*> renderables_;

    HashStr hash_;
};

} // namespace game
} // namespace base