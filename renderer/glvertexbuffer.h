#pragma once

#include "base/types.h"
#include "renderer/glcontext.h"

namespace base {
namespace opengl {

namespace BufferTargets {
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
    enum BufferAccess {
        ReadOnly = GL_READ_ONLY,
        WriteOnly = GL_WRITE_ONLY,
        ReadWrite = GL_READ_WRITE
    };
}
typedef BufferAccesses::BufferAccess BufferAccess;

//! wraps Vertex Buffer Object 
class GLVertexBuffer {
private:
    GLuint id_;     //!< Buffer identifier
    GLenum target_; //!< Currently mapped type format
    bool is_ok_;    //!< Is buffer created
public:
    GLVertexBuffer();
    ~GLVertexBuffer();

    GLuint id() const { return id_; }
    bool is_ok() const { return is_ok_; }

    void Bind(BufferTarget target);
    void Unbind();
    void BindBase(BufferTarget target, u32 index);
    void BindRange(BufferTarget target, u32 index, void* offset, void* size);

    void* Map(BufferAccess access_type);
    bool Unmap();

    void SetData(u32 size, void* data_ptr, BufferUsage usage);
    void SetSubData(u32 offset, u32 size, const void* data_ptr);
    void GetSubData(u32 offset, u32 size, void* data_ptr);

    void Clear();
};

}
}
