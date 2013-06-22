/**
 * \file
 * \author      Alexey Vasilyev <alexa.infra@gmail.com>
 * \copyright   MIT License
 **/
#include "render/particlesystem.h"
#include <stdlib.h>

namespace base
{
namespace resource
{

ParticleSystemSetting::ParticleSystemSetting()
    : maxCount( 1000 )
    , lifetime( 1.0f )
    , lifetimeSpread( 0.2f )
    , colorStart( 1.f, 1.f, 1.f, 0.1f )
    , colorEnd( 1.f, 0.f, 0.f, 0.9f )
    , sizeStart( 1.0f )
    , sizeEnd( 12.f )
    , speed( 10.0f )
    , emissionRate( 50 )
{
}

ParticleSystem::ParticleSystem( ParticleSystemSetting s )
{
    particles = new Particle[s.maxCount];

    for ( u32 i = 0; i < s.maxCount; i++ ) {
        particlesFree.push_back( &particles[i] );
    }

    emissionRate = s.emissionRate;
    emissionTimer = 0.0f;
    settings = s;
    emissionActive = true;
}

ParticleSystem::~ParticleSystem()
{
    delete[] particles;
}

void ParticleSystem::add()
{
    if ( particlesFree.size() == 0 ) {
        return;
    }

    Particle* p = particlesFree.front();
    particlesFree.pop_front();
    p->lifetimeLimit = rand() / static_cast<f32>( RAND_MAX ) * ( 2 * settings.lifetimeSpread ) + ( settings.lifetime - settings.lifetimeSpread );
    p->lifetime = 0;
    f32 direction = rand() / static_cast<f32>( RAND_MAX ) * ( 2 * math::pi );
    p->position = position.xy(); 
    p->speed = math::vec2f( cosf( direction ), sinf( direction ) );
    p->speed *= settings.speed * ( rand() / static_cast<f32>( RAND_MAX ) );
    p->acceleration = rand() / static_cast<f32>( RAND_MAX ) * 200.0f;
    p->size = settings.sizeStart;
    p->rotation = 0.0f;
    p->color = settings.colorStart;
    particlesActive.push_back( p );
}

void ParticleSystem::update( f32 frameTime )
{
    if ( emissionActive ) {
        emissionTimer += frameTime;
        f32 rate = 1.f / emissionRate;

        while ( emissionTimer > rate ) {
            add();
            emissionTimer -= rate;
        }
    }

    for ( ParticleList::iterator it = particlesActive.begin(); it != particlesActive.end(); ) {
        Particle* p = *it;
        p->lifetime += frameTime;

        if ( p->lifetime > p->lifetimeLimit ) {
            particlesFree.push_back( p );
            it = particlesActive.erase( it );
            continue;
        }

        f32 t = p->lifetime / p->lifetimeLimit;
        p->size = math::slerp( settings.sizeStart, settings.sizeEnd, t );
        for(u32 i=0; i<4; i++)
            p->color[i] = math::slerp( settings.colorStart[i], settings.colorEnd[i], t );
        math::vec2f ppos = p->position - position.xy();
        p->position = p->speed * t + normalize( p->speed ) * p->acceleration * t * t / 2.0f;
        ++it;
    }

    particlesActive.sort( [](Particle* first, Particle* second) { return first->size < second->size; } );
}

} // opengl
} // base
