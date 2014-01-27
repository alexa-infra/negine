#include "model_loader.h"
#include "base/log.h"
#include "engine/resourceref.h"

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

Model* loadModel(const std::string& filename)
{
    Importer imp(filename);
    if (!imp.importOk())
        return nullptr;
    const aiScene* scene = imp.scene();

    unsigned int ppFlags = 
        aiProcess_ImproveCacheLocality |
        aiProcess_RemoveRedundantMaterials |
        aiProcess_SplitLargeMeshes |
        aiProcess_GenSmoothNormals |
        //aiProcess_GenNormals |
        aiProcess_JoinIdenticalVertices |
        aiProcess_Triangulate |
        aiProcess_SortByPType |
        aiProcess_OptimizeMeshes;
    const aiScene* ppScene = aiApplyPostProcessing(scene, ppFlags);
    if (ppScene == nullptr)
        return nullptr;

    Model* model = new Model;
    for(u32 i=0; i<scene->mNumMeshes; i++) {
        aiMesh* subMesh = scene->mMeshes[i];
        Model::Surface& surface = model->beginSurface();
        Mesh& m = surface.mesh;

        m.material_ = ResourceRef("default_material");

        if (subMesh->HasPositions())
            m.addAttribute(VertexAttrs::tagPosition);
        if (subMesh->HasNormals())
            m.addAttribute(VertexAttrs::tagNormal);
        m.vertexCount(subMesh->mNumVertices);
        m.indexCount(subMesh->mNumFaces * 3, IndexTypes::UInt32);
        m.complete();
        if (subMesh->HasPositions()) {
            math::vec3f* posData = m.findAttribute<math::vec3f>(VertexAttrs::tagPosition);
            for(u32 j=0; j<subMesh->mNumVertices; j++) {
                const aiVector3D& p = subMesh->mVertices[j];
                posData[j] = math::vec3f(p.x, p.y, p.z);
            }
        }
        if (subMesh->HasNormals()) {
            math::vec3f* normData = m.findAttribute<math::vec3f>(VertexAttrs::tagNormal);
            for(u32 j=0; j<subMesh->mNumVertices; j++) {
                const aiVector3D& p = subMesh->mNormals[j];
                normData[j] = math::vec3f(p.x, p.y, p.z);
            }
        }
        u32* indices = reinterpret_cast<u32*>(m.indices());
        for(u32 f=0; f<subMesh->mNumFaces; f++)
        {
            aiFace& face = subMesh->mFaces[f];
            for(u32 v=0; v<3; v++){
                indices[f*3+v] = face.mIndices[v];
            }
        }   

        model->endSurface();
    }

    return model;
}

} // namespace base
} // namespace opengl