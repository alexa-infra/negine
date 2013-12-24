#include "render/renderstate.h"
#include "render/mesh.h"
#include "base/debug.h"

namespace base {
namespace opengl {

RenderState::RenderState(DeviceContext& context)
    : gl(context)
    , cullface(gl)
    , depthTest(gl)
    , blend(gl)
    , depthWrite(gl)
    , viewportArea(gl)
    , program(gl)
    , indexBuffer(gl)
    , vertexBuffer(gl)
    , activeTexture(gl)
{
}

void RenderState::render(const Mesh& mesh, u32 from, u32 count)
{
    if (true /*do not use buffers */) {
        const std::vector<MeshAttribute>& attributes = mesh.attributes();
        size_t size = attributes.size();
        for (size_t i=0; i<size; i++) {
            const MeshAttribute& attr = attributes[i];
            ASSERT(attr.valid_);
            u32 location = program.current().getAttributeLoc(attr.attr_, attr.idx_);
            gl.EnableVertexAttribArray( location );
            gl.VertexAttribPointer(
                location,
                VertexAttrs::GetComponentCount( attr.attr_ ),
                VertexAttrs::GetGLType( attr.attr_ ),
                GL_FALSE,
                attr.stride_,
                const_cast<Mesh&>(mesh).findAttribute<void*>(attr.attr_, attr.idx_) );
        }
        //if (mesh.numIndexes() == 0) {
        //    gl.DrawArrays(GL_TRIANGLES, from, count);
        //} else {
            gl.DrawElements(GL_TRIANGLES, count, mesh.indexType(), (u32*)const_cast<Mesh&>(mesh).indices() + from);
        //}
        for (u32 i=0; i<size; i++) {
            const MeshAttribute& attr = attributes[i];
            ASSERT(attr.valid_);
            u32 location = program.current().getAttributeLoc(attr.attr_, attr.idx_);
            gl.DisableVertexAttribArray(location);
        }
    }
}

}
}