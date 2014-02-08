#pragma once

#include "base/types.h"
#include "base/debug.h"

namespace base
{

template<class Type>
class Singleton
{
protected:
    static Type* instance_;
    Singleton() {}
public:
    template<typename... Args>
    NEGINE_API static void init(Args&... args) {
        ASSERT( hasInstance() == false );
        instance_ = new Type(args...);
    }
    NEGINE_API static void shutdown() {
        ASSERT( hasInstance() == true );
        delete instance_;
        instance_ = nullptr;
    }
protected:
    static bool hasInstance() {
        return instance_ != nullptr;
    }
    static Type& instance() {
        ASSERT( hasInstance() == true );
        return *instance_;
    }
};

} // namespace base
