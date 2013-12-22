#pragma once

#include "base/types.h"

namespace base {
namespace game {

typedef u32 CompType;

class Entity;

//! Abstract component type
//! Every child should implement type() method, where returned value
//!        should be obtained once from registerType() static method
class NEGINE_API ComponentBase {
    friend class Entity;
public:
    ComponentBase();
    virtual ~ComponentBase();

    virtual CompType type() const = 0;

    static CompType registerType();

    inline Entity* parent() const { return parent_; }
protected:
    Entity* parent_;
private:
    static CompType s_counter_;
};

} // namespace game
} // namespace base