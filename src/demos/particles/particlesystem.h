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
    u32 maxCount;

    f32 lifetime;
    f32 lifetimeSpread;

    math::vec4f colorStart;
    math::vec4f colorEnd;

    f32 sizeStart;
    f32 sizeEnd;

    f32 speed;

    u32 emissionRate;
    std::string texture;

    ParticleSystemSetting();
};

struct Particle {
    f32 lifetime;
    f32 lifetimeLimit;

    math::vec2f position;
    math::vec2f speed;
    f32 acceleration;

    f32 size;
    f32 rotation;
    math::vec4f color;
};
typedef std::list<Particle*> ParticleList;

class ParticleSystem
{
public:
    ParticleSystemSetting settings;     //!< settings
    Particle* particles;                    //!< all particles
    ParticleList particlesFree;     //!< not used particles
    ParticleList particlesActive;   //!< used particles

    bool emissionActive;
    u32 emissionRate;          //!< particles/second
    f32 emissionTimer;         //!< emission time

    math::vec3f position;           //!< center position
public:
    ParticleSystem( ParticleSystemSetting s );

    ~ParticleSystem();

    void update( f32 frameTime );
protected:
    void add();
private:
    DISALLOW_COPY_AND_ASSIGN( ParticleSystem );
};

}
}
