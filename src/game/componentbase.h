#pragma once

#include "base/types.h"
#include <string>

namespace base {
namespace game {

class Scene;

//! Abstract component type
class ComponentBase {
public:
    ComponentBase() : scene_(nullptr) {}

    virtual ~ComponentBase() {}

    virtual const char* extension() const = 0;

    inline std::string name() const { return name_; }

    inline std::string fullname() const { return name_ + extension(); }

    inline void setName(const std::string& name) { name_ = name; }

    inline void setScene(Scene* scene) { scene_ = scene; }

protected:
    Scene* scene_;
    std::string name_;
};

} // namespace game
} // namespace base