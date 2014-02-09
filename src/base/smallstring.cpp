#include "base/smallstring.h"
#include "base/debug.h"

namespace base {

SmallString::SmallString() {
    buffer_[0] = '\0';
}

SmallString::SmallString(const char* s) {
    ASSERT(strlen(s) < 20);
    strcpy(buffer_, s);
}

SmallString::SmallString(const SmallString& s) {
    strcpy(buffer_, s.buffer_);
}

SmallString& SmallString::operator=(const SmallString& s) {
    strcpy(buffer_, s.buffer_);
    return *this;
}

bool SmallString::operator==(const SmallString& s) const {
    return (strcmp(buffer_, s.buffer_) == 0);
}

bool SmallString::operator<(const SmallString& s) const {
    return (strcmp(buffer_, s.buffer_) < 0);
}

const char* SmallString::c_str() const {
    return buffer_;
}

}