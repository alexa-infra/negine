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
    struct Edge
    {
        u32 v[2];
        u32 p[2];
    };

    struct Vertex
    {
        math::vec3f position;
        math::vec2f uv;
        math::vec3f normal;

        std::vector<u32> edges;
        f32 weights[4];

        math::vec4f color;
    };

    struct Polygon
    {
        u32 v[3];
        u32 e[3];
        math::vec2f uv[3];
        math::vec3f normal;

        math::vec4f color;
    };

    struct Line
    {
        u32 v[2];
        math::vec4f color;
    };

    struct MeshBuilder
    {
        enum {
            hasVertexNormal,
            hasVertexColor,
            hasVertexUV,
            hasPolygonColor
        };
        u32 mask;

        MeshBuilder();

        std::vector<Vertex> vertexList;
        std::vector<Polygon> polygonList;
        std::vector<Line> lineList;
        std::vector<Edge> edgeList;

        u32 nextVertexIndex() const
        {
            return vertexList.size();
        }

        Vertex& addVertex(const math::vec3f& p);

        Vertex& addVertex(const math::vec3f& p, const math::vec2f& uv);

        Vertex& addVertex(const math::vec3f& p, const math::vec3f& n);

        Vertex& addVertex(const math::vec3f& p, const math::vec2f& uv, const math::vec3f& n);

        Vertex& addVertex(const math::vec3f& p, const math::vec4f& color);

        void addPolygonQuad(u32 x, u32 y, u32 z, u32 w, const math::vec4f& color);

        Polygon& addPolygon(u32 x, u32 y, u32 z);

        Polygon& addPolygon(u32 x, u32 y, u32 z, const math::vec2f& uv1, const math::vec2f& uv2, const math::vec2f& uv3);

        Polygon& addPolygon(u32 x, u32 y, u32 z, const math::vec4f& color);

        Line& addLine(u32 x, u32 y);

        Line& addLine(u32 x, u32 y, const math::vec4f& color);

        u32 addEdge(u32 a, u32 b, u32 p);

        void getLineList(opengl::Mesh& mesh);

        void getDrawingList(opengl::Mesh& mesh);

        void createQuad();

        void createCube();

        void readOBJ(const std::string& filename);
    };
}

}
