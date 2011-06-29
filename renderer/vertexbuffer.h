#include "renderer/mesh.h"
#include "renderer/glbuffer.h"

namespace base {
namespace opengl {

class VertexBuffer {
protected:
    u32 vertex_count_;
    u32 index_count_;
    GLBufferObject* vertexes_;
    GLBufferObject* indexes_;

public:
    VertexBuffer(Vertex* vertexes, u8 vcount, Face* faces, u8 fcount, BufferUsage usage = BufferUsages::DynamicDraw); 

    ~VertexBuffer();

    Vertex* Lock(BufferAccess access = BufferAccesses::ReadWrite);
    void Unlock();

    void Draw(AttributeBinding& binding);

protected:
    void BindAttributes(AttributeBinding& binding);
    void UnbindAttributes(AttributeBinding& binding);
};

}
}
