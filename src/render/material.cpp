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

void Renderer::init() {
//    RenderPass rp;
//    rp.target = nullptr;
//    rp.mode = "normal";
//    rp.generator = nullptr;
//    rp.viewport = math::vec4f(0, 0, 640, 480);
//    rp.clear = true;
//    rp.depthTest = true;
//    rp.depthWrite = true;
//    rp.cullBackFace = false;
//    rp.blend = false;
//    rp.clearColor = math::vec4f(1.0f, 0.0f, 0.0f, 1.0f);
//    passesList.push_back(rp);

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

void Renderer::rendering() {
    for(auto pass: passesList) {
        GL.setFramebuffer(pass.target.resourceAs<Framebuffer>());
        renderState(pass);
        if (pass.generator == "scene") {
            sceneRenderer(pass.mode.c_str(), pass.params);
        } else if (pass.generator == "fullscreen") {
            fullscreenRenderer(pass.mode.c_str(), pass.params);
        }
    }
}

void Renderer::renderState(const RenderPass& rp) {
    GL.setViewport(rp.viewport);
    GL.setDepthTest(rp.depthTest);
    GL.setDepthWrite(rp.depthWrite);
    if (rp.clear) {
        GL.ClearColor(rp.clearColor.x, rp.clearColor.y, rp.clearColor.z, rp.clearColor.w);
        GL.Clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }
}

void Renderer::sceneRenderer(const std::string& mode, const Params& pp) {
    game::CompList components = game::find_all(root, game::Renderable::Type());
    for(auto c: components) {
        game::Renderable* r = dynamic_cast<game::Renderable*>(c);
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
    
void Renderer::fullscreenRenderer(const std::string& mode, const Params& pp) {
    ResourceRef progRef(mode);
    opengl::GpuProgram* prog = progRef.resourceAs<GpuProgram>();
    GL.setProgram(prog);
    prog->setParams(pp);
    GL.renderState().render(fullscreenQuad, 0, fullscreenQuad.numIndexes());
}

} // namespace opengl
} // namespace base