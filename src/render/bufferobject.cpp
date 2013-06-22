/**
 * \file
 * \author      Alexey Vasilyev <alexa.infra@gmail.com>
 * \copyright   MIT License
 **/
#include "render/bufferobject.h"
#include "base/debug.h"

namespace base
{
namespace opengl
{

BufferObject::BufferObject(DeviceContext& gl, BufferTarget target, BufferUsage usage)
    : GpuResource(gl)
    , target_( target )
    , usage_( usage )
{
    GL.GenBuffers( 1, &id_ );
    ASSERT(id_ != 0);
}

BufferObject::~BufferObject()
{
    GL.DeleteBuffers( 1, &id_ );
}

void BufferObject::bind()
{
    GL.BindBuffer( target_, id_ );
}

void BufferObject::unbind()
{
    GL.BindBuffer( target_, 0 );
}

void BufferObject::bindBase( u32 index )
{
    GL.BindBufferBase( target_, index, id_ );
}

void BufferObject::bindRange( u32 index, void* offset, void* size )
{
    GL.BindBufferRange( target_, index, id_, reinterpret_cast<GLintptr>(offset), reinterpret_cast<GLsizeiptr>(size) );
}

void BufferObject::setData( u32 size, const void* dataPtr )
{
    GL.BufferData( target_, size, dataPtr, usage_ );
}

void BufferObject::setSubData( u32 offset, u32 size, const void* dataPtr )
{
    GL.BufferSubData( target_, offset, size, dataPtr );
}

void BufferObject::getSubData( u32 offset, u32 size, void* dataPtr )
{
    GL.GetBufferSubData( target_, offset, size, dataPtr );
}

}
}
