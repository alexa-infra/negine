/**
 * \file
 * \brief       Particle system
 * \author      Alexey Vasilyev <alexa.infra@gmail.com>
 * \copyright   MIT License
 **/
#include "particlesystemrenderer.h"
#include "glprogram.h"
#include "math/rect.h"

namespace base
{
namespace opengl
{

using base::math::Vector2;
using base::math::Vector3;
using base::math::Vector4;
using base::resource::ParticleSystem;
using base::resource::ParticleSystemSetting;
using base::resource::Particle;
using base::resource::ParticleList;

ParticleSystemRenderer::ParticleSystemRenderer( ParticleSystem* ps, TextureLoader* loader )
    : texture_( NULL )
    , vbo_( NULL )
    , mesh_(NULL)
    , ps_( ps )
{
    const ParticleSystemSetting& settings = ps_->settings;

    MeshBuilder builder;
    builder
        .addAttribute(VertexAttrs::tagPosition)
        .addAttribute(VertexAttrs::tagTexture)
        .addAttribute(VertexAttrs::tagColor);
    mesh_ = new MeshExt(builder, settings.max_count * 4, settings.max_count * 6);

    Vector2* tex = mesh_->findAttributeTyped<Vector2>(VertexAttrs::tagTexture);
    u16* index = mesh_->indices();

    for ( u32 i = 0, j = 0; i < settings.max_count * 6; i += 6, j += 4 ) {
        index[i+0] = j;
        index[i+1] = j + 1;
        index[i+2] = j + 2;
        index[i+3] = j + 2;
        index[i+4] = j + 3;
        index[i+5] = j;
        tex[j + 0].set( 0.f, 0.f );
        tex[j + 1].set( 1.f, 0.f );
        tex[j + 2].set( 1.f, 1.f );
        tex[j + 3].set( 0.f, 1.f );
    }

    vbo_ = new VertexBuffer;
    vbo_->EnableAttributeMesh( mesh_ );
    vbo_->SetIndexData( mesh_->indices(), mesh_->numIndexes() * sizeof(u16) );
    vbo_->SetVertexData( mesh_->data(), mesh_->rawSize() );
    vbo_->Load();

    texture_ = loader->Load( settings.texture );
}

ParticleSystemRenderer::~ParticleSystemRenderer()
{
    delete vbo_;
    delete mesh_;
}

void ParticleSystemRenderer::Draw( Program* program )
{
    program->set_uniform( base::opengl::UniformVars::Diffuse, texture_ );
    vbo_->BindAttributes( );

    for ( ParticleList::const_iterator it = ps_->particles_active.begin(); it != ps_->particles_active.end(); ++it ) {
        const Particle* p = *it;
        glDrawElements(
            GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, (u16*)0 + (p->face*3));
    }

    vbo_->UnbindAttributes( );
}

void ParticleSystemRenderer::Commit()
{
    Vector2 v[4];

    Vector3* pos = mesh_->findAttributeTyped<Vector3>(VertexAttrs::tagPosition);
    Vector4* color = mesh_->findAttributeTyped<Vector4>(VertexAttrs::tagColor);

    for ( ParticleList::const_iterator it = ps_->particles_active.begin(); it != ps_->particles_active.end(); ++it ) {
        const Particle* p = *it;
        base::math::RectF rectange( p->position, Vector2( p->size ), p->rotation );
        rectange.Points( v );
        pos[p->index + 0] = Vector3( v[0] );
        pos[p->index + 1] = Vector3( v[1] );
        pos[p->index + 2] = Vector3( v[2] );
        pos[p->index + 3] = Vector3( v[3] );
        color[p->index + 0] = p->color;
        color[p->index + 1] = p->color;
        color[p->index + 2] = p->color;
        color[p->index + 3] = p->color;
    }

    vbo_->SetVertexData( mesh_->data(), mesh_->rawSize() );
}

} // namespace opengl
} // namespace base

