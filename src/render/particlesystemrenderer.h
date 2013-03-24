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
class MeshExt;
class DeviceContext;

class ParticleSystemRenderer
{
    Texture* texture_;
    VertexBuffer* vbo_;
    MeshExt* mesh_;
    DeviceContext& GL;
    resource::ParticleSystem* ps_;
public:
    ParticleSystemRenderer( resource::ParticleSystem* ps, DeviceContext& gl );
    ~ParticleSystemRenderer();

    void Commit();
    void Draw( GpuProgram* binding );
};

}
}
