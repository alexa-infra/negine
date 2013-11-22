#include "renderstate.h"

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

}
}