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

namespace base {
namespace opengl {

class ParticleSystemRenderer {
    Texture* texture_;
    VertexBuffer* vbo_;
    ParticleSystem* ps_;
public:
    ParticleSystemRenderer(ParticleSystem* ps);
    ~ParticleSystemRenderer();

    void Commit();
    void Draw(AttributeBinding& binding);
};

}
}
