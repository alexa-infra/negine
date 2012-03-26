/**
 * \file
 * \author      Alexey Vasilyev <alexa.infra@gmail.com>
 * \copyright   MIT License
 **/
#include "app/demo.h"
#include "luavm.h"

int get_current_dir(lua_State* L) {
    std::string p = "not implemented";
    lua_pushstring(L, p.c_str());
    return 1;
}

int main(int argc, char** argv) {
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
    return 0;
}
