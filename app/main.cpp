#include "app/demo.h"
#include <GL/glut.h>
#include "luavm.h"

int main(int argc, char** argv) {
    {
        LuaVM vm;
//        vm.set_include("~/projects/negine/?.lua");
        std::string status;
        if (!vm.run_script("main", status))
            std::cout << status << std::endl;
    }
    {
        GlutSampleWindow app(800, 600);
        app.Run();
    }
    return 0;
}
