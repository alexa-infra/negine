#pragma once

#include "base/types.h"
#include "base/fixedmap.h"
#include "math/vec4.h"
#include "base/parameter.h"
#include "engine/resourceref.h"
#include "render/mesh.h"
#include <vector>

namespace base {

namespace game { class Entity; class Camera; }

namespace opengl {

class DeviceContext;
class GpuProgram;

struct Material : public BaseResource<Material>
{
    typedef FixedMap<SmallString, ResourceRef> ProgramMap;
    ProgramMap modeMap; // mode -> gpu program
    Params defaultParams;

    bool hasMode(const SmallString& mode) const;

    opengl::GpuProgram* program(const SmallString& mode) const;
};

struct RenderPass
{
    ResourceRef target;
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
};

struct Renderer {

    Renderer(DeviceContext& gl) : GL(gl) {}

    NEGINE_API void init();
    NEGINE_API void rendering();

    std::vector<RenderPass> passesList;
    game::Entity* root;
    game::Camera* camera;

private:
    void renderState(const RenderPass& rp);
    void sceneRenderer(const std::string& mode, const Params& pp);
    void fullscreenRenderer(const std::string& mode, const Params& pp);

    Mesh fullscreenQuad;
    DeviceContext& GL;
};

}
}