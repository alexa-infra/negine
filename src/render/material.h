#pragma once

#include "base/types.h"
#include "base/fixedmap.h"
#include "math/vec4.h"
#include "render/glcontext.h"
#include "render/renderstate.h"
#include "base/parameter.h"
#include "render/camera.h"
#include "render/gpuprogram.h"
#include <vector>

namespace base
{
namespace opengl
{

class Mesh;

struct Material
{
    typedef FixedMap<SmallString, GpuProgram*> ProgramMap;
    ProgramMap programs;
    Params defaultParams;
};

struct Model
{
    Mesh* mesh;
    math::Matrix4 transform;
    Material* material;
};

struct Scene
{
    std::vector<Model> objects;
};

struct SceneGenerator
{
    DeviceContext& GL;
    Scene& scene;
    Camera& cam;

    void perform(const Params& rpParams, const SmallString& mode)
    {
        base::Params params;
        for (auto obj: scene.objects) {
            GpuProgram* prog;
            if (obj.material->programs.tryGet(mode, prog)) {
                if (GL.renderState().program.set(prog)) {
                    prog->setParams(obj.material->defaultParams);
                    prog->setParams(rpParams);
                }
                params["mvp"] = obj.transform * cam.clipMatrix();
                prog->setParams(params);

                //GL.renderState().render(*obj.mesh);
            }
        }
    }
};

struct Framebuffer
{
    //Params::TextureMap textures;
    //math::vec2f dimensions;
};

struct Generator
{
};

struct RenderPass
{
    Framebuffer* target;
    SmallString mode;
    Generator* generator;

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

    DeviceContext& GL;
    std::vector<RenderPass> passesList;
    void init() {
        RenderPass rp;
        rp.target = nullptr;
        rp.mode = "normal";
        rp.generator = nullptr;
        rp.viewport = math::vec4f(0, 0, 640, 480);
        rp.clear = true;
        rp.depthTest = false;
        rp.depthWrite = false;
        rp.cullBackFace = false;
        rp.blend = false;
        rp.clearColor = math::vec4f(1.0f, 0.0f, 0.0f, 1.0f);
        passesList.push_back(rp);
    }
    void rendering() {
        for(auto pass: passesList) {
            bind(pass.target);
            renderState(pass);
            //generator->draw(pass.mode);
        }
    }
    void bind(Framebuffer* fbo) {
        if (fbo == nullptr) return;
    }
    void renderState(const RenderPass& rp) {
        GL.renderState().viewportArea.set(rp.viewport);
        GL.renderState().depthTest.set(rp.depthTest);
        GL.renderState().depthWrite.set(rp.depthWrite);
        if (rp.clear) {
            GL.ClearColor(rp.clearColor.x, rp.clearColor.y, rp.clearColor.z, rp.clearColor.w);
            GL.Clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        }
    }
};

}
}