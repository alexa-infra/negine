/**
 * \file
 * \author      Alexey Vasilyev <alexa.infra@gmail.com>
 * \copyright   MIT License
 **/
#include "render/bufferobject.h"

namespace base
{
namespace opengl
{

BufferObject::BufferObject(DeviceContext& gl)
    : GpuResource(gl)
    , target_( 0 )
    , is_ok_( false )
{
    GL.GenBuffers( 1, &id_ );
    is_ok_ = id_ != 0;
}

BufferObject::~BufferObject()
{
    if ( is_ok_ ) {
        GL.DeleteBuffers( 1, &id_ );
    }
}

void BufferObject::Bind( BufferTarget target )
{
    GL.BindBuffer( target_ = target, id_ );
}

void BufferObject::Unbind()
{
    GL.BindBuffer( target_, 0 );
}

void BufferObject::BindBase( BufferTarget target, u32 index )
{
    GL.BindBufferBase( target_ = target, index, id_ );
}

void BufferObject::BindRange( BufferTarget target, u32 index, void* offset, void* size )
{
    GL.BindBufferRange( target_ = target, index, id_, ( GLintptr )offset, ( GLsizeiptr )size );
}

void* BufferObject::Map( BufferAccess access_type )
{
    return GL.MapBuffer( target_, access_type );
}
bool BufferObject::Unmap()
{
    return ( GL.UnmapBuffer( target_ ) == GL_TRUE );
}

void BufferObject::SetData( u32 size, const void* data_ptr, BufferUsage usage )
{
    GL.BufferData( target_, size, data_ptr, usage );
}

void BufferObject::SetSubData( u32 offset, u32 size, const void* data_ptr )
{
    GL.BufferSubData( target_, offset, size, data_ptr );
}

void BufferObject::GetSubData( u32 offset, u32 size, void* data_ptr )
{
    GL.GetBufferSubData( target_, offset, size, data_ptr );
}

void BufferObject::Clear()
{
    GL.BufferData( target_, 0, NULL, 0 );
}


}
}
