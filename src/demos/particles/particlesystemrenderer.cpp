/**
 * \file
 * \brief       Particle system
 * \author      Alexey Vasilyev <alexa.infra@gmail.com>
 * \copyright   MIT License
 **/
#include "particlesystemrenderer.h"
#include "particlesystem.h"
#include "gpuprogram.h"
#include "math/rect.h"
#include "render/texture.h"
#include "render/vertexbuffer.h"
#include "render/mesh.h"
#include "math/vec4.h"

namespace base
{
namespace opengl
{

using base::math::vec2f;
using base::math::vec3f;
using base::math::vec4f;
using base::resource::ParticleSystem;
using base::resource::ParticleSystemSetting;
using base::resource::Particle;
using base::resource::ParticleList;

ParticleSystemRenderer::ParticleSystemRenderer( ParticleSystem* ps, DeviceContext& gl )
    : texture_( NULL )
    , vbo_( NULL )
    , mesh_(NULL)
    , GL(gl)
    , ps_( ps )
{
    const ParticleSystemSetting& settings = ps_->settings;

    mesh_ = new Mesh();
    (*mesh_)
        .addAttribute(VertexAttrs::tagPosition)
        .addAttribute(VertexAttrs::tagTexture)
        .addAttribute(VertexAttrs::tagColor)
        .vertexCount(settings.maxCount * 4)
        .indexCount(settings.maxCount * 6, IndexTypes::UInt16)
        .complete();

    vec2f* tex = mesh_->findAttributeTyped<vec2f>(VertexAttrs::tagTexture);
    u16* index = reinterpret_cast<u16*>(mesh_->indices());

    for ( u32 i = 0, j = 0; i < settings.maxCount * 6; i += 6, j += 4 ) {
        index[i+0] = j;
        index[i+1] = j + 1;
        index[i+2] = j + 2;
        index[i+3] = j + 2;
        index[i+4] = j + 3;
        index[i+5] = j;
        tex[j + 0] = vec2f( 0.f, 0.f );
        tex[j + 1] = vec2f( 1.f, 0.f );
        tex[j + 2] = vec2f( 1.f, 1.f );
        tex[j + 3] = vec2f( 0.f, 1.f );
    }

    vbo_ = new VertexBuffer(GL);
    vbo_->enableAttributeMesh( mesh_ );
    vbo_->setIndexData( mesh_->indices(), mesh_->numIndexes() * sizeof(u16) );
    vbo_->setVertexData( mesh_->data(), mesh_->rawSize() );

    texture_ = GL.texture_loader()->load( settings.texture );
}

ParticleSystemRenderer::~ParticleSystemRenderer()
{
    delete vbo_;
    delete mesh_;
}

Texture* ParticleSystemRenderer::texture()
{
    return texture_;
}

void ParticleSystemRenderer::draw( GpuProgram* program )
{
    vbo_->bind( );

    // TODO: Uint32?
    GL.DrawElements(
        GL_TRIANGLES, ps_->particlesActive.size() * 6, GL_UNSIGNED_SHORT, NULL);

    vbo_->unbind( );
}

void ParticleSystemRenderer::commit()
{
    vec2f v[4];

    vec3f* pos = mesh_->findAttributeTyped<vec3f>(VertexAttrs::tagPosition);
    vec4f* color = mesh_->findAttributeTyped<vec4f>(VertexAttrs::tagColor);

    u32 idx = 0;
    for ( ParticleList::const_iterator it = ps_->particlesActive.begin(); it != ps_->particlesActive.end(); ++it, idx += 4 ) {
        const Particle* p = *it;
        base::math::RectF rectange( p->position, vec2f( p->size ), p->rotation );
        rectange.Points( v );
        for( u32 i=0; i<4; i++ ) {
            pos[idx + i] = vec3f( v[i], 0.0f );
            color[idx + i] = p->color;    
        }
    }

    vbo_->setVertexData( mesh_->data(), mesh_->rawSize() );
}

} // namespace opengl
} // namespace base

