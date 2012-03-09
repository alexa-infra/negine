/**
 * @author  Alexey Vasilyev <alexa.infra@gmail.com>
 * @section LICENSE
 * 
 * This file is part of Negine (https://hg.alexadotlife.com/negine)
 * Licensed under LGPLv3 (or GPLv3) - see LICENSE file for details
 **/
#include "renderer/vertexbuffer.h"

namespace base {
namespace opengl {

VertexBuffer::VertexBuffer()
: vertex_count_(0)
, faces_count_(0)
{
    vertexes_ = new GLBufferObject;
    indexes_ = new GLBufferObject;
}

VertexBuffer::VertexBuffer(Vertex* vertexes, u16 vcount, Face* faces, u16 fcount, BufferUsage usage/* = BufferUsages::DynamicDraw*/)
: vertex_count_(vcount)
, faces_count_(fcount)
{
    vertexes_ = new GLBufferObject;
    indexes_ = new GLBufferObject;

    SetData(vertexes, vcount, faces, fcount, usage);
}

VertexBuffer::~VertexBuffer() {
    delete vertexes_;
    delete indexes_;
}

void VertexBuffer::SetData(Vertex* vertexes, u16 vcount, Face* faces, u16 fcount, BufferUsage usage/* = BufferUsages::DynamicDraw*/) {
    vertex_count_ = vcount;
    faces_count_ = fcount;

    vertexes_->Bind(BufferTargets::Array);
    vertexes_->SetData(vertex_array_size(), (void*)vertexes, usage);

    u16* indexes = new u16[index_count()];
    for(u16 i=0; i<faces_count(); i++) {
        indexes[3*i+0] = faces[i].index[0];
        indexes[3*i+1] = faces[i].index[1];
        indexes[3*i+2] = faces[i].index[2];
    }

    indexes_->Bind(BufferTargets::ElementArray);
    indexes_->SetData(index_array_size(), (void*)indexes, BufferUsages::StreamDraw);
    delete[] indexes;
}

Vertex* VertexBuffer::Lock(BufferAccess access/* = BufferAccesses::ReadWrite*/) {
    vertexes_->Bind(BufferTargets::Array);
    return (Vertex*)vertexes_->Map(access);
}

void VertexBuffer::Unlock() {
    vertexes_->Bind(BufferTargets::Array);
    vertexes_->Unmap();
}

void VertexBuffer::Draw(const AttributeBinding& binding) {
    Draw(binding, /*from_face*/0, /*count_faces*/faces_count());
}

void VertexBuffer::Draw(const AttributeBinding& binding, u32 from_face, u32 count_faces) {
    BindAttributes(binding);
    DrawOnly(from_face, count_faces);
    UnbindAttributes(binding);
}

void VertexBuffer::DrawOnly(u32 from_face, u32 count_faces) {
    glDrawElements(GL_TRIANGLES,
        face_to_index(count_faces),
        GL_UNSIGNED_SHORT,
        (GLvoid*)face_to_index(from_face)); 
}

void VertexBuffer::BindAttributes(const AttributeBinding& binding) {
    vertexes_->Bind(BufferTargets::Array);
    AttributeBinding::const_iterator it;
    for(it = binding.begin(); it != binding.end(); ++it) {
        glVertexAttribPointer(
                    it->second,
                    VertexAttrs::GetComponentCount(it->first),
                    GL_FLOAT,
                    GL_FALSE,
                    sizeof(Vertex),
                    (GLvoid*)VertexAttrs::GetOffset(it->first));
    }
    indexes_->Bind(BufferTargets::ElementArray);
}

void VertexBuffer::UnbindAttributes(const AttributeBinding& binding) {
    indexes_->Unbind();
    vertexes_->Unbind();
}

}
}
