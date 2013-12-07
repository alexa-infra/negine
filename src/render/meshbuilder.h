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

    struct Surface
    {
        u32 pos;
        u32 normal;
        u32 uv;
        u32 color;
        u32 vertex;
        u32 polygon;
        u32 polygonCount;

        std::string name;
        std::string texture;

        Surface()
            : pos(0), normal(0), uv(0), color(0), vertex(0), polygon(0)
        {
        }
    };

    struct Polygon
    {
        u32 v[3];
    };

    struct Line
    {
        u32 v[2];
    };

    class NEGINE_API MeshBuilder
    {
    public:
        MeshBuilder();

        u32 addVertex(const math::vec3f& p);

        u32 addVertex(const math::vec3f& p, const math::vec2f& uv);

        u32 addVertex(const math::vec3f& p, const math::vec3f& n);

        u32 addVertex(const math::vec3f& p, const math::vec2f& uv, const math::vec3f& n);

        u32 addVertex(const math::vec3f& p, const math::vec4f& color);

        u32 addVertex(i32 p, i32 n = 0, i32 v = 0, i32 c = 0);

        void addPolygonQuad(const math::vec3f& a1, const math::vec3f& a2, const math::vec3f& a3, const math::vec3f& a4, const math::vec4f& color);

        u32 addPolygon(u32 x, u32 y, u32 z);

        u32 addLine(u32 x, u32 y);

        void beginSurface();

        void endSurface();

        u32 addLine(const math::vec3f& a, const math::vec3f& b, const math::vec4f& color);

        void getLineList(opengl::Mesh& mesh);

        void getDrawingList(opengl::Mesh& mesh);

        void createCube();

        void createGrid();

        void readOBJ(const std::string& filename);
    public:
        std::vector<math::vec3f> posData;
        std::vector<math::vec2f> uvData;
        std::vector<math::vec3f> normalData;
        std::vector<math::vec4f> colorData;
        
        std::vector<Surface> surfaces;
        Surface currentSurface;
    private:
        std::vector<Vertex> vertexList;
        std::vector<Polygon> polygonList;
        std::vector<Line> lineList;

        

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
