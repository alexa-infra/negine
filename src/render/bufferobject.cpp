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

BufferObject::BufferObject(DeviceContext& gl)
    : GpuResource(gl)
    , target_( 0 )
{
    GL.GenBuffers( 1, &id_ );
    ASSERT(id_ != 0);
}

BufferObject::~BufferObject()
{
    GL.DeleteBuffers( 1, &id_ );
}

void BufferObject::bind( BufferTarget target )
{
    GL.BindBuffer( target_ = target, id_ );
}

void BufferObject::unbind()
{
    GL.BindBuffer( target_, 0 );
}

void BufferObject::bindBase( BufferTarget target, u32 index )
{
    GL.BindBufferBase( target_ = target, index, id_ );
}

void BufferObject::bindRange( BufferTarget target, u32 index, void* offset, void* size )
{
    GL.BindBufferRange( target_ = target, index, id_, reinterpret_cast<GLintptr>(offset), reinterpret_cast<GLsizeiptr>(size) );
}

void BufferObject::setData( u32 size, const void* dataPtr, BufferUsage usage )
{
    GL.BufferData( target_, size, dataPtr, usage );
}

void BufferObject::setSubData( u32 offset, u32 size, const void* dataPtr )
{
    GL.BufferSubData( target_, offset, size, dataPtr );
}

void BufferObject::getSubData( u32 offset, u32 size, void* dataPtr )
{
    GL.GetBufferSubData( target_, offset, size, dataPtr );
}

void BufferObject::clear()
{
    GL.BufferData( target_, 0, nullptr, 0 );
}


}
}
