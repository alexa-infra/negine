#pragma once

#include "base/types.h"
#include <vector>
#include "render/gpuresource.h"
#include "math/vec2.h"
#include "render/texture.h"
#include "engine/resource.h"

namespace base {
namespace opengl {

class Framebuffer : public GpuResource, public BaseResource<Framebuffer>
{
public:
    Framebuffer(DeviceContext& context);
    
    ~Framebuffer();

    void resizeWindow(const math::vec2i& newSize);

    void destroy();

    void bind();

    void unbind();

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

    void addTarget(InternalType format);

    void addTargetTexture(Texture* texture);

    void complete();

    bool checkStatus(const char*& message);

    inline bool initialized() const { return initialized_; }
private:
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
