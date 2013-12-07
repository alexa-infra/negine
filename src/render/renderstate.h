#pragma once

#include "base/types.h"
#include "render/glcontext.h"
#include "math/vec4.h"

namespace base
{
namespace opengl
{

template<GLenum State, bool init>
class BoolState
{
public:
    BoolState(DeviceContext& context) : gl(context) {
        currentState = init;
    }
    bool set(bool newState) {
        if (newState == currentState)
            return currentState;
        currentState = newState;
        if (currentState)
            gl.Enable(State);
        else
            gl.Disable(State);
        return currentState;
    }
    bool get() const { return currentState; }
private:
    DeviceContext& gl;
    bool currentState;
};

template<bool init>
class DepthWriteState
{
public:
    DepthWriteState(DeviceContext& context) : gl(context) {
        currentState = init;
    }
    bool set(bool newState) {
        if (newState == currentState)
            return currentState;
        currentState = newState;
        if (currentState)
            gl.DepthMask(GL_TRUE);
        else
            gl.DepthMask(GL_FALSE);
        return currentState;
    }
private:
    DeviceContext& gl;
    bool currentState;
};

class ViewportState
{
public:
    ViewportState(DeviceContext& context) : gl(context), viewport(0, 0, 0, 0) {
    }

    void set(const math::vec4f& v) {
        if (v == viewport)
            return;
        viewport = v;
        gl.Viewport(viewport.x, viewport.y, viewport.z, viewport.w);
    }
private:
    DeviceContext& gl;
    math::vec4f viewport;
};

class GpuProgramState
{
public:
    GpuProgramState(DeviceContext& context) : gl(context), current(0) {}
    bool set(u32 program)
    {
        if (current == program)
            return false;
        current = program;
        gl.UseProgram(current);
        return true;
    }
private:
    DeviceContext& gl;
    u32 current;
};

template<GLenum Buffer>
class BufferState
{
public:
    BufferState(DeviceContext& context) : gl(context), current(0) {}
    void set(u32 buffer)
    {
        if (current == buffer)
            return;
        current = buffer;
        gl.BindBuffer(Buffer, current);
    }
private:
    DeviceContext& gl;
    u32 current;
};

class TextureUnitState
{
public:
    TextureUnitState(DeviceContext& context) : gl(context), current(0) {}
    void set(u32 unit)
    {
        if (current == unit)
            return;
        current = unit;
        gl.ActiveTexture(GL_TEXTURE0 + unit);
    }
private:
    DeviceContext& gl;
    u32 current;
};

class Mesh;

class RenderState
{
public:
    RenderState(DeviceContext& context);

public:
    DeviceContext& gl;

    BoolState<GL_CULL_FACE, false> cullface;
    BoolState<GL_DEPTH_TEST, false> depthTest;
    BoolState<GL_BLEND, false> blend;
    DepthWriteState<true> depthWrite;
    ViewportState viewportArea;
    GpuProgramState program;
    BufferState<GL_ELEMENT_ARRAY_BUFFER> indexBuffer;
    BufferState<GL_ARRAY_BUFFER> vertexBuffer;
    TextureUnitState activeTexture;

    void render(const Mesh& mesh, u32 from, u32 count);
};



}
}