#include "render/renderstate.h"
#include "render/mesh.h"

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

void RenderState::render(const Mesh& mesh)
{
    if (true /*do not use buffers */) {
        const MeshLayer* attributes = mesh.attributes();
        for (u32 i=0; i<VertexAttrs::Count; i++) {
            const MeshLayer& attr = attributes[i];
            if (!attr.valid_)
                continue;
            VertexAttr vertexAttr = static_cast<VertexAttr>(i);
            u32 location = VertexAttrs::GetAttributeLocation( vertexAttr );
            gl.EnableVertexAttribArray( location );
            gl.VertexAttribPointer(
                location,
                VertexAttrs::GetComponentCount( vertexAttr ),
                VertexAttrs::GetGLType( vertexAttr ),
                GL_FALSE,
                attr.stride_,
                const_cast<Mesh&>(mesh).findAttributeTyped<void*>(vertexAttr) );
        }
        if (mesh.numIndexes() == 0) {
            gl.DrawArrays(GL_TRIANGLES, 0, mesh.numIndexes());
        } else {
            gl.DrawElements(GL_TRIANGLES, mesh.numIndexes(), mesh.indexType(), const_cast<Mesh&>(mesh).indices());
        }
        for (u32 i=0; i<VertexAttrs::Count; i++) {
            const MeshLayer& attr = attributes[i];
            if (!attr.valid_)
                continue;
            VertexAttr vertexAttr = static_cast<VertexAttr>(i);
            u32 location = VertexAttrs::GetAttributeLocation( vertexAttr );
            gl.DisableVertexAttribArray(location);
        }
    }
}

}
}