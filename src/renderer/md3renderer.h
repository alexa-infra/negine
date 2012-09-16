/**
 * \file
 * \brief       MD3 model renderer
 * \author      Alexey Vasilyev <alexa.infra@gmail.com>
 * \copyright   MIT License
 **/
#pragma once
#include "renderer/md3mesh.h"
#include "renderer/vertexbuffer.h"
#include "renderer/mesh.h"
#include <vector>

namespace base
{
namespace opengl
{

class Md3Renderer
{
    resource::Md3Model* model;
    std::vector<Mesh*> meshes;
    std::vector<VertexBuffer*> vbs;

public:
    Md3Renderer( resource::Md3Model* m );
    ~Md3Renderer();

    void Commit();

    void Draw( AttributeBinding& binding );
private:
    static Vector3 DecodeNormal( const u8* normal );
};

}
}