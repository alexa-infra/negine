#include "renderer/vertexbuffer.h"

namespace base {
namespace opengl {

VertexBuffer::VertexBuffer(Vertex* vertexes, u8 vcount, Face* faces, u8 fcount, BufferUsage usage/* = BufferUsages::DynamicDraw*/) {
    index_count_ = fcount * 3;
    vertexes_ = new GLBufferObject;
    vertexes_->Bind(BufferTargets::Array);
    vertexes_->SetData(sizeof(Vertex)*vcount, (void*)vertexes, usage);

    indexes_ = new GLBufferObject;
    indexes_->Bind(BufferTargets::ElementArray);
    
    u8* indexes = new u8[fcount*3];
    for(u8 i=0; i<fcount; i++) {
        indexes[3*i+0] = faces[i].index[0];
        indexes[3*i+1] = faces[i].index[1];
        indexes[3*i+2] = faces[i].index[2];
    }
    indexes_->SetData(sizeof(u8)*fcount*3, (void*)indexes, BufferUsages::StreamDraw);
    delete[] indexes;
}

VertexBuffer::~VertexBuffer() {
    delete vertexes_;
    delete indexes_;
}

Vertex* VertexBuffer::Lock(BufferAccess access/* = BufferAccesses::ReadWrite*/) {
    vertexes_->Bind(BufferTargets::Array);
    return (Vertex*)vertexes_->Map(access);
}

void VertexBuffer::Unlock() {
    vertexes_->Bind(BufferTargets::Array);
    vertexes_->Unmap();
}

void VertexBuffer::Draw(AttributeBinding& binding) {
    vertexes_->Bind(BufferTargets::Array);
    BindAttributes(binding);
    {
        indexes_->Bind(BufferTargets::ElementArray);
        glDrawElements(GL_TRIANGLES, index_count_, GL_UNSIGNED_BYTE, 0); 
        indexes_->Unbind();
    }
    UnbindAttributes(binding);
    vertexes_->Unbind();
}

void VertexBuffer::BindAttributes(AttributeBinding& binding) {
    AttributeBinding::iterator it;
    for(it = binding.begin(); it != binding.end(); ++it) {
        glVertexAttribPointer(
                    it->second,
                    VertexAttrs::GetComponentCount(it->first),
                    GL_FLOAT,
                    GL_FALSE,
                    sizeof(Vertex),
                    (GLvoid*)VertexAttrs::GetOffset(it->first));
        glEnableVertexAttribArray(it->second);
    }
}

void VertexBuffer::UnbindAttributes(AttributeBinding& binding) {
    AttributeBinding::iterator it;
    for(it = binding.begin(); it != binding.end(); ++it) {
        glDisableVertexAttribArray(it->second);
    }
}

}
}
