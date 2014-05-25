#include "py_render.h"
#include <cmath>
#include <boost/python.hpp>
#include "render/glcontext.h"
#include "render/gpuprogram.h"
#include "render/framebuffer.h"
#include "render/texture.h"
#include "engine/resourceref.h"
#include "render/material.h"
#include "engine/texture_loader.h"

#include <boost/python/suite/indexing/vector_indexing_suite.hpp>

using namespace base;
using namespace base::opengl;
using namespace boost::python;

std::string gl_tostr(const DeviceContext& gl);
GpuProgram* createProgram(DeviceContext& gl, const char* name) {
    GpuProgram* prog = new GpuProgram(gl);
    ResourceRef ref(name);
    ref.setResource(prog);
    return prog;
}
Texture* createTexture(DeviceContext& gl, const char* name, const char* filename) {
    ResourceRef ref(name);
    TextureInfo defaultSettings;
    defaultSettings.Filtering = TextureFilters::Anisotropic;
    defaultSettings.GenerateMipmap = true;
    Texture* texture = loadTexture(gl, defaultSettings, filename);
    ref.setResource(texture);
    return ref.resourceAs<opengl::Texture>();
}
Framebuffer* createFramebuffer(DeviceContext& gl, const char* name) {
    Framebuffer* fbo = new Framebuffer(gl);
    ResourceRef ref(name);
    ref.setResource(fbo);
    return fbo;
}
void resizeFramebuffer(Framebuffer* fbo, i32 width, i32 height) {
    fbo->resizeWindow(math::vec2i(width, height));
}

BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(GpuProgram_overloads, setAttribute, 2, 3);

void init_py_render()
{
    class_<DeviceContext, boost::noncopyable>("GL", no_init)
        .def( "__str__", gl_tostr)
        .def( "createProgram", createProgram, return_value_policy<manage_new_object>() )
        .def( "createTexture", createTexture, return_value_policy<manage_new_object>() )
        .def( "createFramebuffer", createFramebuffer, return_value_policy<manage_new_object>() )
        ;
    enum_<VertexAttr>("VertexAttrs")
        .value("tagPosition", VertexAttrs::tagPosition)
        .value("tagTexture", VertexAttrs::tagTexture)
        .value("tagNormal", VertexAttrs::tagNormal)
        .value("tagColor", VertexAttrs::tagColor)
        .value("tagTangent", VertexAttrs::tagTangent)
        .value("tagBitangent", VertexAttrs::tagBitangent)
        ;
    enum_<ShaderType>("ShaderType")
        .value("VERTEX", ShaderType::VERTEX)
        .value("PIXEL", ShaderType::PIXEL)
        ;
    class_<Texture, boost::noncopyable>("Texture", no_init)
        .def( "destroy", &Texture::destroy )
        ;
    class_<GpuProgram, boost::noncopyable>("GpuProgram", no_init)
        .def( "destroy", &GpuProgram::destroy )
        .def( "setAttribute", &GpuProgram::setAttribute, GpuProgram_overloads() )
        .def( "setShaderSource", &GpuProgram::setShaderSource )
        .def( "complete", &GpuProgram::complete )
        ;
    enum_<InternalType>("InternalTypes")
        .value("RGB8", InternalTypes::RGB8)
        .value("RGBA8", InternalTypes::RGBA8)
        .value("D32F", InternalTypes::D32F)
        ;
    class_<Framebuffer, boost::noncopyable>("Framebuffer", no_init)
        .def( "addTargetTexture", &Framebuffer::addTargetTexture )
        .def( "addTarget", &Framebuffer::addTarget )
        .def( "complete", &Framebuffer::complete )
        .def( "destroy", &Framebuffer::destroy )
        .def( "resize", &resizeFramebuffer )
        ;
    class_<RenderPass>("RenderPass")
        .def_readwrite("target", &RenderPass::target)
        .def_readwrite("generator", &RenderPass::generator)
        .def_readwrite("mode", &RenderPass::mode)
        .def_readwrite("viewport", &RenderPass::viewport)
        .def_readwrite("clear", &RenderPass::clear)
        .def_readwrite("depthTest", &RenderPass::depthTest)
        .def_readwrite("depthWrite", &RenderPass::depthWrite)
        .def_readwrite("cullBackFace", &RenderPass::cullBackFace)
        .def_readwrite("blend", &RenderPass::blend)
        .def_readwrite("clearColor", &RenderPass::clearColor)
        ;
    class_<RenderPipeline>("RenderPipeline")
        .def(vector_indexing_suite<RenderPipeline>());
}

std::string gl_tostr(const DeviceContext& gl)
{
    return "this is the context!!!";
}
