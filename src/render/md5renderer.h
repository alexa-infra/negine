/**
 * \file
 * \brief       Md5 mesh renderer
 * \author      Alexey Vasilyev <alexa.infra@gmail.com>
 * \copyright   MIT License
 **/
#pragma once

#include "base/types.h"
#include "render/md5mesh.h"
#include "render/mesh.h"
#include "render/vertexbuffer.h"

namespace base
{
namespace opengl
{

//! single mesh only
class Md5Renderer
{
public:
    resource::Md5BoundingBox boundingBox;
private:
    MeshExt* mesh_;
    VertexBuffer* vb;
    resource::Md5Model* md5;
    DeviceContext& GL;
public:
    Md5Renderer( resource::Md5Model* model, DeviceContext& gl );
    ~Md5Renderer();

    void Commit();
    void Draw( );

private:
    void GenerateVertexes( resource::Md5Mesh& mesh );
    void GenerateIndexes( resource::Md5Mesh& mesh );
    void GenerateLightningInfo( resource::Md5Mesh& mesh );
    void UpdateBoundingBox( math::vec3f& vetex );
};

} // namespace base
} // namespace opengl
