#include <boost/python.hpp>
#include "math/vec4.h"

using namespace base;
using namespace boost::python;

void init_py_math()
{
    class_<math::vec4f>("vec4f")
        .def( init<f32, f32, f32, f32>( args( "x", "y", "z", "w" ) ) )
        .def_readwrite( "x", &math::vec4f::x )
        .def_readwrite( "y", &math::vec4f::y )
        .def_readwrite( "z", &math::vec4f::z )
        .def_readwrite( "w", &math::vec4f::w )
        ;
}

