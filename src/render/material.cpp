#include "material.h"
#include "engine/meshbuilder.h"
#include "game/components/camera.h"
#include "game/components/renderable.h"
#include "game/components/transform.h"
#include "game/scene.h"
#include "math/matrix.h"
#include "render/gpuprogram.h"
#include "render/renderstate.h"
#include "render/glcontext.h"
#include "math/matrix-inl.h"

namespace base {

template<>
ResourceType ResourceBase<opengl::Material>::type_ = ResourceManager::registerResource();

namespace opengl {

bool Material::hasMode(const SmallString& mode) const {
    return modeMap.contains(mode);
}

opengl::GpuProgram* Material::program(const SmallString& mode) const {
    ResourceRef* ref;
    if (!modeMap.tryGet(mode, ref))
        return nullptr;
    return ref->resourceAs<opengl::GpuProgram>();
}

Renderer::Renderer() {
    imp::MeshBuilder bb;
    bb.beginSurface();
    bb.addVertex(math::vec3f( 1, -1, -1), math::vec2f(0, 0));
    bb.addVertex(math::vec3f( 1,  1, -1), math::vec2f(0, 1));
    bb.addVertex(math::vec3f(-1,  1, -1), math::vec2f(1, 1));
    bb.addVertex(math::vec3f(-1, -1, -1), math::vec2f(1, 0));

    bb.addPolygon(0, 1, 2);
    bb.addPolygon(0, 2, 3);
    bb.endSurface();
    bb.getDrawingList(fullscreenQuad);
}

void Renderer::render(DeviceContext& GL, const RenderPipeline& pipeline, const game::Camera* camera) {
    for (auto pass : pipeline) {
        ResourceRef target(pass.target.c_str());
        GL.setFramebuffer(target.resourceAs<Framebuffer>());
        renderState(GL, pass);
        if (pass.generator == "scene") {
            sceneRenderer(GL, pass.mode.c_str(), pass.params, camera);
        } else if (pass.generator == "fullscreen") {
            fullscreenRenderer(GL, pass.mode.c_str(), pass.params);
        }
    }
}

void Renderer::renderState(DeviceContext& GL, const RenderPass& rp) {
    GL.setViewport(rp.viewport);
    GL.setDepthTest(rp.depthTest);
    GL.setDepthWrite(rp.depthWrite);
    if (rp.clear) {
        GL.ClearColor(rp.clearColor.x, rp.clearColor.y, rp.clearColor.z, rp.clearColor.w);
        GL.Clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }
}

void Renderer::sceneRenderer(DeviceContext& GL, const std::string& mode, const Params& pp, const game::Camera* camera) {
    const game::Scene* root = camera->scene();
    auto begin = foundation::hash::begin(root->renderables_);
    auto end = foundation::hash::end(root->renderables_);
    for (auto it = begin; it != end; ++it) {
        game::Renderable* r = it->value;
        opengl::Model* model = r->model();
        math::Matrix4 mvp = camera->clipMatrix() * r->world();

        size_t meshCount = model->surfaceCount();
        for(size_t i=0; i<meshCount; i++) {
            opengl::Mesh& m = const_cast<opengl::Mesh&>(model->surfaceAt(i).mesh);
            Material* material = m.material_.resourceAs<Material>(); // m.material();
            Params* meshParams = &m.params_;
            if (material->hasMode(mode.c_str())) {
                opengl::GpuProgram* prog = material->program(mode.c_str());
                GL.setProgram(prog);
                prog->setParams(material->defaultParams);
                prog->setParams(*meshParams);
                prog->setParams(pp);
                prog->setParam("mvp", mvp);
                GL.renderState().render(m, 0, m.numIndexes());
            }
        }
    }
}
    
void Renderer::fullscreenRenderer(DeviceContext& GL, const std::string& mode, const Params& pp) {
    ResourceRef progRef(mode);
    opengl::GpuProgram* prog = progRef.resourceAs<GpuProgram>();
    GL.setProgram(prog);
    prog->setParams(pp);
    GL.renderState().render(fullscreenQuad, 0, fullscreenQuad.numIndexes());
}

} // namespace opengl
} // namespace base