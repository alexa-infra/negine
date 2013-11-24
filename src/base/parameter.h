#pragma once

#include "base/types.h"
#include "base/any.h"
#include "base/fixedmap.h"
//#include <string>
//#include <unordered_map>

namespace base {

    //typedef std::unordered_map<std::string, any> ParameterMap;
    typedef FixedMap<SmallString, any> Params;
}
