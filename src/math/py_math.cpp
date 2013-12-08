#include <boost/python.hpp>
#include "math/vec4.h"
#include <string>
#include <sstream>

using namespace base;
using namespace boost::python;

std::string vec4f_str(const math::vec4f&);
std::string vec4f_repr(const math::vec4f&);

void init_py_math()
{
    class_<math::vec4f>("vec4f", init<f32, f32, f32, f32>( args( "x", "y", "z", "w" ) ) )
        .def_readwrite( "x", &math::vec4f::x )
        .def_readwrite( "y", &math::vec4f::y )
        .def_readwrite( "z", &math::vec4f::z )
        .def_readwrite( "w", &math::vec4f::w )
        .def( "__str__", vec4f_str )
        .def( "__repr__", vec4f_repr )
        ;
}

std::string vec4f_str(const math::vec4f& v)
{
	std::ostringstream out;
	out << "{ " << v.x << ", " << v.y << ", " << v.z << ", " << v.w << " }";
	return out.str();
}

std::string vec4f_repr(const math::vec4f& v)
{
	return "vec4f: " + vec4f_str(v); 
}