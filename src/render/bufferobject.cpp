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

BufferObject::BufferObject()
    : id_( 0 )
    , target_( 0 )
    , is_ok_( false )
{
    glGenBuffers( 1, &id_ );
    is_ok_ = id_ != 0;
}

BufferObject::~BufferObject()
{
    if ( is_ok_ ) {
        glDeleteBuffers( 1, &id_ );
    }
}

void BufferObject::Bind( BufferTarget target )
{
    glBindBuffer( target_ = target, id_ );
}

void BufferObject::Unbind()
{
    glBindBuffer( target_, 0 );
}

void BufferObject::BindBase( BufferTarget target, u32 index )
{
    glBindBufferBase( target_ = target, index, id_ );
}

void BufferObject::BindRange( BufferTarget target, u32 index, void* offset, void* size )
{
    glBindBufferRange( target_ = target, index, id_, ( GLintptr )offset, ( GLsizeiptr )size );
}

void* BufferObject::Map( BufferAccess access_type )
{
    return glMapBuffer( target_, access_type );
}
bool BufferObject::Unmap()
{
    return ( glUnmapBuffer( target_ ) == GL_TRUE );
}

void BufferObject::SetData( u32 size, const void* data_ptr, BufferUsage usage )
{
    glBufferData( target_, size, data_ptr, usage );
}

void BufferObject::SetSubData( u32 offset, u32 size, const void* data_ptr )
{
    glBufferSubData( target_, offset, size, data_ptr );
}

void BufferObject::GetSubData( u32 offset, u32 size, void* data_ptr )
{
    glGetBufferSubData( target_, offset, size, data_ptr );
}

void BufferObject::Clear()
{
    glBufferData( target_, 0, NULL, 0 );
}


}
}
