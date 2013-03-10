#include "base/env.h"
#ifdef OS_MAC
    #include <unistd.h>
#endif

namespace base {
    
#ifdef OS_MAC
    std::string Env::variable(const std::string& name, const std::string& def)
    {
        char* value = getenv(name.c_str());
        if (!value) return def;
        return std::string(value);
    }
#endif

}