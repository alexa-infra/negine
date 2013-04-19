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
class GpuProgram;
class VertexBuffer;
class Mesh;
class DeviceContext;

class ParticleSystemRenderer
{
    Texture* texture_;
    VertexBuffer* vbo_;
    Mesh* mesh_;
    DeviceContext& GL;
    resource::ParticleSystem* ps_;
public:
    ParticleSystemRenderer( resource::ParticleSystem* ps, DeviceContext& gl );
    ~ParticleSystemRenderer();

    Texture* texture();
    void Commit();
    void Draw( GpuProgram* binding );
};

}
}
