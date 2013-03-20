/**
 * \file
 * \brief       MD3 model renderer
 * \author      Alexey Vasilyev <alexa.infra@gmail.com>
 * \copyright   MIT License
 **/
#pragma once
#include "render/md3mesh.h"
#include "render/vertexbuffer.h"
#include "render/mesh.h"
#include <vector>

namespace base
{
namespace math
{
    class Vector3;
}

namespace opengl
{

class Md3Renderer
{
    resource::Md3Model* model;
    std::vector<MeshExt*> meshes;
    std::vector<VertexBuffer*> vbs;

public:
    Md3Renderer( resource::Md3Model* m );
    ~Md3Renderer();

    void Commit();

    void Draw( );
private:
    static void DecodeNormal( const u8* normal, math::Vector3& v );
};

}
}
