#pragma once

#include "base/types.h"
#include "renderer/glcontext.h"

namespace ext {
namespace opengl {

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

    void Bind(GLenum target_format);
    void Unbind();

    void* Map(GLenum access_type);
    bool Unmap();

    void SetData(u32 size, void* data_ptr, GLenum usage);
    void SetSubData(u32 offset, u32 size, const void* data_ptr);
    void GetSubData(u32 offset, u32 size, void* data_ptr);

    void Clear();
};

}
}
