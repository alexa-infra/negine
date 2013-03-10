/**
 * \file
 * \brief       Particle system renderer
 * \author      Alexey Vasilyev <alexa.infra@gmail.com>
 * \copyright   MIT License
 **/
#pragma once

#include "render/texture.h"
#include "render/vertexbuffer.h"
#include "render/particlesystem.h"

namespace base
{
namespace opengl
{

class GpuProgram;

class ParticleSystemRenderer
{
    Texture* texture_;
    VertexBuffer* vbo_;
    MeshExt* mesh_;
    resource::ParticleSystem* ps_;
public:
    ParticleSystemRenderer( resource::ParticleSystem* ps, TextureLoader* loader );
    ~ParticleSystemRenderer();

    void Commit();
    void Draw( GpuProgram* binding );
};

}
}
