/**
 * @author  Alexey Vasilyev <alexa.infra@gmail.com>
 * @section LICENSE
 * 
 * This file is part of Negine (https://alexadotlife.com/hg/negine)
 * Licensed under LGPLv3 (or GPLv3) - see LICENSE file for details
 **/
#pragma once

#include "renderer/gltexture.h"
#include "renderer/vertexbuffer.h"
#include <list>

namespace base {
namespace opengl {

struct ParticleSystemSetting {
    u32 max_count;

    f32 particle_lifetime;
    f32 particle_lifetime_spread;

    Vector4 color_start;
    Vector4 color_end;

    f32 size_start;
    f32 size_end;

    f32 speed;

    u32 emission_rate;
    f32 lifetime;

    ParticleSystemSetting();
};

struct Particle {
    u32 index;

    f32 life_time;
    f32 life;
    
    Vector2 position;
    Vector2 speed;
    f32 acceleration;
    
    f32 size;
    f32 rotation;
    Vector4 color;
};

inline bool sort_particles_from_farest_to_nearest(Particle* first, Particle* second) {
    return first->size < second->size;
}

class ParticleSystem {
public:
    ParticleSystemSetting settings;     //!< settings
    Texture* texture;                   //!< texture
    VertexBuffer* vbo;                  //!< vertexes

    Particle* particles;                    //!< all particles
    std::list<Particle*> particles_free;     //!< not used particles
    std::list<Particle*> particles_active;   //!< used particles

    bool emission_active;       //!< emission is enabled
    u32 emission_rate;          //!< particles/second
    f32 emission_timer;         //!< emission time

    f32 lifetime;               //!< particle system lifetime

    Vector3 position;           //!< center position

public:
    ParticleSystem(ParticleSystemSetting s);

    ~ParticleSystem();

    void Draw(AttributeBinding& binding, f32 frame_time);

protected:
    void add();

    void update(f32 frame_time);
};

}
}
