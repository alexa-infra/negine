/**
 * \file
 * \author      Alexey Vasilyev <alexa.infra@gmail.com>
 * \copyright   MIT License
 **/
#include "render/particlesystem.h"
#include <stdlib.h>
#include "math/vector-inl.h"

namespace base
{
namespace resource
{

ParticleSystemSetting::ParticleSystemSetting()
    : max_count( 1000 )
    , particle_lifetime( 1.0f )
    , particle_lifetime_spread( 0.2f )
    , color_start( 1.f, 1.f, 1.f, 0.1f )
    , color_end( 1.f, 0.f, 0.f, 0.9f )
    , size_start( 1.0f )
    , size_end( 12.f )
    , speed( 10.0f )
    , emission_rate( 50 )
    , lifetime( 100000000000.f )
{
}

ParticleSystem::ParticleSystem( ParticleSystemSetting s )
{
    particles = new Particle[s.max_count];

    for ( u32 i = 0; i < s.max_count; i++ ) {
        particles[i].index = 4 * i;
        particles[i].face = 2 * i;
        particles_free.push_back( &particles[i] );
    }

    emission_active = true;
    emission_rate = s.emission_rate;
    emission_timer = 0.f;
    lifetime = s.lifetime;
    settings = s;
}

ParticleSystem::~ParticleSystem()
{
    delete[] particles;
}

void ParticleSystem::add()
{
    if ( particles_free.size() == 0 ) {
        return;
    }

    Particle* p = particles_free.front();
    particles_free.pop_front();
    p->life_time = rand() / ( f32 )( RAND_MAX ) * ( 2 * settings.particle_lifetime_spread ) + ( settings.particle_lifetime - settings.particle_lifetime_spread );
    p->life = 0;
    f32 direction = rand() / ( f32 )( RAND_MAX ) * ( 2 * math::pi );
    p->position = math::Vector2( position );
    p->speed = math::Vector2( cosf( direction ), sinf( direction ) );
    p->speed *= settings.speed * ( rand() / ( f32 )( RAND_MAX ) );
    p->acceleration = 0.f;
    p->size = settings.size_start;
    p->rotation = 0.f;
    p->color = settings.color_start;
    particles_active.push_back( p );
}

void ParticleSystem::update( f32 frame_time )
{
    if ( emission_active ) {
        emission_timer += frame_time;
        f32 rate = 1.f / emission_rate;

        while ( emission_timer > rate ) {
            add();
            emission_timer -= rate;
        }

        lifetime -= frame_time;

        if ( lifetime < 0 ) {
            emission_active = false;
        }
    }

    for ( ParticleList::iterator it = particles_active.begin(); it != particles_active.end(); ) {
        Particle* p = *it;
        p->life += frame_time;

        if ( p->life > p->life_time ) {
            particles_free.push_back( p );
            it = particles_active.erase( it );
            continue;
        }

        f32 t = p->life / p->life_time;
        p->size = settings.size_start * ( 1 - t ) + settings.size_end * t;
        p->color[0] = settings.color_start[0] * ( 1 - t ) + settings.color_end[0] * t;
        p->color[1] = settings.color_start[1] * ( 1 - t ) + settings.color_end[1] * t;
        p->color[2] = settings.color_start[2] * ( 1 - t ) + settings.color_end[2] * t;
        p->color[3] = settings.color_start[3] * ( 1 - t ) + settings.color_end[3] * t;
        math::Vector2 ppos = p->position - math::Vector2( position );
        ppos += p->speed * p->life;
        p->position = ppos + math::Vector2( position );
        ++it;
    }

    particles_active.sort( sort_particles_from_farest_to_nearest );
}

} // opengl
} // base
