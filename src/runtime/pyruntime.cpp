#include "demo.h"
#include <cmath>
#include <boost/python.hpp>

using namespace boost::python;

BOOST_PYTHON_MODULE(negine_runtime)
{
    def("globalApp", globalApp, return_value_policy<reference_existing_object>());
    class_<Demo, boost::noncopyable>("App", no_init)
        .add_property("context", make_function(static_cast< opengl::DeviceContext& (Application::*)() >(&Application::context), return_value_policy<reference_existing_object>()));
}