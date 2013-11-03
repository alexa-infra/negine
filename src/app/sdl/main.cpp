#include "sdlapp.h"
#include <cmath>
#include <boost/python.hpp>
#include <string>

using namespace base;
using namespace boost::python;

extern "C" __declspec(dllexport) void initexample(void);

class Demo : public SDLApp
{
public:
    Demo(const std::string& filename) {
        Py_Initialize();
        PyImport_AppendInittab("example", initexample);
        object m = import("__main__");
        global = object(m.attr("__dict__"));

        exec_file(filename.c_str(), global, global);
        object fuu = extract<object>(global["fuu"]);
        fuu();
    }
    virtual ~Demo() {
    }
protected:
    void OnFrame() {
        GL.ClearColor( 1.0f, 0.0f, 0.0f, 1.0f );
        GL.Clear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
        GL_ASSERT(GL);
        SDLApp::OnFrame();
    }

    void OnMotion( i32 x, i32 y, i32 dx, i32 dy ) {
    }

    void OnKeyboardDown( u8 key ) {
        exec("print greet()", global, global);
    }

    object global;
};

std::string greet();

BOOST_PYTHON_MODULE(example)
{
	def( "greet", greet );
}

std::string greet()
{
	return "hello world from embedded python";
}

int main(int argc, char* argv[])
{
    Demo app(argv[1]);
    app.Run();
    return 0;
}
