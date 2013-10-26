#include "sdlapp.h"

using namespace base;

class Demo : public SDLApp
{
public:
    Demo() {
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
    }
};

#include <cmath>
#include <boost/python.hpp>
#include <string>

using namespace boost::python;

std::string greet();

BOOST_PYTHON_MODULE(example)
{
	def( "greet", greet );
}

std::string greet() {
	return "hello world from embedded python";
}

int main()
{
	Py_Initialize();
	if (PyImport_AppendInittab("example", initexample) == -1)
		return 1;
	object m = import("__main__");
	object global(m.attr("__dict__"));
	object result = exec(
		"from example import greet\n"
		"print greet()         \n",
		global, global);
    
    Demo app;
    app.Run();
    return 0;
}
