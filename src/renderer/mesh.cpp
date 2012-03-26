/**
 * \file
 * \author      Alexey Vasilyev <alexa.infra@gmail.com>
 * \copyright   MIT License
 **/
#include "renderer/mesh.h"
#include "renderer/vertexbuffer.h"

namespace base {
namespace opengl {

namespace VertexAttrs {

const Vertex test;
const uptr offsets[] = {
    ((uptr)&test.pos) - ((uptr)&test),
    ((uptr)&test.n) - ((uptr)&test),
    ((uptr)&test.tex) - ((uptr)&test),
    ((uptr)&test.tangent) - ((uptr)&test),
    ((uptr)&test.binormal) - ((uptr)&test),
    ((uptr)&test.color) - ((uptr)&test)
};

uptr GetOffset(VertexAttr attr) {
    return offsets[attr];
}
u8 GetComponentCount(VertexAttr attr) {
    switch (attr) {
    case tagPosition: return 3;
    case tagNormal: return 3;
    case tagTexture: return 2;
    case tagTangent: return 3;
    case tagBinormal: return 3;
    case tagColor: return 4;
    default:
        return 0;
    }
}

} // namespace VertexAttr

Mesh::Mesh() {
}

Mesh::~Mesh() {
    delete[] vertexes;
    delete[] faces;
}

}
}
