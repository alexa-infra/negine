#include "engine.h"
#include "render/glcontext.h"
#include "engine/resource.h"
#include "engine/model_loader.h"
#include "engine/texture_loader.h"
#include "render/material.h"
#include "foundation/memory.h"
#include "physics/physics.h"

namespace base {

using namespace opengl;

Engine* Engine::instance_ = nullptr;

opengl::DeviceContext& Engine::context() {
    return instance().GL;
}

opengl::Renderer& Engine::renderer() {
    return *(instance().renderer_);
}

phys::Physics& Engine::physics() {
    return *(instance().physics_);
}

Engine::Engine(DeviceContext& context) : GL(context) {
    foundation::memory_globals::init();
    ResourceManager::addFactory(Model::Type(), [](const std::string& p) { 
        Model* model = loadModel(p);
        return dynamic_cast<Resource*>(model);
    });
    ResourceManager::addFactory(Texture::Type(), [](const std::string& p) { 
        TextureInfo defaultSettings;
        defaultSettings.Filtering = TextureFilters::Anisotropic;
        defaultSettings.GenerateMipmap = true;
        Texture* texture = loadTexture(Engine::context(), defaultSettings, p);
        return dynamic_cast<Resource*>(texture);
    });
    renderer_ = new Renderer(GL);
    physics_ = new phys::Physics();
}

Engine::~Engine() {
    delete physics_;
    delete renderer_;
    ResourceManager::shutdown();
    foundation::memory_globals::shutdown();
}

} // namespace base