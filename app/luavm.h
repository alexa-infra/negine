/**
 * @author  Alexey Vasilyev <alexa.infra@gmail.com>
 * @section LICENSE
 * 
 * This file is part of Negine (https://alexadotlife.com/hg/negine)
 * Licensed under LGPLv3 (or GPLv3) - see LICENSE file for details
 **/
#pragma once

#include <string>
#include <iostream>
#include "lua.hpp"

class LuaVM {
protected:
    lua_State* L;
public:
    LuaVM();
    ~LuaVM();
    void set_include(const std::string& path);
    bool run_script(const std::string& name, std::string& report);
    void call_func(const std::string& function, int argc, char** argv);
    void register_func(const std::string& name, lua_CFunction func);
};
