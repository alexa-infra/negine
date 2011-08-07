/**
 * @author  Alexey Vasilyev <alexa.infra@gmail.com>
 * @section LICENSE
 * 
 * This file is part of Negine (https://alexadotlife.com/hg/negine)
 * Licensed under LGPLv3 (or GPLv3) - see LICENSE file for details
 **/
#pragma once

#include "base/types.h"
#include "renderer/glcontext.h"

namespace base {
namespace opengl {

namespace BufferTargets {
    //! the target to which the buffer object is bound
    enum BufferTarget {
        Array = GL_ARRAY_BUFFER,
        CopyRead = GL_COPY_READ_BUFFER,
        CopyWrite = GL_COPY_WRITE_BUFFER,
        Draw = GL_DRAW_INDIRECT_BUFFER,
        ElementArray = GL_ELEMENT_ARRAY_BUFFER,
        PixelPack = GL_PIXEL_PACK_BUFFER,
        PixelUnpack = GL_PIXEL_UNPACK_BUFFER,
        Texture = GL_TEXTURE_BUFFER,
        TransformFeedback = GL_TRANSFORM_FEEDBACK_BUFFER, 
        Uniform = GL_UNIFORM_BUFFER
    };
}
typedef BufferTargets::BufferTarget BufferTarget;

namespace BufferUsages {
    //! expected usage of buffer. Stream - once, Static - not often, Dynamic - often
    enum BufferUsage {
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
}
typedef BufferUsages::BufferUsage BufferUsage;

namespace BufferAccesses {
    //! access to buffer during mapping
    enum BufferAccess {
        ReadOnly = GL_READ_ONLY,
        WriteOnly = GL_WRITE_ONLY,
        ReadWrite = GL_READ_WRITE
    };
}
typedef BufferAccesses::BufferAccess BufferAccess;

//! Wraps Buffer Object 
class GLBufferObject {
private:
    GLuint id_;     //!< Buffer identifier
    GLenum target_; //!< Currently mapped type format
    bool is_ok_;    //!< Is buffer created
public:
    GLBufferObject();
    ~GLBufferObject();

    //! Gets name of buffer
    GLuint id() const { return id_; }
    //! Get status of buffer creation
    bool is_ok() const { return is_ok_; }

    //! Bind buffer to target
    void Bind(BufferTarget target);
    //! Unbind active target
    void Unbind();
    //! Bind buffer to target by index
    void BindBase(BufferTarget target, u32 index);
    //! Bind buffer to target by index, and to desired position
    void BindRange(BufferTarget target, u32 index, void* offset, void* size);

    //! Map buffer from GPU to memory
    void* Map(BufferAccess access_type);
    //! Unmap buffer, returns buffer to GPU
    bool Unmap();

    //! Sets data of buffer, copy from memory to GPU
    void SetData(u32 size, const void* data_ptr, BufferUsage usage);
    //! Sets sub-data of buffer, copy from memory to GPU
    void SetSubData(u32 offset, u32 size, const void* data_ptr);
    //! Gets sub-data of buffer, copy from GPU to memory
    void GetSubData(u32 offset, u32 size, void* data_ptr);

    //! Clear allocated memory at GPU
    void Clear();

private:
    DISALLOW_COPY_AND_ASSIGN(GLBufferObject);
};

}
}
