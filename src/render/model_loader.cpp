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

Model* ModelLoader::load(const std::string& filename)
{
    using namespace Assimp;
    DefaultLogger::set(new AiLog);
    const aiScene * scene = aiImportFile(filename.c_str(), 0);

    if(!scene || !scene->mRootNode) {
        DefaultLogger::kill();
        return nullptr;
    }

    unsigned int ppFlags = 
        aiProcess_ImproveCacheLocality |
        aiProcess_RemoveRedundantMaterials |
        aiProcess_SplitLargeMeshes |
        aiProcess_Triangulate |
        aiProcess_SortByPType |
        aiProcess_OptimizeMeshes;

    if(!aiApplyPostProcessing(scene, ppFlags))
        return nullptr; 

    // copy model/material data here

    aiReleaseImport(scene); 
    DefaultLogger::kill();

    return nullptr;
}

} // namespace base
} // namespace opengl