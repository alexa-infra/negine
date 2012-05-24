/**
 * \file
 * \brief       Particle system
 * \author      Alexey Vasilyev <alexa.infra@gmail.com>
 * \copyright   MIT License
 **/
#include "particlesystemrenderer.h"
#include "base/math/rect.h"

namespace base {
namespace opengl {

using base::resource::ParticleSystem;
using base::resource::ParticleSystemSetting;
using base::resource::Particle;
using base::resource::ParticleList;

ParticleSystemRenderer::ParticleSystemRenderer(ParticleSystem* ps)
    : texture_(NULL)
    , vbo_(NULL)
    , ps_(ps)
{
    const ParticleSystemSetting& settings = ps_->settings;
    Vertex* v = new Vertex[settings.max_count * 4];
    Face* f = new Face[settings.max_count * 2];

    for (u32 i=0, j=0; i<settings.max_count * 2; i+=2, j+=4) {
        f[i].index[0] = j;
        f[i].index[1] = j+1;
        f[i].index[2] = j+2;

        f[i+1].index[0] = j+2;
        f[i+1].index[1] = j+3;
        f[i+1].index[2] = j;

        v[j+0].tex.set(0.f, 0.f);
        v[j+1].tex.set(1.f, 0.f);
        v[j+2].tex.set(1.f, 1.f);
        v[j+3].tex.set(0.f, 1.f);
    }

    vbo_ = new VertexBufferMemory(v, settings.max_count * 4, f, settings.max_count * 2);
    delete[] v;
    delete[] f;
}

ParticleSystemRenderer::~ParticleSystemRenderer()
{
    delete texture_;
    delete vbo_;
}

void ParticleSystemRenderer::Draw(AttributeBinding& binding) {
    vbo_->BindAttributes(binding);
    for (ParticleList::iterator it = ps_->particles_active.begin(); it != ps_->particles_active.end(); ++it) {
        Particle* p = *it;
        vbo_->DrawOnly(p->face, 2);
    }
    vbo_->UnbindAttributes(binding);
}

void ParticleSystemRenderer::Commit()
{
    Vector2 v[4];
    Vertex* vertex = vbo_->Lock();
    for (ParticleList::iterator it = ps_->particles_active.begin(); it != ps_->particles_active.end(); ++it) {
        Particle* p = *it;

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
    }
    vbo_->Unlock();
}

} // namespace opengl
} // namespace base

