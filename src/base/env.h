#pragma once

#include "base/platform.h"
#include <string>

namespace base {
    namespace env {
        static std::string variable(const std::string& name, const std::string& def);
        bool fileExists(const std::string& name);
    }
}