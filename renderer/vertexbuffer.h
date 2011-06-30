#pragma once

#include "renderer/mesh.h"
#include "renderer/glbuffer.h"

namespace base {
namespace opengl {

class VertexBuffer {
protected:
    u32 vertex_count_;
    u32 faces_count_;
    GLBufferObject* vertexes_;
    GLBufferObject* indexes_;

public:
    VertexBuffer();
    VertexBuffer(Vertex* vertexes, u8 vcount, Face* faces, u8 fcount, BufferUsage usage = BufferUsages::DynamicDraw); 

    void SetData(Vertex* vertexes, u8 vcount, Face* faces, u8 fcount, BufferUsage usage = BufferUsages::DynamicDraw);

    ~VertexBuffer();

    Vertex* Lock(BufferAccess access = BufferAccesses::ReadWrite);
    void Unlock();

    void Draw(AttributeBinding& binding);
    void Draw(AttributeBinding& binding, u32 from_face, u32 count_faces);

    u32 faces_count() const { return faces_count_; }
    u32 index_count() const { return faces_count_ * 3; }
    u32 index_array_size() const { return index_count() * sizeof(u8); }
    u32 vertex_count() const { return vertex_count_; }
    u32 vertex_array_size() const { return vertex_count_ * sizeof(Vertex); }
    u32 face_to_index(u32 face) const { return face * 3; }

protected:
    void BindAttributes(AttributeBinding& binding);
    void UnbindAttributes(AttributeBinding& binding);
};

}
}
