#pragma once

#include "base/types.h"
#include "base/fixedmap.h"
#include "math/vec4.h"
#include "base/parameter.h"
#include "engine/resourceref.h"
#include "render/mesh.h"
#include <vector>

namespace base {

namespace game { class Scene; class Camera; }

namespace opengl {

class DeviceContext;
class GpuProgram;

struct Material : public ResourceBase<Material>
{
    typedef FixedMap<SmallString, ResourceRef> ProgramMap;
    ProgramMap modeMap; // mode -> gpu program
    Params defaultParams;

    bool hasMode(const SmallString& mode) const;

    opengl::GpuProgram* program(const SmallString& mode) const;
};

struct RenderPass
{
    std::string target;
    std::string generator;
    std::string mode;
    math::vec4f viewport;
    Params params;

    bool clear;
    bool depthTest;
    bool depthWrite;
    bool cullBackFace;
    bool blend;
    math::vec4f clearColor;

    bool operator==(const RenderPass& rp) const {
        return target == rp.target &&
            generator == rp.generator &&
            mode == rp.mode &&
            viewport == rp.viewport &&
            clear == rp.clear &&
            depthTest == rp.depthTest &&
            depthWrite == rp.depthWrite &&
            cullBackFace == rp.cullBackFace && 
            blend == rp.blend &&
            clearColor == rp.clearColor &&
            params == rp.params;
    }
};
typedef std::vector<RenderPass> RenderPipeline;

struct Renderer {

    Renderer();
    NEGINE_API void render(DeviceContext& context, const RenderPipeline& pipeline, const game::Camera* camera);

private:
    void renderState(DeviceContext& context, const RenderPass& rp);
    void sceneRenderer(DeviceContext& context, const std::string& mode, const Params& pp, const game::Camera* camera);
    void fullscreenRenderer(DeviceContext& context, const std::string& mode, const Params& pp);

    Mesh fullscreenQuad;
};

}
}