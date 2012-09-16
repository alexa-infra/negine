/**
 * \file
 * \brief       Particle system renderer
 * \author      Alexey Vasilyev <alexa.infra@gmail.com>
 * \copyright   MIT License
 **/
#pragma once

#include "renderer/gltexture.h"
#include "renderer/vertexbuffer.h"
#include "renderer/particlesystem.h"

namespace base
{
namespace opengl
{

class Program;

class ParticleSystemRenderer
{
    Texture* texture_;
    VertexBuffer* vbo_;
    resource::ParticleSystem* ps_;
public:
    ParticleSystemRenderer( resource::ParticleSystem* ps, TextureLoader* loader );
    ~ParticleSystemRenderer();

    void Commit();
    void Draw( Program* binding );
};

}
}
