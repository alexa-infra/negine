/**
 * \file
 * \author      Alexey Vasilyev <alexa.infra@gmail.com>
 * \copyright   MIT License
 **/
#include "base/types.h"
#include "app/demo.h"
#include "luavm.h"

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>

#ifdef OS_WIN
#include <crtdbg.h>
#endif

int get_current_dir(lua_State* L) {
    std::string p = "not implemented";
    lua_pushstring(L, p.c_str());
    return 1;
}

int main(int argc, char** argv) {

#ifdef OS_WIN
    //_CrtSetBreakAlloc(...);
    _CrtMemState state;
    _CrtMemCheckpoint(&state);
#endif
    {
        LuaVM vm;
//        vm.set_include("~/projects/negine/?.lua");
        std::string status;
        if (!vm.run_script("main", status))
            std::cout << status << std::endl;
        vm.register_func("get_current_dir", &get_current_dir);
        vm.call_func("main", argc, argv);
    }
    {
        Demo app;
        app.Run();
    }
#ifdef OS_WIN
    _CrtMemDumpAllObjectsSince(&state);
#endif
    return 0;
}
