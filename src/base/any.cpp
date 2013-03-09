#include "base/any.h"
#include <algorithm>

namespace base {

any& any::swap(any& rhs) {
    std::swap(content, rhs.content);
    return *this;
}

}
