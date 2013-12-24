#include "model_loader.h"
#include "base/log.h"

#include <assimp/cimport.h>
#include <assimp/Logger.hpp>
#include <assimp/DefaultLogger.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

class AiLog : public Assimp::Logger
{
protected:
    virtual void OnDebug(const char* message) {
        LOG("[assimp]: %s", message);
    }
    virtual void OnInfo(const char* message) {
        LOG("[assimp]: %s", message);
    }
    virtual void OnWarn(const char* message) {
        WARN("[assimp]: %s", message);
    }
    virtual void OnError(const char* message) {
        ERR("[assimp]: %s", message);
    }
    virtual bool attachStream(Assimp::LogStream *pStream, 
        unsigned int severity = Debugging | Err | Warn | Info) {
            return true;
    }
    virtual bool detatchStream(Assimp::LogStream *pStream, 
        unsigned int severity = Debugging | Err | Warn | Info) {
            return true;
    }
};

namespace base {
namespace opengl {

using namespace Assimp;

class Importer {
public:
    Importer(const std::string& filename) {
        DefaultLogger::set(new AiLog);
        scene_ = aiImportFile(filename.c_str(), 0);
    }
    bool importOk() const {
        return scene_ != nullptr && scene_->mRootNode != nullptr;
    }
    const aiScene* scene() const { return scene_; }
    ~Importer() {
        if (scene_ != nullptr)
            aiReleaseImport(scene_); 
        DefaultLogger::kill();
    }
private:
    const aiScene* scene_;
};

Model* ModelLoader::load(const std::string& filename)
{
    Importer imp(filename);
    if (!imp.importOk())
        return nullptr;
    const aiScene* scene = imp.scene();

    unsigned int ppFlags = 
        aiProcess_ImproveCacheLocality |
        aiProcess_RemoveRedundantMaterials |
        aiProcess_SplitLargeMeshes |
        aiProcess_Triangulate |
        aiProcess_SortByPType |
        aiProcess_OptimizeMeshes;
    const aiScene* ppScene = aiApplyPostProcessing(scene, ppFlags);
    if (ppScene == nullptr)
        return nullptr;



    return nullptr;
}

} // namespace base
} // namespace opengl