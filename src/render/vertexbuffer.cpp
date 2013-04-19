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
    , vao_(0)
    , GL(gl)
{
}

VertexBuffer::~VertexBuffer()
{
    GL.DeleteVertexArrays(1, &vao_);
    delete vertexes_;
    delete indexes_;
}

void VertexBuffer::EnableAttribute( VertexAttr attr, u32 stride, void* pointer )
{
    enabledAttributes_[(u32)attr] = EnabledAttribute(stride, pointer);
}

void VertexBuffer::EnableAttributeMesh( const Mesh* mesh )
{
    const MeshLayer* attributes = mesh->attributes();
    for (u32 i=0; i<VertexAttrs::Count; i++)
    {
        if (!attributes[i].valid_)
            continue;
        EnableAttribute(attributes[i].attr_, attributes[i].stride_, (u8*)0 + attributes[i].start_);
    }
}

void VertexBuffer::SetVertexData( void* vertexes, u32 vertexCount )
{
    if (vertexes_ == nullptr)
        vertexes_ = new BufferObject(GL);
    vertexes_->Bind( BufferTargets::Array );
    vertexes_->SetData( vertexCount, vertexes, BufferUsages::DynamicDraw );
    vertexes_->Unbind();
}

void VertexBuffer::SetIndexData( void* index, u32 indexCount )
{
    if (indexes_ == nullptr)
        indexes_ = new BufferObject(GL);
    indexes_->Bind( BufferTargets::ElementArray );
    indexes_->SetData( indexCount, index, BufferUsages::DynamicDraw );
    indexes_->Unbind();
}

void VertexBuffer::BindAttributes( )
{
    ASSERT(vao_ != 0);
    GL.BindVertexArray(vao_);
}

void VertexBuffer::Load()
{
    ASSERT(vertexes_ != nullptr && indexes_ != nullptr);
    ASSERT(enabledAttributes_[VertexAttrs::tagPosition].enabled_
        || enabledAttributes_[VertexAttrs::tagNormal].enabled_
        || enabledAttributes_[VertexAttrs::tagTexture].enabled_);

    if (vao_ == 0)
        GL.GenVertexArrays(1, &vao_);
    GL.BindVertexArray(vao_);

    vertexes_->Bind( BufferTargets::Array );

    for (u32 i=0; i<VertexAttrs::Count; i++)
    {
        EnabledAttribute& attr = enabledAttributes_[i];
        VertexAttr vertexAttr = (VertexAttr)i;
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

    indexes_->Bind( BufferTargets::ElementArray );
    GL.BindVertexArray(0);
}

void VertexBuffer::UnbindAttributes( )
{
    GL.BindVertexArray(0);
}

}
}
