#include "app/demo.h"
#include <GL/glut.h>
#include "luavm.h"

#include <boost/filesystem.hpp>

namespace fs = boost::filesystem;

int get_current_dir(lua_State* L) {
    fs::path p = fs::current_path();
    lua_pushstring(L, p.native().c_str());
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
        GlutSampleWindow app(800, 600);
        app.Run();
    }
    return 0;
}
