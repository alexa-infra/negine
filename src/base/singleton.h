#pragma once

#include <assert.h>

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
        assert( hasInstance() == false );
        instance_ = new Type;
    }
    static void shutdown() {
        assert( hasInstance() == true );
        delete instance_;
        instance_ = nullptr;
    }
    static bool hasInstance() {
        return instance_ != nullptr;
    }
    static Type* instance() {
        assert( hasInstance() == true );
        return instance_;
    }
};
template<typename Type> Type* Singleton<Type>::instance_ = nullptr;

} // namespace base