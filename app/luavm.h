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
};
