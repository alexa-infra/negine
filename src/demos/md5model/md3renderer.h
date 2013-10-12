/**
 * \file
 * \brief       MD3 model renderer
 * \author      Alexey Vasilyev <alexa.infra@gmail.com>
 * \copyright   MIT License
 **/
#pragma once
#include "md3mesh.h"
#include "render/vertexbuffer.h"
#include "render/mesh.h"
#include <vector>
#include "math/vec3.h"

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

    void Draw( );
private:
    static void DecodeNormal( const u8* normal, math::vec3f& v );
};

}
}
