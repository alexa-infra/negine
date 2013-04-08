/**
 * \file
 * \brief       Particle system
 * \author      Alexey Vasilyev <alexa.infra@gmail.com>
 * \copyright   MIT License
 **/
#pragma once

#include "base/types.h"
#include "math/vec2.h"
#include "math/vec3.h"
#include "math/vec4.h"
#include <list>
#include <string>

namespace base
{
namespace resource
{

struct ParticleSystemSetting {
    u32 max_count;

    f32 particle_lifetime;
    f32 particle_lifetime_spread;

    math::vec4f color_start;
    math::vec4f color_end;

    f32 size_start;
    f32 size_end;

    f32 speed;

    u32 emission_rate;
    f32 lifetime;

    std::string texture;

    ParticleSystemSetting();
};

struct Particle {
    f32 life_time;
    f32 life;

    math::vec2f position;
    math::vec2f speed;
    f32 acceleration;

    f32 size;
    f32 rotation;
    math::vec4f color;
};
typedef std::list<Particle*> ParticleList;

inline bool sort_particles_from_farest_to_nearest( Particle* first, Particle* second )
{
    return first->size < second->size;
}

class ParticleSystem
{
public:
    ParticleSystemSetting settings;     //!< settings
    Particle* particles;                    //!< all particles
    ParticleList particles_free;     //!< not used particles
    ParticleList particles_active;   //!< used particles

    bool emission_active;       //!< emission is enabled
    u32 emission_rate;          //!< particles/second
    f32 emission_timer;         //!< emission time

    f32 lifetime;               //!< particle system lifetime

    math::vec3f position;           //!< center position
public:
    ParticleSystem( ParticleSystemSetting s );

    ~ParticleSystem();

    void update( f32 frame_time );
protected:
    void add();
private:
    DISALLOW_COPY_AND_ASSIGN( ParticleSystem );
};

}
}
