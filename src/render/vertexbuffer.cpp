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

VertexBuffer::VertexBuffer()
    : vertexes_(nullptr)
    , indexes_(nullptr)
    , vao_(0)
{
}

VertexBuffer::~VertexBuffer()
{
    glDeleteVertexArrays(1, &vao_);
    delete vertexes_;
    delete indexes_;
}

void VertexBuffer::EnableAttribute( VertexAttr attr, u32 stride, void* pointer )
{
    enabledAttributes_[(u32)attr] = EnabledAttribute(stride, pointer);
}

void VertexBuffer::EnableAttributeMesh( const MeshExt* mesh )
{
    const std::vector<MeshLayer*>& attributes = mesh->attributes();
    for (u32 i=0; i<attributes.size(); i++)
    {
        EnableAttribute(attributes[i]->attr_, attributes[i]->stride_, (u8*)0 + attributes[i]->start_);
    }
}

void VertexBuffer::SetVertexData( void* vertexes, u32 vertexCount )
{
    if (vertexes_ == nullptr)
        vertexes_ = new BufferObject;
    vertexes_->Bind( BufferTargets::Array );
    vertexes_->SetData( vertexCount, vertexes, BufferUsages::DynamicDraw );
    vertexes_->Unbind();
}

void VertexBuffer::SetIndexData( void* index, u32 indexCount )
{
    if (indexes_ == nullptr)
        indexes_ = new BufferObject;
    indexes_->Bind( BufferTargets::ElementArray );
    indexes_->SetData( indexCount, index, BufferUsages::DynamicDraw );
    indexes_->Unbind();
}

void VertexBuffer::BindAttributes( )
{
    ASSERT(vao_ != 0);
    glBindVertexArray(vao_);
}

void VertexBuffer::Load()
{
    ASSERT(vertexes_ != nullptr && indexes_ != nullptr);
    ASSERT(enabledAttributes_[VertexAttrs::tagPosition].enabled_
        || enabledAttributes_[VertexAttrs::tagNormal].enabled_
        || enabledAttributes_[VertexAttrs::tagTexture].enabled_);

    if (vao_ == 0)
        glGenVertexArrays(1, &vao_);
    glBindVertexArray(vao_);

    vertexes_->Bind( BufferTargets::Array );

    for (u32 i=0; i<VertexAttrs::Count; i++)
    {
        EnabledAttribute& attr = enabledAttributes_[i];
        VertexAttr vertexAttr = (VertexAttr)i;
        u32 location = VertexAttrs::GetAttributeLocation( vertexAttr );
        if (enabledAttributes_[i].enabled_)
        {
            glEnableVertexAttribArray( location );
            glVertexAttribPointer(
                location,
                VertexAttrs::GetComponentCount( vertexAttr ),
                VertexAttrs::GetGLType( vertexAttr ),
                GL_FALSE,
                attr.stride_,
                attr.pointer_ );
        }
        else
        {
            glDisableVertexAttribArray(location);
        }
    }

    indexes_->Bind( BufferTargets::ElementArray );
    glBindVertexArray(0);
}

void VertexBuffer::UnbindAttributes( )
{
    glBindVertexArray(0);
}

}
}
