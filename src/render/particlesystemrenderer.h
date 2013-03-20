/**
 * \file
 * \brief       Particle system renderer
 * \author      Alexey Vasilyev <alexa.infra@gmail.com>
 * \copyright   MIT License
 **/
#pragma once

#include "base/types.h"

namespace base
{
namespace resource
{
    class ParticleSystem;
}
namespace opengl
{

class Texture;
class TextureLoader;
class GpuProgram;
class VertexBuffer;
class MeshExt;

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
