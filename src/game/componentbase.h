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

    ~ComponentBase() {}

    inline std::string name() const { return name_; }

    inline void setName(const std::string& name) { name_ = name; }

    inline void setScene(Scene* scene) { scene_ = scene; }

    inline const Scene* scene() const { return scene_; }

protected:
    Scene* scene_;
    std::string name_;
};

} // namespace game
} // namespace base