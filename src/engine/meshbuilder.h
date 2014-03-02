#pragma once

#include "base/types.h"
#include "foundation/array.h"
#include "math/vec3.h"
#include "math/vec4.h"

namespace base {

namespace opengl {
    class Mesh;
}

namespace imp {
    struct Vertex
    {
        u32 pos;
        u32 normal;
        u32 uv;
        u32 color;

        Vertex() = default;

        Vertex(u32 p, u32 n = -1, u32 v = -1, u32 c = -1)
            : pos(p), normal(n), uv(v), color(c)
        {}
    };

    struct Surface
    {
        u32 pos;
        u32 normal;
        u32 uv;
        u32 color;
        u32 vertex;
        u32 polygon;
        u32 polygonCount;

        Surface() = default;
    };

    struct Polygon
    {
        u32 v[3];
    };

    struct Line
    {
        u32 v[2];
    };

	using namespace foundation;

    class MeshBuilder
    {
    public:
        NEGINE_API MeshBuilder();

        NEGINE_API u32 addVertex(const math::vec3f& p);

        NEGINE_API u32 addVertex(const math::vec3f& p, const math::vec2f& uv);

        NEGINE_API u32 addVertex(const math::vec3f& p, const math::vec3f& n);

        NEGINE_API u32 addVertex(const math::vec3f& p, const math::vec2f& uv, const math::vec3f& n);

        NEGINE_API u32 addVertex(const math::vec3f& p, const math::vec4f& color);

        u32 addVertex(i32 p, i32 n = 0, i32 v = 0, i32 c = 0);

        void addPolygonQuad(const math::vec3f& a1, const math::vec3f& a2, const math::vec3f& a3, const math::vec3f& a4, const math::vec4f& color);

        NEGINE_API u32 addPolygon(u32 x, u32 y, u32 z);

        u32 addLine(u32 x, u32 y);

        NEGINE_API void beginSurface();

        NEGINE_API void endSurface();

        u32 addLine(const math::vec3f& a, const math::vec3f& b, const math::vec4f& color);

        void getLineList(opengl::Mesh& mesh);

        NEGINE_API void getDrawingList(opengl::Mesh& mesh);

        void createCube();

        void createGrid();
    public:
        Array<math::vec3f> posData;
        Array<math::vec2f> uvData;
        Array<math::vec3f> normalData;
        Array<math::vec4f> colorData;
        
        Array<Surface> surfaces;
        Surface currentSurface;
    private:
        Array<Vertex> vertexList;
        Array<Polygon> polygonList;
        Array<Line> lineList;

        enum
        {
            hasVertexNormal = 1 << 0,
            hasVertexColor = 1 << 1,
            hasVertexUV = 1 << 2
        };
        u32 mask;
    };
}

}
