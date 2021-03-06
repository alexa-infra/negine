#pragma once

#include "base/types.h"
#include "render/glcontext.h"
#include "math/vec4.h"
#include "render/gpuprogram.h"
#include "render/texture.h"
#include "render/framebuffer.h"

namespace base {
namespace opengl {

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
        gl.Viewport(static_cast<i32>(viewport.x), static_cast<i32>(viewport.y), static_cast<i32>(viewport.z), static_cast<i32>(viewport.w));
    }
private:
    DeviceContext& gl;
    math::vec4f viewport;
};

class GpuProgramState
{
public:
    GpuProgramState(DeviceContext& context) : gl(context), current_(nullptr) {}
    bool set(GpuProgram* program)
    {
        if (current_ == program)
            return false;
        current_ = program;
        if (current_ == nullptr)
            gl.UseProgram(0u);
        else
            gl.UseProgram(current_->handle());
        return true;
    }
    GpuProgram& current() { return *current_; }
private:
    DeviceContext& gl;
    GpuProgram* current_;
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
    TextureUnitState(DeviceContext& context) : gl(context), current(0xffff) {}
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

class TextureState
{
public:
    Texture* current_;
    TextureState() : current_(nullptr) {}
    void set(Texture* newState) {
        if (current_ == newState)
            return;
        if (newState == nullptr)
            current_->unbind();
        else
            newState->bind();
        current_ = newState;
    }
};

class FramebufferState {
public:
    Framebuffer* current_;
    FramebufferState() : current_(nullptr) {
    }

    void set(Framebuffer* newState) {
        if (newState == current_) {
            if (current_ != nullptr && !current_->initialized()) {
                current_->complete();
            }
            return;
        }
        if (newState == nullptr) {
            current_->unbind();
        } else {
            newState->bind();
        }
        current_ = newState;
    }
};


class Mesh;

class RenderState
{
public:
    RenderState(DeviceContext& context);

public:

    BoolState<GL_CULL_FACE, false> cullface;
    BoolState<GL_DEPTH_TEST, false> depthTest;
    BoolState<GL_BLEND, false> blend;
    DepthWriteState<true> depthWrite;
    ViewportState viewportArea;
    GpuProgramState program;
    BufferState<GL_ELEMENT_ARRAY_BUFFER> indexBuffer;
    BufferState<GL_ARRAY_BUFFER> vertexBuffer;
    TextureUnitState activeTexture;
    TextureState textureState;
    FramebufferState framebuffer;

    NEGINE_API void render(const Mesh& mesh, u32 from, u32 count);
private:
    DeviceContext& gl;
};



}
}