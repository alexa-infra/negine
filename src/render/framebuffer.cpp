#include "framebuffer.h"
#include "base/debug.h"
#include "base/log.h"

namespace base {

template<>
ResourceType BaseResource<opengl::Framebuffer>::type_ = ResourceManager::registerResource();

namespace opengl {

Framebuffer::Framebuffer(DeviceContext& context) : GpuResource(context) {
    initialized_ = false;
    completed_ = false;
    size_ = math::vec2i(0, 0);
    windowSize_ = true;
    scale_ = math::vec2f(1, 1);
}

Framebuffer::~Framebuffer() {
    destroy();
}

void Framebuffer::resizeWindow(const math::vec2i& newSize) {
    if (windowSize_) {
        math::vec2i aSize(newSize.x * scale_.x, newSize.y * scale_.y);
        if (aSize != size_) {
            size_ = aSize;
            initialized_ = false;
        }
    }

    if (!initialized_) {
        complete();
    }
}

void Framebuffer::destroy() {
    if (id_ != 0) {
        GL.DeleteFramebuffers(1, &id_);
        id_ = 0;

        for(auto rt: targets_) {
            if (rt.texture == nullptr)
                GL.DeleteRenderbuffers(1, &id_);
        }
        targets_.clear();
        completed_ = false;
        initialized_ = false;
    }
}

void Framebuffer::bind() {
    ASSERT(handle() != 0);
    GL.BindFramebuffer(GL_FRAMEBUFFER, handle());
    if (!completed_) {
        return;
    }
    
    size_t numColorTargets = colorAttachments_.size();
    if (numColorTargets > 0) {
        GL.DrawBuffers(static_cast<GLsizei>(numColorTargets), colorAttachments_.data());
    } else {
        GL.DrawBuffer(GL_NONE);
        GL.ReadBuffer(GL_NONE);
    }
}

void Framebuffer::unbind() {
    GL.BindFramebuffer(GL_FRAMEBUFFER, 0);
    size_t numColorTargets = colorAttachments_.size();
    if (numColorTargets == 0) {
        GL.DrawBuffer(GL_BACK);
        GL.ReadBuffer(GL_BACK);
    }
}

void Framebuffer::addTarget(InternalType format) {
    if (completed_) return;
    RenderTarget rt;
    rt.texture = nullptr;
    rt.format = format;
    GL.GenRenderbuffers(1, &rt.id);
    if (InternalTypes::isColor(format)) {
        rt.target = GL_COLOR_ATTACHMENT0 + static_cast<GLsizei>(colorAttachments_.size());
        colorAttachments_.push_back(rt.target);
    } else if (InternalTypes::isDepth(format)) {
        rt.target = GL_DEPTH_ATTACHMENT;
    } else if (InternalTypes::isStencil(format)) {
        rt.target = GL_STENCIL_ATTACHMENT;
    } else if (InternalTypes::isDepthStencil(format)) {
        rt.target = GL_DEPTH_STENCIL_ATTACHMENT;
    }
    targets_.push_back(rt);
}

void Framebuffer::addTargetTexture(Texture* texture) {
    if (completed_) return;
    RenderTarget rt;
    rt.id = texture->handle();
    rt.format = texture->info().InternalType;
    rt.texture = texture;
    if (InternalTypes::isColor(rt.format)) {
        rt.target = GL_COLOR_ATTACHMENT0 + static_cast<GLsizei>(colorAttachments_.size());
        colorAttachments_.push_back(rt.target);
    } else if (InternalTypes::isDepth(rt.format)) {
        rt.target = GL_DEPTH_ATTACHMENT;
    } else if (InternalTypes::isStencil(rt.format)) {
        ASSERT(false);
        //rt.target = GL_STENCIL_ATTACHMENT;
    } else if (InternalTypes::isDepthStencil(rt.format)) {
        rt.target = GL_DEPTH_STENCIL_ATTACHMENT;
    }
    targets_.push_back(rt);
}

void Framebuffer::complete() {
    ASSERT(size_.x > 0 && size_.y > 0);
    completed_ = false;
    if (id_ == 0)
        GL.GenFramebuffers(1, &id_);
    GL.setFramebuffer(this);
    for(auto rt: targets_) {
        if (rt.texture != nullptr) {
            TextureInfo newInfo = rt.texture->info();
            newInfo.Width = size_.x;
            newInfo.Height = size_.y;
            rt.texture->createEmpty(newInfo);

            GL.FramebufferTexture2D(GL_FRAMEBUFFER,
                rt.target,
                rt.texture->info().Type,
                rt.texture->handle(),
                0);
        } else {
            GL.BindRenderbuffer(GL_RENDERBUFFER, rt.id);
            GL.RenderbufferStorage(GL_RENDERBUFFER,
                rt.format,
                size_.x,
                size_.y);
            GL.FramebufferRenderbuffer(GL_FRAMEBUFFER, 
                rt.target,
                GL_RENDERBUFFER,
                rt.id);
        }
    }
    const char* message = nullptr;
    completed_ = checkStatus(message);
    if (!completed_) {
        ERR(message);
    }
    initialized_ = true;

    size_t numColorTargets = colorAttachments_.size();
    if (numColorTargets > 0) {
        GL.DrawBuffers(static_cast<GLsizei>(numColorTargets), colorAttachments_.data());
    } else {
        GL.DrawBuffer(GL_NONE);
        GL.ReadBuffer(GL_NONE);
    }
}

bool Framebuffer::checkStatus(const char*& message) {
    GLenum status = GL.CheckFramebufferStatus(GL_FRAMEBUFFER);
    switch(status)
    {
    case GL_FRAMEBUFFER_COMPLETE:
        return true;
    case GL_FRAMEBUFFER_UNSUPPORTED:
        message = "Unsupported framebuffer format";
        return false;
    case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
        message = "Framebuffer incomplete, missing attachment";
        return false;
    case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
        message = "Framebuffer incomplete, incomplete attachment";
        return false;
    case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER:
        message = "Framebuffer incomplete, missing draw buffer";
        return false;
    case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER:
        message = "Framebuffer incomplete, missing read buffer";
        return false;
    default:
        return false;
    }
}

} // namespace opengl
} // namespace base
