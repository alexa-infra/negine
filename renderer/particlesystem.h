/**
 * @author  Alexey Vasilyev <alexa.infra@gmail.com>
 * @section LICENSE
 * 
 * This file is part of Negine (https://alexadotlife.com/hg/negine)
 * Licensed under LGPLv3 (or GPLv3) – see LICENSE file for details
 **/
#pragma once

#include "renderer/spritegroup.h"

#include <list>

namespace base {
namespace opengl {

struct ParticleSystemSetting {
    u32 max_count;

    f32 number;
    f32 life_time;
    f32 life_time_spread;

    Vector2 direction;
    Vector2 direction_spread;

    Vector4 color;
    f32 size;

    f32 speed;

    ParticleSystemSetting()
        : number(100)
        , life_time(10000.f)
        , life_time_spread(3000.f)
        , direction(1, 1)
        , direction_spread(0.1, 0.1)
        , speed(1.f)
    {
    }
};

struct Particle {
    u32 sprite_index;
    f32 life_time;
    f32 current_life;
    Vector3 direction;
};

class ParticleSystem {
public:
    ParticleSystemSetting settings_;
    std::vector<Particle*> particles_;
    std::list<Particle*> alive_particles_;
    std::stack<u32> free_indexes_;
    SpriteGroup* sg_;
    
    ParticleSystem() {
        for (u32 i=0; i<settings_.max_count; i++)
        {
            particles_.push_back(new Particle);
            free_indexes_.push(i);
        }
        sg_ = new SpriteGroup(settings_.max_count);
    }

    ~ParticleSystem() {
        for (u32 i=0; i<settings_.max_count; i++)
            delete particles_[i];
        delete sg_;
    }

    void Draw(AttributeBinding& binding, f32 frame_time) {
        
    }
};

}
}