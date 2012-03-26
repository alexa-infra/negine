/**
 * \file
 * \author      Alexey Vasilyev <alexa.infra@gmail.com>
 * \copyright   MIT License
 **/
#include "base/math/vector.h"

namespace base {
namespace math {

Vector2::Vector2(const Vector3& v) {
    set(v.x, v.y);
}

Vector2::Vector2(const Vector4& v) {
    set(v.x, v.y);
}

Vector3::Vector3(const Vector4& v) {
    set(v.x, v.y, v.z);
}

}
}
