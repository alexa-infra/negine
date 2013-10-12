/**
 * \file
 * \brief       Md5 mesh renderer
 * \author      Alexey Vasilyev <alexa.infra@gmail.com>
 * \copyright   MIT License
 **/
#pragma once

#include "base/types.h"
#include "md5mesh.h"
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
    Mesh* mesh_;
    VertexBuffer* vb;
    resource::Md5Model* md5;
    DeviceContext& GL;
public:
    Md5Renderer( resource::Md5Model* model, DeviceContext& gl );
    ~Md5Renderer();

    void commit( );
    void draw( );

private:
    void generateVertexes( resource::Md5Mesh& mesh );
    void generateIndexes( resource::Md5Mesh& mesh );
    void generateLightningInfo( resource::Md5Mesh& mesh );
    void updateBoundingBox( math::vec3f& vetex );
};

} // namespace base
} // namespace opengl
