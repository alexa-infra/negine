/**
 * @author  Alexey Vasilyev <alexa.infra@gmail.com>
 * @section LICENSE
 * 
 * This file is part of Negine (https://alexadotlife.com/hg/negine)
 * Licensed under LGPLv3 (or GLPv3) – see LICENSE file for details
 **/
#include "renderer/mesh.h"
#include "renderer/vertexbuffer.h"

namespace base {
namespace opengl {

namespace VertexAttrs {

const Vertex test;
const u8 offsets[] = {
    ((uptr)&test.pos) - ((uptr)&test),
    ((uptr)&test.n) - ((uptr)&test),
    ((uptr)&test.tex) - ((uptr)&test),
    ((uptr)&test.color) - ((uptr)&test)
};

u8 GetOffset(VertexAttr attr) {
    return offsets[attr];
}
u8 GetComponentCount(VertexAttr attr) {
    switch (attr) {
    case tagPosition: return 3;
    case tagNormal: return 3;
    case tagTexture: return 2;
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
