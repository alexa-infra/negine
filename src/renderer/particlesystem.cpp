/**
 * \file
 * \author      Alexey Vasilyev <alexa.infra@gmail.com>
 * \copyright   MIT License
 **/
#include "renderer/particlesystem.h"

#include "renderer/gltexture.h"
#include "renderer/vertexbuffer.h"
#include "base/math/rect.h"
#include <list>
#include <stdlib.h>
#include <iostream>

namespace base {
namespace opengl {

ParticleSystemSetting::ParticleSystemSetting()
    : max_count(1000)
    , particle_lifetime(1.0f)
    , particle_lifetime_spread(0.2f)
    , color_start(1.f, 1.f, 1.f, 0.1f)
    , color_end(1.f, 0.f, 0.f, 0.9f)
    , size_start(1.0f)
    , size_end(12.f)
    , speed(10.0f)
    , emission_rate(50)
    , lifetime(100000000000.f)
{
}

ParticleSystem::ParticleSystem(ParticleSystemSetting s) {
    particles = new Particle[s.max_count];
    for (u32 i=0; i<s.max_count; i++) {
        particles[i].index = 4 * i;
        particles[i].face = 2 * i;
        particles_free.push_back(&particles[i]);
    }
    Vertex* v = new Vertex[s.max_count * 4];
    Face* f = new Face[s.max_count * 2];
    emission_active = true;
    emission_rate = s.emission_rate;
    emission_timer = 0.f;
    lifetime = s.lifetime;
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
    }

    vbo = new VertexBufferMemory(v, s.max_count * 4, f, s.max_count * 2);
    delete[] v;
    delete[] f;
}

ParticleSystem::~ParticleSystem() {
    delete[] particles;
    delete vbo;
}

void ParticleSystem::add() {
    if (particles_free.size() == 0)
        return;
    Particle* p = particles_free.front();
    particles_free.pop_front();

    p->life_time = rand() / (f32)(RAND_MAX) * (2 * settings.particle_lifetime_spread) + (settings.particle_lifetime - settings.particle_lifetime_spread);
    p->life = 0;

    f32 direction = rand() / (f32)(RAND_MAX) * (2 * math::pi);
    p->position = Vector2(position);
    p->speed = Vector2(cosf(direction), sinf(direction)) * (rand() / (f32)(RAND_MAX));
    p->speed.Normalize();
    p->acceleration = 0.f;

    p->size = settings.size_start;
    p->rotation = 0.f;
    p->color = settings.color_start;

    particles_active.push_back(p);
}

void ParticleSystem::Draw(AttributeBinding& binding) {
    vbo->BindAttributes(binding);
    for (auto it = particles_active.begin(); it != particles_active.end(); ++it) {
        Particle* p = *it;
        vbo->DrawOnly(p->face, 2);
    }
    vbo->UnbindAttributes(binding);
}

void ParticleSystem::update(f32 frame_time) {
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

    Vector2 v[4];
    Vertex* vertex = vbo->Lock();
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
        ppos += p->speed * p->life;
        p->position = ppos + Vector2(position);

        base::math::RectF rectange(p->position, Vector2(p->size), p->rotation);
        rectange.Points(v);

        vertex[p->index + 0].pos = Vector3(v[0]);
        vertex[p->index + 1].pos = Vector3(v[1]);
        vertex[p->index + 2].pos = Vector3(v[2]);
        vertex[p->index + 3].pos = Vector3(v[3]);

        vertex[p->index + 0].color = p->color;
        vertex[p->index + 1].color = p->color;
        vertex[p->index + 2].color = p->color;
        vertex[p->index + 3].color = p->color;

        ++it;
    }
    vbo->Unlock();

    particles_active.sort(sort_particles_from_farest_to_nearest);
}

} // opengl
} // base
