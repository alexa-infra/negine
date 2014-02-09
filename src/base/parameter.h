#pragma once

#include "base/types.h"
#include "base/fixedmap.h"
#include "math/vec4.h"
#include "math/matrix.h"
#include "math/matrix-inl.h"

namespace base {

    class Variant
    {
    public:
        inline Variant() : type(Type::None) {}
        inline Variant(bool v) : type(Type::Bool), b(v) {}
        inline Variant(f32 v) : type(Type::Float), f(v) {}
        inline Variant(i32 v) : type(Type::Int), i(v) {}
        inline Variant(const math::vec2f& v) : type(Type::Vec2), v2({ v.x, v.y }) {}
        inline Variant(const math::vec3f& v) : type(Type::Vec3), v3({ v.x, v.y, v.z }) {}
        inline Variant(const math::vec4f& v) : type(Type::Vec4), v4({ v.x, v.y, v.z, v.w }) {}
        
        Variant(const math::Matrix4& m);
        Variant(const char* v);

        NEGINE_API Variant(const Variant& v);
        NEGINE_API Variant& operator=(const Variant& v);
        NEGINE_API ~Variant();

        bool operator==(const Variant& v) const;
        bool operator!=(const Variant& v) const;

        inline bool isNone()   const { return type == Type::None; }
        inline bool isBool()   const { return type == Type::Bool; }
        inline bool isFloat()  const { return type == Type::Float; }
        inline bool isInt()    const { return type == Type::Int; }
        inline bool isVec2()   const { return type == Type::Vec2; }
        inline bool isVec3()   const { return type == Type::Vec3; }
        inline bool isVec4()   const { return type == Type::Vec4; }
        inline bool isMat4()   const { return type == Type::Mat4; }
        inline bool isString() const { return type == Type::String; }
        
        inline const char*  asString() const { ASSERT(isString()); return s.s; }
        inline bool           asBool() const { ASSERT(isBool());   return b; }
        inline f32           asFloat() const { ASSERT(isFloat());  return f; }
        inline i32             asInt() const { ASSERT(isInt());    return i; }
        inline math::vec2f    asVec2() const { ASSERT(isVec2());   return math::vec2f(v2.x, v2.y); }
        inline math::vec3f    asVec3() const { ASSERT(isVec3());   return math::vec3f(v3.x, v3.y, v3.z); }
        inline math::vec4f    asVec4() const { ASSERT(isVec4());   return math::vec4f(v4.x, v4.y, v4.z, v4.w); }
        math::Matrix4  asMat4() const;

    private:
        enum class Type {
            None, Bool, Int, Float, Vec2, Vec3, Vec4, Mat4, String
        } type;

        union {
            bool b;
            f32 f;
            i32 i;
            struct Vec2 { f32 x, y; } v2;
            struct Vec3 { f32 x, y, z; } v3;
            struct Vec4 { f32 x, y, z, w; } v4;
            struct Mat4 { f32 m[16]; } m4;
            struct String { char* s; } s;
        };
    };

    typedef FixedMap<SmallString, Variant> Params;
}
