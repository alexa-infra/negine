#pragma once

#include "math/vec4.h"
#include <vector>

namespace base
{

namespace opengl
{
    class Mesh;
}

namespace imp
{
    struct Vertex
    {
        u32 pos;
        u32 normal;
        u32 uv;
        u32 color;

        Vertex(u32 p, u32 n = -1, u32 v = -1, u32 c = -1)
            : pos(p), normal(n), uv(v), color(c)
        {}
    };

    struct Polygon
    {
        u32 v[3];
    };

    struct Line
    {
        u32 v[2];
    };

    class MeshBuilder
    {
    public:
        MeshBuilder();

        u32 addVertex(const math::vec3f& p);

        u32 addVertex(const math::vec3f& p, const math::vec2f& uv);

        u32 addVertex(const math::vec3f& p, const math::vec3f& n);

        u32 addVertex(const math::vec3f& p, const math::vec2f& uv, const math::vec3f& n);

        u32 addVertex(const math::vec3f& p, const math::vec4f& color);

        void addPolygonQuad(const math::vec3f& a1, const math::vec3f& a2, const math::vec3f& a3, const math::vec3f& a4, const math::vec4f& color);

        u32 addPolygon(u32 x, u32 y, u32 z);

        u32 addLine(u32 x, u32 y);

        u32 addLine(const math::vec3f& a, const math::vec3f& b, const math::vec4f& color);

        void getLineList(opengl::Mesh& mesh);

        void getDrawingList(opengl::Mesh& mesh);

        void createCube();

        void createGrid();

        void readOBJ(const std::string& filename);
    private:
        std::vector<math::vec3f> posData;
        std::vector<math::vec2f> uvData;
        std::vector<math::vec3f> normalData;
        std::vector<math::vec4f> colorData;

        std::vector<Vertex> vertexList;
        std::vector<Polygon> polygonList;
        std::vector<Line> lineList;

        enum
        {
            hasVertexNormal,
            hasVertexColor,
            hasVertexUV
        };
        u32 mask;
    };
}

}
