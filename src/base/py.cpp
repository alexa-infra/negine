#include "base/py.h"
#include <cmath>
#include <boost/python.hpp>
#include "math/py_math.h"
#include "render/py_render.h"

using namespace boost::python;

BOOST_PYTHON_MODULE(negine_core)
{
    init_py_math();
    init_py_render();
}