/**
 * \file
 * \brief       vertex buffer obejct wrapper
 * \author      Alexey Vasilyev <alexa.infra@gmail.com>
 * \copyright   MIT License
 **/
#include "render/vertexbuffer.h"
#include "render/glcontext.h"
#include "render/bufferobject.h"
#include "render/statistics.h"
#include <string.h>

namespace base
{
namespace opengl
{

VertexBuffer::VertexBuffer(DeviceContext& gl)
    : vertexes_(nullptr)
    , indexes_(nullptr)
    , GL(gl)
#ifdef USE_VAO
    , vao_(0)
#endif
{
}

VertexBuffer::~VertexBuffer()
{
#ifdef USE_VAO
    GL.DeleteVertexArrays(1, &vao_);
#endif
    delete vertexes_;
    delete indexes_;
}

void VertexBuffer::enableAttribute( VertexAttr attr, u32 stride, void* pointer )
{
    enabledAttributes_[static_cast<u32>(attr)] = EnabledAttribute(stride, pointer);
}

void VertexBuffer::enableAttributeMesh( const Mesh* mesh )
{
    const MeshLayer* attributes = mesh->attributes();
    for (u32 i=0; i<VertexAttrs::Count; i++)
    {
        if (!attributes[i].valid_)
            continue;
        enableAttribute(attributes[i].attr_, attributes[i].stride_, reinterpret_cast<u8*>(NULL) + attributes[i].start_);
    }
}

void VertexBuffer::setVertexData( void* vertexes, u32 vertexCount )
{
    if (vertexes_ == nullptr)
        vertexes_ = new BufferObject(GL);
    vertexes_->bind( BufferTargets::Array );
    vertexes_->setData( vertexCount, vertexes, BufferUsages::DynamicDraw );
    vertexes_->unbind();
}

void VertexBuffer::setIndexData( void* index, u32 indexCount )
{
    if (indexes_ == nullptr)
        indexes_ = new BufferObject(GL);
    indexes_->bind( BufferTargets::ElementArray );
    indexes_->setData( indexCount, index, BufferUsages::DynamicDraw );
    indexes_->unbind();
}

void VertexBuffer::bind( )
{
#ifdef USE_VAO
    if (vao_ != 0) {
        GL.BindVertexArray(vao_);
        return;
    }
    
    ASSERT(vertexes_ != nullptr && indexes_ != nullptr);
    GL.GenVertexArrays(1, &vao_);
    GL.BindVertexArray(vao_);

    vertexes_->bind( BufferTargets::Array );

    for (u32 i=0; i<VertexAttrs::Count; i++)
    {
        const EnabledAttribute& attr = enabledAttributes_[i];
        VertexAttr vertexAttr = static_cast<VertexAttr>(i);
        u32 location = VertexAttrs::GetAttributeLocation( vertexAttr );
        if (attr.enabled_)
        {
            GL.EnableVertexAttribArray( location );
            GL.VertexAttribPointer(
                location,
                VertexAttrs::GetComponentCount( vertexAttr ),
                VertexAttrs::GetGLType( vertexAttr ),
                GL_FALSE,
                attr.stride_,
                attr.pointer_ );
        }
        else
        {
            GL.DisableVertexAttribArray(location);
        }
    }

    indexes_->bind( BufferTargets::ElementArray );
#else
    vertexes_->bind( BufferTargets::Array );

    for (u32 i=0; i<VertexAttrs::Count; i++)
    {
        EnabledAttribute& attr = enabledAttributes_[i];
        VertexAttr vertexAttr = static_cast<VertexAttr>(i);
        u32 location = VertexAttrs::GetAttributeLocation( vertexAttr );
        if (enabledAttributes_[i].enabled_)
        {
            GL.EnableVertexAttribArray( location );
            GL.VertexAttribPointer(
                location,
                VertexAttrs::GetComponentCount( vertexAttr ),
                VertexAttrs::GetGLType( vertexAttr ),
                GL_FALSE,
                attr.stride_,
                attr.pointer_ );
        }
        else
        {
            GL.DisableVertexAttribArray(location);
        }
    }

    indexes_->bind( BufferTargets::ElementArray );
#endif
}

void VertexBuffer::unbind( )
{
#ifdef USE_VAO
    // TODO: do we need this?
    GL.BindVertexArray(0);
#else
    indexes_->unbind( );
    vertexes_->unbind( );
#endif
}

}
}
