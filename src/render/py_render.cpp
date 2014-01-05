#include "py_render.h"
#include <cmath>
#include <boost/python.hpp>
#include "render/glcontext.h"
#include "render/gpuprogram.h"

using namespace base;
using namespace base::opengl;
using namespace boost::python;

std::string gl_tostr(const DeviceContext& gl);
GpuProgram* createProgram(DeviceContext& gl) {
    GpuProgram* prog = new GpuProgram(gl);
    return prog;
}

BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(GpuProgram_overloads, setAttribute, 2, 3);

void init_py_render()
{
    class_<DeviceContext, boost::noncopyable>("GL", no_init)
        .def( "__str__", gl_tostr)
        .def( "createProgram", createProgram, return_value_policy<manage_new_object>() )
        ;
    enum_<VertexAttr>("VertexAttrs")
        .value("tagPosition", VertexAttrs::tagPosition)
        .value("tagTexture", VertexAttrs::tagTexture)
        .value("tagNormal", VertexAttrs::tagNormal)
        .value("tagColor", VertexAttrs::tagColor)
        .value("tagTangent", VertexAttrs::tagTangent)
        .value("tagBitangent", VertexAttrs::tagBitangent)
        ;
    enum_<ShaderType>("ShaderTypes")
        .value("VERTEX", ShaderTypes::VERTEX)
        .value("PIXEL", ShaderTypes::PIXEL)
        ;
    class_<GpuProgram, boost::noncopyable>("GpuProgram", no_init)
        .def( "destroy", &GpuProgram::destroy )
        .def( "setAttribute", &GpuProgram::setAttribute, GpuProgram_overloads() )
        .def( "setShaderSource", &GpuProgram::setShaderSource )
        .def( "complete", &GpuProgram::complete )
        ;
}

std::string gl_tostr(const DeviceContext& gl)
{
    return "this is the context!!!";
}
