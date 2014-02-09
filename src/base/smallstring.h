#pragma once

#include "base/types.h"
#include <string>

namespace base {

class SmallString
{
public:
    NEGINE_API SmallString();
    NEGINE_API SmallString(const char* s);
    NEGINE_API SmallString(const SmallString& s);
    NEGINE_API SmallString& operator=(const SmallString& s);
    NEGINE_API bool operator==(const SmallString& s) const;
    NEGINE_API bool operator<(const SmallString& s) const;
    NEGINE_API const char* c_str() const;
private:
    char buffer_[20];
};

class HashString
{
public:
    inline HashString() {
        hash_ = 0;
    }
    inline HashString(const char* s) {
        std::hash<std::string> h;
        hash_ = h(s);
    }
    inline bool operator==(const HashString& s) const {
        return hash_ == s.hash_;
    }
    inline bool operator<(const HashString& s) const {
        return hash_ < s.hash_;
    }
    inline const char* c_str() const {
        return "";
    }
private:
    u64 hash_;
};

}