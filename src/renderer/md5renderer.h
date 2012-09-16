/**
 * \file
 * \brief       Md5 mesh renderer
 * \author      Alexey Vasilyev <alexa.infra@gmail.com>
 * \copyright   MIT License
 **/
#pragma once

#include "base/types.h"
#include "renderer/md5mesh.h"
#include "renderer/mesh.h"
#include "renderer/vertexbuffer.h"

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
    Vertex* vertexArray;
    Face* vertexIndices;
    VertexBuffer* vb;
    resource::Md5Model* md5;
public:
    Md5Renderer( resource::Md5Model* model );
    ~Md5Renderer();

    void Commit();
    void Draw( AttributeBinding& binding );

private:
    void GenerateVertexes( resource::Md5Mesh& mesh, Vertex* vertexes );
    void GenerateIndexes( resource::Md5Mesh& mesh, Face* indexes );
    void GenerateLightningInfo( resource::Md5Mesh& mesh, Vertex* vertexes );
    void UpdateBoundingBox( Vertex& vetex );
};

} // namespace base
} // namespace opengl