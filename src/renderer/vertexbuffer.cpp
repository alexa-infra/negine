/**
 * \file
 * \brief       vertex buffer obejct wrapper
 * \author      Alexey Vasilyev <alexa.infra@gmail.com>
 * \copyright   MIT License
 **/
#include "renderer/vertexbuffer.h"
#include <string.h>

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

    indexes_->Bind(BufferTargets::ElementArray);
    indexes_->SetData(index_array_size(), (void*)faces, BufferUsages::StreamDraw);
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
        count_faces * 3,
        GL_UNSIGNED_SHORT,
        (Face*)0 + from_face); 
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
                    (u8*)0 + VertexAttrs::GetOffset(it->first));
    }
    indexes_->Bind(BufferTargets::ElementArray);
}

void VertexBuffer::UnbindAttributes(const AttributeBinding& binding) {
    indexes_->Unbind();
    vertexes_->Unbind();
}

///////////////////////

VertexBufferMemory::VertexBufferMemory()
: vertex_count_(0)
, faces_count_(0)
, vertexes_(NULL)
, indexes_(NULL)
{
}

VertexBufferMemory::VertexBufferMemory(Vertex* vertexes, u16 vcount, Face* faces, u16 fcount, BufferUsage usage/* = BufferUsages::DynamicDraw*/)
: vertex_count_(vcount)
, faces_count_(fcount)
, vertexes_(NULL)
, indexes_(NULL)
{
    vertexes_ = new Vertex[vcount];
    indexes_ = new Face[fcount];

    SetData(vertexes, vcount, faces, fcount, usage);
}

VertexBufferMemory::~VertexBufferMemory() {
    delete[] vertexes_;
    delete[] indexes_;
}

void VertexBufferMemory::SetData(Vertex* vertexes, u16 vcount, Face* faces, u16 fcount, BufferUsage usage/* = BufferUsages::DynamicDraw*/) {
    if (vertex_count_ != vcount)
    {
        delete[] vertexes_;
        vertexes_ = new Vertex[vcount];
        vertex_count_ = vcount;
    }
    if (faces_count_ != fcount)
    {
        delete[] indexes_;
        faces_count_ = fcount;
        indexes_ = new Face[fcount];
    }

    memcpy(vertexes_, vertexes, vertex_count_ * sizeof(Vertex)); //vertex_array_size());
    memcpy(indexes_, faces, faces_count_ * sizeof(Face));// index_array_size());
}

void VertexBufferMemory::Draw(const AttributeBinding& binding) {
    Draw(binding, /*from_face*/0, /*count_faces*/faces_count());
}

void VertexBufferMemory::Draw(const AttributeBinding& binding, u32 from_face, u32 count_faces) {
    BindAttributes(binding);
    DrawOnly(from_face, count_faces);
    UnbindAttributes(binding);
}

void VertexBufferMemory::DrawOnly(u32 from_face, u32 count_faces) {
    glDrawElements(GL_TRIANGLES,
        count_faces * 3, // face_to_index(count_faces),
        GL_UNSIGNED_SHORT,
        //(u16*)indexes_ + face_to_index(from_face)); 
        indexes_ + from_face);
}

void VertexBufferMemory::BindAttributes(const AttributeBinding& binding) {
    AttributeBinding::const_iterator it;
    for(it = binding.begin(); it != binding.end(); ++it) {
        glVertexAttribPointer(
                    it->second,
                    VertexAttrs::GetComponentCount(it->first),
                    GL_FLOAT,
                    GL_FALSE,
                    sizeof(Vertex),
                    ((u8*)vertexes_) + VertexAttrs::GetOffset(it->first));
    }
}

void VertexBufferMemory::UnbindAttributes(const AttributeBinding& binding) {
}

}
}
