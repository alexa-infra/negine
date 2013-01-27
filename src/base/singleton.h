#pragma once

#include "base/types.h"

namespace base
{

template<typename Type>
class Singleton
{
protected:
    static Type* instance_;
    Singleton() {}
public:
    static void init() {
        ASSERT( hasInstance() == false );
        instance_ = new Type;
    }
    static void shutdown() {
        ASSERT( hasInstance() == true );
        delete instance_;
        instance_ = nullptr;
    }
    static bool hasInstance() {
        return instance_ != nullptr;
    }
    static Type* instance() {
        ASSERT( hasInstance() == true );
        return instance_;
    }
};
template<typename Type> Type* Singleton<Type>::instance_ = nullptr;

} // namespace base
