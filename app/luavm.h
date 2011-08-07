/**
 * @author  Alexey Vasilyev <alexa.infra@gmail.com>
 * @section LICENSE
 * 
 * This file is part of Negine (https://alexadotlife.com/hg/negine)
 * Licensed under LGPLv3 (or GPLv3) – see LICENSE file for details
 **/
#pragma once

#include <string>
#include <iostream>
#include "lua.hpp"

class LuaVM {
protected:
    lua_State* L;
public:
    LuaVM() : L(NULL) {
        L = lua_open();
        luaL_openlibs(L);
    }
    ~LuaVM() {
        lua_close(L);
    }
    void set_include(const std::string& path) {
        lua_getglobal(L, "package");
        lua_pushstring(L, path.c_str());
        lua_setfield(L, -2, "path");
        lua_pop(L, 1);
    }
    bool run_script(const std::string& name, std::string& report) {
        lua_getglobal(L, "require");
        lua_pushstring(L, name.c_str());
        int status = lua_pcall(L, 1, 0, 0);

        if (status == 0)
            return true;
        report = lua_tostring(L, -1);
        lua_pop(L, 1);
        return false;
    }
    void call_func(const std::string& function, int argc, char** argv) {
        lua_getglobal(L, function.c_str());
        if (!lua_isfunction(L, -1)) {
            lua_pop(L, 1);
            return;
        }
        for (int i=1; i<argc; i++)
            lua_pushstring(L, argv[i]);
        lua_call(L, argc-1, 0);
    }
    void register_func(const std::string& name, lua_CFunction func) {
        lua_register(L, name.c_str(), func);
    }
};
