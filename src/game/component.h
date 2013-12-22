#pragma once

#include "game/componentbase.h"

namespace base {
namespace game {

//! Template for core components
//! Should be used as
//!    class MyComponent : public Component<MyComponent> {};
//!    and (in source file)
//!    CompType MyComponent::type_ = ComponentBase::registerType();
template<class T>
class Component : public ComponentBase {
public:
    CompType type() const { return type_; }
    inline static CompType Type() { return type_; }
private:
    static CompType type_;
};

} // namespace game
} // namespace base