#include "engine.h"
#include "render/glcontext.h"
#include "engine/resource.h"
#include "engine/model_loader.h"
#include "engine/texture_loader.h"

namespace base {

using namespace opengl;

Engine* Engine::instance_ = nullptr;

Engine::Engine(DeviceContext& context) : GL(context) {
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
}

Engine::~Engine() {
    ResourceManager::shutdown();
}

void Engine::init(DeviceContext& context)
{
    instance_ = new Engine(context);
}

void Engine::shutdown()
{
    delete instance_;
    instance_ = nullptr;
}

} // namespace base