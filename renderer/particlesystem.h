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
#include "base/math/rect.h"
#include <list>
#include <stdlib.h>

namespace base {
namespace opengl {

struct ParticleSystemSetting {
    u32 max_count;

    f32 life_time;
    f32 life_time_spread;

    Vector4 color_start;
    Vector4 color_end;

    f32 size_start;
    f32 size_end;

    f32 speed;

    ParticleSystemSetting()
        : max_count(1000)
        , life_time(200.f)
        , life_time_spread(50.f)
        , color_start(1, 0, 0, 1)
        , color_end(1, 1, 1, 1)
        , size_start(0.1f)
        , size_end(1.f)
        , speed(0.1f)
    {
    }
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
    ParticleSystem(ParticleSystemSetting s) {
        particles = new Particle[s.max_count];
        for (u32 i=0; i<s.max_count; i++) {
            particles[i].index = i;
            particles_free.push_back(&particles[i]);
        }
        //particles_free.insert( particles_free.begin(), &(), &(particles[0]) + s.max_count );
        Vertex* v = new Vertex[s.max_count * 4];
        Face* f = new Face[s.max_count * 2];
        emission_active = true;
        emission_rate = 10;
        emission_timer = 0.f;
        lifetime = 100000000000.f;
        settings = s;

        for (u32 i=0, j=0; i<s.max_count * 2; i+=2, j+=4) {
            f[i].index[0] = j;
            f[i].index[1] = j+1;
            f[i].index[2] = j+2;

            f[i+1].index[0] = j+2;
            f[i+1].index[1] = j+3;
            f[i+1].index[2] = j;

            v[j + 0].tex.set(0.f, 0.f);
            v[j + 1].tex.set(1.f, 0.f);
            v[j + 2].tex.set(1.f, 1.f);
            v[j + 3].tex.set(0.f, 1.f);

            v[j + 0].n.set(0.f, 0.f, 1.f);
            v[j + 1].n.set(0.f, 0.f, 1.f);
            v[j + 2].n.set(0.f, 0.f, 1.f);
            v[j + 3].n.set(0.f, 0.f, 1.f);
        }

        vbo = new VertexBuffer(v, s.max_count * 4, f, s.max_count * 2);
        delete[] v;
        delete[] f;
    }

    ~ParticleSystem() {
        delete[] particles;
        delete vbo;
    }

    void add() {
        if (particles_free.size() == 0)
            return;
        Particle* p = particles_free.front();
        particles_free.pop_front();

        p->life_time = rand() / (f32)(RAND_MAX + 1) * (2 * settings.life_time_spread) + (settings.life_time - settings.life_time_spread);
        p->life = 0;

        f32 direction = rand() / (f32)(RAND_MAX + 1) * (2 * math::pi);
        p->position = Vector2(position);
        p->speed = Vector2(cosf(direction), sinf(direction)) * (rand() / (f32)(RAND_MAX + 1)) *0.1f;
        p->acceleration = 0.f;

        p->size = settings.size_start;
        p->rotation = 0.f;
        p->color = settings.color_start;

        particles_active.push_back(p);
    }

    void Draw(AttributeBinding& binding, f32 frame_time) {
        update(frame_time);
        //vbo->Draw(binding);
        //return;
        vbo->BindAttributes(binding);
        for (auto it = particles_active.begin(); it != particles_active.end(); ++it) {
            Particle* p = *it;
            vbo->DrawOnly(p->index * 4, 2);

        }
        vbo->UnbindAttributes(binding);
    }

    void update(f32 frame_time) {
        if (emission_active) {
            emission_timer += frame_time;
            f32 rate = 1.f / emission_rate;
            while (emission_timer > rate) {
                add();
                emission_timer -= rate;
            }
            lifetime -= frame_time;
            if (lifetime < 0) {
                emission_active = false;
            }
        }
        for (auto it = particles_active.begin(); it != particles_active.end();) {
            Particle* p = *it;

            p->life += frame_time;
            if (p->life > p->life_time) {
                particles_free.push_back(p);
                it = particles_active.erase(it);
                continue;
            }

            f32 t = p->life / p->life_time;
            
            p->size = settings.size_start * (1-t) + settings.size_end * t;
            
            p->color[0] = settings.color_start[0] * (1-t) + settings.color_end[0] * t;
            p->color[1] = settings.color_start[1] * (1-t) + settings.color_end[1] * t;
            p->color[2] = settings.color_start[2] * (1-t) + settings.color_end[2] * t;
            p->color[3] = settings.color_start[3] * (1-t) + settings.color_end[3] * t;
            
            Vector2 ppos = p->position - Vector2(position);
            ppos += p->speed * frame_time;
            p->position = ppos;

            ++it;
        }

        Vector2 v[4];
        Vertex* vertex = vbo->Lock();
        for (auto it = particles_active.begin(); it != particles_active.end(); ++it) {
            Particle* p = *it;
            
            base::math::RectF rectange(p->position, Vector2(p->size), p->rotation);
            rectange.Points(v);

            vertex[4 * p->index + 0].pos = Vector3(v[0]);
            vertex[4 * p->index + 1].pos = Vector3(v[1]);
            vertex[4 * p->index + 2].pos = Vector3(v[2]);
            vertex[4 * p->index + 3].pos = Vector3(v[3]);

            vertex[4 * p->index + 0].color = p->color;
            vertex[4 * p->index + 1].color = p->color;
            vertex[4 * p->index + 2].color = p->color;
            vertex[4 * p->index + 3].color = p->color;

        }
        vbo->Unlock();
    }
};

}
}