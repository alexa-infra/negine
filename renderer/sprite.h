/**
 * @author  Alexey Vasilyev <alexa.infra@gmail.com>
 * @section LICENSE
 * 
 * This file is part of Negine (https://hg.alexadotlife.com/negine)
 * Licensed under LGPLv3 (or GPLv3) - see LICENSE file for details
 **/
#pragma once

#error depricated header

#include "renderer/mesh.h"
#include "renderer/glbuffer.h"
#include "base/math/rect.h"
#include <assert.h>

namespace base {
namespace opengl {

using namespace base::math;

class Sprite {
public:
    RectF rectange;
    Vector4 color;

    Sprite() : color(0.f, 0.f, 1.f, 1.f) {}

    u32 init_vertex(Vertex* vertex, u32 index) const {
        assert(index % 4 == 0);

        vertex[index + 0].tex.set(0.f, 0.f);
        vertex[index + 1].tex.set(1.f, 0.f);
        vertex[index + 2].tex.set(1.f, 1.f);
        vertex[index + 3].tex.set(0.f, 1.f);

        vertex[index + 0].color = color;
        vertex[index + 1].color = color;
        vertex[index + 2].color = color;
        vertex[index + 3].color = color;

        return index + 4;
    }
    u32 init_faces(Face* faces, u32 index) const {
        assert(index % 2 == 0);
        faces[index + 0].index[0] = index * 2 + 0;
        faces[index + 0].index[1] = index * 2 + 1;
        faces[index + 0].index[2] = index * 2 + 2;
        faces[index + 1].index[0] = index * 2 + 2;
        faces[index + 1].index[1] = index * 2 + 3;
        faces[index + 1].index[2] = index * 2 + 0;
        return index + 2;
    }
    u32 fill_position(Vertex* vertex, u32 index) const {
        assert(index % 4 == 0);

        Vector2 v[4];
        rectange.Points(v);

        vertex[index + 0].pos = Vector3(v[0]);
        vertex[index + 1].pos = Vector3(v[1]);
        vertex[index + 2].pos = Vector3(v[2]);
        vertex[index + 3].pos = Vector3(v[3]);

        return index + 4;
    }
};

} // namespace base
} // namespace opengl