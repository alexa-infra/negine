/**
 * \file
 * \brief       Wrapper for buffer object class
 * \author      Alexey Vasilyev <alexa.infra@gmail.com>
 * \copyright   MIT License
 **/
#pragma once

#include "base/types.h"
#include "render/gpuresource.h"

namespace base
{
namespace opengl
{

//! the target to which the buffer object is bound
enum class BufferTarget {
    Array = GL_ARRAY_BUFFER,
    ElementArray = GL_ELEMENT_ARRAY_BUFFER,
    Uniform = GL_UNIFORM_BUFFER,
    Texture = GL_TEXTURE_BUFFER
};

//! expected usage of buffer. Stream - once, Static - not often, Dynamic - often
enum class BufferUsage {
    StreamDraw = GL_STREAM_DRAW,
    StreamRead = GL_STREAM_READ,
    StreamCopy = GL_STREAM_COPY,
    StaticDraw = GL_STATIC_DRAW,
    StaticRead = GL_STATIC_READ,
    StaticCopy = GL_STATIC_COPY,
    DynamicDraw = GL_DYNAMIC_DRAW,
    DynamicRead = GL_DYNAMIC_READ,
    DynamicCopy = GL_DYNAMIC_COPY
};

//! Wraps Buffer Object
class BufferObject : public GpuResource
{
private:
    GLenum target_; //!< Currently mapped type format
    GLenum usage_;
public:
    BufferObject(DeviceContext& gl, BufferTarget target, BufferUsage usage);
    ~BufferObject();

    //! Bind buffer to target
    void bind();
    //! Unbind active target
    void unbind();
    //! Bind buffer to target by index
    void bindBase( u32 index );
    //! Bind buffer to target by index, and to desired position
    void bindRange( u32 index, void* offset, void* size );

    //! Sets data of buffer, copy from memory to GPU
    void setData( u32 size, const void* dataPtr );
    //! Sets sub-data of buffer, copy from memory to GPU
    void setSubData( u32 offset, u32 size, const void* dataPtr );
    //! Gets sub-data of buffer, copy from GPU to memory
    void getSubData( u32 offset, u32 size, void* dataPtr );

private:
    DISALLOW_COPY_AND_ASSIGN( BufferObject );
};

}
}
