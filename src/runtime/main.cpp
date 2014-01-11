#include "demo.h"
#include <boost/python/errors.hpp>
#include <boost/bind.hpp>

void run(const char* module) {
    Demo app(module);
    app.Run();    
}

int main(int argc, char* argv[])
{
    if (boost::python::handle_exception(boost::bind(run, argv[1]))) {
        if (PyErr_Occurred()) {
            PyErr_Print();
        }
    }

    return 0;
}