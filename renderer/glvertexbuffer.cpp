#include "renderer/glvertexbuffer.h"

namespace ext {
namespace opengl {

GLVertexBuffer::GLVertexBuffer() 
    : id_(0)
    , target_(0)
    , is_ok_(false) {

    glGenBuffers(1, &id_);
    is_ok_ = (glIsBuffer(id_) == GL_TRUE);
}

GLVertexBuffer::~GLVertexBuffer() {
    if (is_ok_)
        glDeleteBuffers(1, &id_);
}

void GLVertexBuffer::Bind(GLenum format) {
    glBindBuffer(target_ = format, id_);
}

void GLVertexBuffer::Unbind() {
    glBindBuffer(target_, 0);
}

void* GLVertexBuffer::Map(GLenum access_type) {
    return glMapBuffer(target_, access_type);
}
bool GLVertexBuffer::Unmap() {
    return (glUnmapBuffer(target_) == GL_TRUE);
}

void GLVertexBuffer::SetData(u32 size, void* data_ptr, GLenum usage) {
    glBufferData(target_, size, data_ptr, usage);
}

void GLVertexBuffer::SetSubData(u32 offset, u32 size, const void* data_ptr) {
    glBufferSubData(target_, offset, size, data_ptr);
}

void GLVertexBuffer::GetSubData(u32 offset, u32 size, void* data_ptr) {
    glGetBufferSubData(target_, offset, size, data_ptr);
}

void GLVertexBuffer::Clear() {
    glBufferData(target_, 0, NULL, 0);
}


}
}
