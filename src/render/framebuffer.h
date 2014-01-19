#pragma once

#include "base/types.h"
#include <vector>
#include "render/gpuresource.h"
#include "math/vec2.h"
#include "render/texture.h"
#include "engine/resource.h"

namespace base {
namespace opengl {

class Framebuffer : public GpuResource, public ResourceBase<Framebuffer>
{
public:
    NEGINE_API Framebuffer(DeviceContext& context);
    
    NEGINE_API ~Framebuffer();

    NEGINE_API void resizeWindow(const math::vec2i& newSize);

    NEGINE_API void destroy();

    void bind();

    void unbind();

    NEGINE_API void addTarget(InternalType format);

    NEGINE_API void addTargetTexture(Texture* texture);

    NEGINE_API void complete();

    bool checkStatus(const char*& message);

    inline bool initialized() const { return initialized_; }
private:
    struct RenderTarget {
        GLuint id;
        InternalType format;
        GLenum target;
        GLenum textureFormat;
        Texture* texture;
        RenderTarget() {
            texture = nullptr;
        }
    };
    
    bool windowSize_;
    math::vec2i size_;
    math::vec2f scale_;

    bool initialized_;
    std::vector<u32> colorAttachments_;

    std::vector<RenderTarget> targets_;
    bool completed_;
private:
    DISALLOW_COPY_AND_ASSIGN( Framebuffer );
};


} // namespace opengl
} // namespace base
