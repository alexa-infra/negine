/**
 * @author  Alexey Vasilyev <alexa.infra@gmail.com>
 * @section LICENSE
 * 
 * This file is part of Negine (https://hg.alexadotlife.com/negine)
 * Licensed under LGPLv3 (or GPLv3) - see LICENSE file for details
 **/
#include "renderer/glbuffer.h"

namespace base {
namespace opengl {

GLBufferObject::GLBufferObject() 
    : id_(0)
    , target_(0)
    , is_ok_(false) {
    glGenBuffers(1, &id_);
    is_ok_ = (glIsBuffer(id_) == GL_TRUE);
}

GLBufferObject::~GLBufferObject() {
    if (is_ok_)
        glDeleteBuffers(1, &id_);
}

void GLBufferObject::Bind(BufferTarget target) {
    glBindBuffer(target_ = target, id_);
}

void GLBufferObject::Unbind() {
    glBindBuffer(target_, 0);
}

void GLBufferObject::BindBase(BufferTarget target, u32 index) {
    glBindBufferBase(target_ = target, index, id_);
}

void GLBufferObject::BindRange(BufferTarget target, u32 index, void* offset, void* size) {
    glBindBufferRange(target_ = target, index, id_, (GLintptr)offset, (GLsizeiptr)size);
}

void* GLBufferObject::Map(BufferAccess access_type) {
    return glMapBuffer(target_, access_type);
}
bool GLBufferObject::Unmap() {
    return (glUnmapBuffer(target_) == GL_TRUE);
}

void GLBufferObject::SetData(u32 size, const void* data_ptr, BufferUsage usage) {
    glBufferData(target_, size, data_ptr, usage);
}

void GLBufferObject::SetSubData(u32 offset, u32 size, const void* data_ptr) {
    glBufferSubData(target_, offset, size, data_ptr);
}

void GLBufferObject::GetSubData(u32 offset, u32 size, void* data_ptr) {
    glGetBufferSubData(target_, offset, size, data_ptr);
}

void GLBufferObject::Clear() {
    glBufferData(target_, 0, NULL, 0);
}


}
}
