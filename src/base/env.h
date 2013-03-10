#pragma once

#include "base/types.h"
#include <string>

namespace base {
    class Env {
    public:
        static std::string variable(const std::string& name, const std::string& def);
    };
}