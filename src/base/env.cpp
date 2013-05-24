#include "base/env.h"
#ifdef OS_UNIX
    #include <unistd.h>
#endif
#include <fstream>

namespace base {
    
#ifdef OS_UNIX
    std::string Env::variable(const std::string& name, const std::string& def)
    {
        char* value = getenv(name.c_str());
        if (!value) return def;
        return std::string(value);
    }
#endif

    bool file_exists( const std::string& name )
    {
        std::ifstream f( name.c_str() );;
        return f.is_open() && f.good();
    }
}
