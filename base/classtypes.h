#pragma once

#include <typeinfo>
#include "base/types.h"

namespace base {

namespace opengl { class Texture; }

namespace Types {
    enum Type {
        Unknown,
        I32,
        U32,
        Texture
    };
}
typedef Types::Type Type;

template<typename T>
class class_info {
public:
    static inline u32   align()   { return __alignof(T); }
    static inline u32   size()    { return sizeof(T); }
    static inline Type  type()    { return Types::Unknown; }
};

template<> inline Type class_info<i32>::type() { return Types::I32; }
template<> inline Type class_info<u32>::type() { return Types::U32; }

template<> inline Type class_info<opengl::Texture*>::type() { return Types::Texture; }

}
