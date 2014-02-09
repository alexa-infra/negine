#include "base/parameter.h"

namespace base {

Variant::Variant(const math::Matrix4& m) : type(Type::Mat4) {
    memcpy(m4.m, reinterpret_cast<const f32*>(&m), 16 * sizeof(f32));
}

Variant::Variant(const char* v) : type(Type::String) {
    s.s = new char[strlen(v)];
    strcpy(s.s, v);
}

Variant::~Variant() {
    if (type == Type::String)
        delete[] s.s;
}

Variant::Variant(const Variant& v) : type(v.type) {
    switch (type) {
    case Type::String: {
                            s.s = new char[strlen(v.s.s)];
                            strcpy(s.s, v.s.s);
                            break;
    }
    case Type::Mat4: {
                            memcpy(m4.m, v.m4.m, 16 * sizeof(f32));
                            break;
    }
    case Type::Bool: b = v.b; break;
    case Type::Float: f = v.f; break;
    case Type::Int: i = v.i; break;
    case Type::Vec2: v2 = v.v2; break;
    case Type::Vec3: v3 = v.v3; break;
    case Type::Vec4: v4 = v.v4; break;
    default: break;
    }
}

Variant& Variant::operator=(const Variant& v) {
    if (type == Type::String) delete[] s.s;
    type = v.type;
    switch (type) {
    case Type::String: {
                            s.s = new char[strlen(v.s.s)];
                            strcpy(s.s, v.s.s);
                            break;
    }
    case Type::Mat4: {
                            memcpy(m4.m, v.m4.m, 16 * sizeof(f32));
                            break;
    }
    case Type::Bool: b = v.b; break;
    case Type::Float: f = v.f; break;
    case Type::Int: i = v.i; break;
    case Type::Vec2: v2 = v.v2; break;
    case Type::Vec3: v3 = v.v3; break;
    case Type::Vec4: v4 = v.v4; break;
    default: break;
    }
    return *this;
}

math::Matrix4 Variant::asMat4() const {
    ASSERT(isMat4());
    math::Matrix4 m;
    memcpy(reinterpret_cast<f32*>(&m), m4.m, 16 * sizeof(f32));
    return m;
}

bool Variant::operator==(const Variant& v) const {
    if (type != v.type) return false;
    switch (type) {
    case Type::None: return true;
    case Type::Bool: return b == v.b;
    case Type::Int: return i == v.i;
    case Type::Float: return f == v.f;
    case Type::Vec2: return memcmp(&v2, &v.v2, 2 * sizeof(f32)) == 0;
    case Type::Vec3: return memcmp(&v3, &v.v3, 3 * sizeof(f32)) == 0;
    case Type::Vec4: return memcmp(&v4, &v.v4, 4 * sizeof(f32)) == 0;
    case Type::Mat4: return memcmp(&m4, &v.m4, 16 * sizeof(f32)) == 0;
    case Type::String: return strcmp(s.s, v.s.s) == 0;
    default: return false;
    }
}
bool Variant::operator!=(const Variant& v) const {
    if (type != v.type) return true;
    switch (type) {
    case Type::None: return false;
    case Type::Bool: return b != v.b;
    case Type::Int: return i != v.i;
    case Type::Float: return f != v.f;
    case Type::Vec2: return memcmp(&v2, &v.v2, 2 * sizeof(f32)) != 0;
    case Type::Vec3: return memcmp(&v3, &v.v3, 3 * sizeof(f32)) != 0;
    case Type::Vec4: return memcmp(&v4, &v.v4, 4 * sizeof(f32)) != 0;
    case Type::Mat4: return memcmp(&m4, &v.m4, 16 * sizeof(f32)) != 0;
    case Type::String: return strcmp(s.s, v.s.s) != 0;
    default: return false;
    }
    return true;
}
    
}