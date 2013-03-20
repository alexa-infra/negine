#pragma once

#include "base/platform.h"
#include <string>

namespace base {
    class Env {
    public:
        static std::string variable(const std::string& name, const std::string& def);
    };

    bool file_exists( const std::string& name );
}