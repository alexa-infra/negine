#pragma once

#include "math/vec4.h"
#include "base/debug.h"
#include <vector>
#include "mesh.h"
#include "base/log.h"

namespace base
{

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

    struct Object
    {
        bool perVertexColor;
        std::vector<Vertex> vertexList;
        std::vector<Polygon> polygonList;
        std::vector<Edge> edgeList;

        Vertex& addVertex(const math::vec3f& p)
        {
            u32 idx = vertexList.size();
            Vertex v;
            v.position = p;
            vertexList.push_back(v);
            return vertexList[idx];
        }

        Vertex& addVertex(const math::vec3f& p, const math::vec2f& uv)
        {
            perVertexColor = false;
            Vertex& v = addVertex(p);
            v.uv = uv;
            return v;
        }

        Vertex& addVertex(const math::vec3f& p, const math::vec4f& color)
        {
            //perVertexColor = true;
            Vertex& v = addVertex(p);
            v.color = color;
            return v;
        }

        void addPolygonQuad(u32 x, u32 y, u32 z, u32 w, const math::vec4f& color)
        {
            addPolygon(x, y, z, color);
            addPolygon(x, z, w, color);
        }

        Polygon& addPolygon(u32 x, u32 y, u32 z)
        {
            Polygon p;
            p.v[0] = x;
            p.v[1] = y;
            p.v[2] = z;
            Vertex& a = vertexList[x];
            Vertex& b = vertexList[y];
            Vertex& c = vertexList[z];
            p.uv[0] = a.uv;
            p.uv[1] = b.uv;
            p.uv[2] = c.uv;
            math::vec3f xy = b.position - a.position;
            math::vec3f xz = c.position - a.position;
            p.normal = cross(xz, xy);
            u32 idx = polygonList.size();
            polygonList.push_back(p);
            p.e[0] = addEdge(x, y, idx);
            p.e[1] = addEdge(x, z, idx);
            p.e[2] = addEdge(y, z, idx);
            return polygonList[idx];
        }

        Polygon& addPolygon(u32 x, u32 y, u32 z, const math::vec2f& uv1, const math::vec2f& uv2, const math::vec2f& uv3)
        {
            Polygon& p = addPolygon(x, y, z);
            p.uv[0] = uv1;
            p.uv[1] = uv2;
            p.uv[2] = uv3;
        }

        Polygon& addPolygon(u32 x, u32 y, u32 z, const math::vec4f& color)
        {
            Polygon& p = addPolygon(x, y, z);
            p.color = color;
        }

#define kInvalidIndex 0xffffffff

        u32 addEdge(u32 a, u32 b, u32 p)
        {
            Edge tmp;
            tmp.v[0] = std::min(a, b);
            tmp.v[1] = std::max(a, b);
            tmp.p[0] = p;
            tmp.p[1] = kInvalidIndex;
            bool found = false;
            for(u32 i=0; i<edgeList.size(); i++)
            {
                Edge& edge = edgeList[i];
                if (edge.v[0] == tmp.v[0] && edge.v[1] == tmp.v[1])
                {
//                    ASSERT(edgeList[i].p[2] == kInvalidIndex);
                    edgeList[i].p[2] = p;
                    return i;
                }
            }
            u32 idx = edgeList.size();
            edgeList.push_back(tmp);
            vertexList[a].edges.push_back(idx);
            vertexList[b].edges.push_back(idx);
            return idx;
        }

        void getDrawingList(opengl::Mesh& mesh)
        {
            u32 vertexCount = polygonList.size() * 3;
            mesh
                .addAttribute(opengl::VertexAttrs::tagPosition)
                .addAttribute(opengl::VertexAttrs::tagNormal)
                .addAttribute(opengl::VertexAttrs::tagColor)
                .vertexCount(vertexCount, vertexCount)
                .complete();

            math::vec3f* position = mesh.findAttributeTyped<math::vec3f>(opengl::VertexAttrs::tagPosition);
            math::vec3f* normal = mesh.findAttributeTyped<math::vec3f>(opengl::VertexAttrs::tagNormal);
            math::vec4f* color = mesh.findAttributeTyped<math::vec4f>(opengl::VertexAttrs::tagColor);
            u16* indeces = mesh.indices();

            for(u32 i=0, j=0; i<polygonList.size(); i++, j+=3)
            {
                const Polygon& polygon = polygonList[i];
                for(u32 k=0, idx=j; k<3; k++, idx++)
                {
                    const Vertex& v = vertexList[polygon.v[k]];
                    position[idx] = v.position;
                    normal[idx] = polygon.normal;
                    WARN("%f %f %f", polygon.color.x, polygon.color.y, polygon.color.z);
                    if(!perVertexColor)
                        color[idx] = polygon.color;
                    else
                        color[idx] = v.color;
                    indeces[idx] = static_cast<u16>(idx);
                }
            }
        }

        void createQuad()
        {
            math::vec3f x(1, 0, 0);
            math::vec3f y(0, 1, 0);
            math::vec3f z(0, 0, 1);
            addVertex(x * 0 + y * 0);
            addVertex(x * 1 + y * 0);
            addVertex(x * 0 + y * 1);
            math::vec4f red(1, 0, 0, 1);
            addPolygon(2, 1, 0, red);
        }
        void createCube()
        {
            math::vec3f x(1, 0, 0);
            math::vec3f y(0, 1, 0);
            math::vec3f z(0, 0, 1);
            math::vec3f ori(-0.5f, -0.5f, -0.5f);

            addVertex(ori);
            addVertex(ori + x);
            addVertex(ori + x + z);
            addVertex(ori + z);
            addVertex(ori + y);
            addVertex(ori + y + x);
            addVertex(ori + y + x + z);
            addVertex(ori + y + z);

            math::vec4f red(1, 0, 0, 1);
            math::vec4f green(0, 1, 0, 1);
            math::vec4f blue(0, 0, 1, 1);
            math::vec4f black(0, 0, 0, 1);
            addPolygonQuad(0, 1, 2, 3, red);
            addPolygonQuad(0, 1, 5, 4, green);
            addPolygonQuad(2, 3, 7, 6, green);
            addPolygonQuad(1, 2, 6, 5, blue);
            addPolygonQuad(0, 4, 7, 3, blue);
            addPolygonQuad(7, 6, 5, 4, red);
        }
    };
}

}
