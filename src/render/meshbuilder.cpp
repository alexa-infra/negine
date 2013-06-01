#include "render/meshbuilder.h"
#include "base/log.h"
#include "base/lexer.h"
#include "base/debug.h"
#include "mesh.h"

namespace base {

using namespace math;

namespace imp {


MeshBuilder::MeshBuilder()
    : mask(0)
{
    vertexList.reserve(1000);
    polygonList.reserve(1000);
    edgeList.reserve(1000);
}

Vertex& MeshBuilder::addVertex(const vec3f& p)
{
    u32 idx = vertexList.size();
    Vertex v;
    v.position = p;
    vertexList.push_back(v);
    return vertexList[idx];
}

Vertex& MeshBuilder::addVertex(const vec3f& p, const vec2f& uv)
{
    mask |= hasVertexUV;
    Vertex& v = addVertex(p);
    v.uv = uv;
    return v;
}

Vertex& MeshBuilder::addVertex(const vec3f& p, const vec3f& n)
{
    mask |= hasVertexNormal;
    Vertex& v = addVertex(p);
    v.normal = n;
    return v;
}

Vertex& MeshBuilder::addVertex(const vec3f& p, const vec2f& uv, const vec3f& n)
{
    mask |= hasVertexUV;
    mask |= hasVertexNormal;
    Vertex& v = addVertex(p);
    v.uv = uv;
    v.normal = n;
    return v;
}

Vertex& MeshBuilder::addVertex(const vec3f& p, const vec4f& color)
{
    mask |= hasVertexColor;
    Vertex& v = addVertex(p);
    v.color = color;
    return v;
}

void MeshBuilder::addPolygonQuad(u32 x, u32 y, u32 z, u32 w, const vec4f& color)
{
    mask |= hasPolygonColor;
    Polygon& a = addPolygon(x, y, z);
    a.color = color;
    Polygon& b = addPolygon(x, z, w);
    b.color = color;
}

Polygon& MeshBuilder::addPolygon(u32 x, u32 y, u32 z)
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
    vec3f xy = b.position - a.position;
    vec3f xz = c.position - a.position;
    p.normal = cross(xz, xy);
    u32 idx = polygonList.size();
    polygonList.push_back(p);
    p.e[0] = addEdge(x, y, idx);
    p.e[1] = addEdge(x, z, idx);
    p.e[2] = addEdge(y, z, idx);
    return polygonList[idx];
}

Polygon& MeshBuilder::addPolygon(u32 x, u32 y, u32 z, const vec2f& uv1, const vec2f& uv2, const vec2f& uv3)
{
    mask |= hasVertexUV;
    Polygon& p = addPolygon(x, y, z);
    p.uv[0] = uv1;
    p.uv[1] = uv2;
    p.uv[2] = uv3;
    return p;
}

Polygon& MeshBuilder::addPolygon(u32 x, u32 y, u32 z, const vec4f& color)
{
    mask |= hasPolygonColor;
    Polygon& p = addPolygon(x, y, z);
    p.color = color;
    return p;
}

#define kInvalidIndex 0xffffffff

u32 MeshBuilder::addEdge(u32 a, u32 b, u32 p)
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
            edgeList[i].p[1] = p;
            return i;
        }
    }
    u32 idx = edgeList.size();
    edgeList.push_back(tmp);
    vertexList[a].edges.push_back(idx);
    vertexList[b].edges.push_back(idx);
    return idx;
}

void MeshBuilder::getDrawingList(opengl::Mesh& mesh)
{
    u32 vertexCount = polygonList.size() * 3;
    mesh.addAttribute(opengl::VertexAttrs::tagPosition);
    mesh.addAttribute(opengl::VertexAttrs::tagNormal);
    bool hasUV = (mask & hasVertexUV) == hasVertexUV;
    if (hasUV)
        mesh.addAttribute(opengl::VertexAttrs::tagTexture);
    bool hasColor = (mask & hasPolygonColor) == hasPolygonColor || (mask & hasVertexColor) == hasVertexColor;
    if (hasColor)
        mesh.addAttribute(opengl::VertexAttrs::tagColor);
    mesh.vertexCount(vertexCount);
    mesh.indexCount(vertexCount, opengl::IndexTypes::UInt32);
    mesh.complete();

    vec3f* position = mesh.findAttributeTyped<vec3f>(opengl::VertexAttrs::tagPosition);
    vec3f* normal = mesh.findAttributeTyped<vec3f>(opengl::VertexAttrs::tagNormal);
    vec4f* color = nullptr;
    if (hasColor)
        color = mesh.findAttributeTyped<vec4f>(opengl::VertexAttrs::tagColor);
    vec2f* uv = nullptr;
    if (hasUV)
        uv = mesh.findAttributeTyped<vec2f>(opengl::VertexAttrs::tagTexture);
    u32* indeces = reinterpret_cast<u32*>(mesh.indices());

    for(u32 i=0, j=0; i<polygonList.size(); i++, j+=3)
    {
        const Polygon& polygon = polygonList[i];
        for(u32 k=0, idx=j; k<3; k++, idx++)
        {
            const Vertex& v = vertexList[polygon.v[k]];
            position[idx] = v.position;
            if ((mask & hasVertexNormal) == hasVertexNormal)
                normal[idx] = v.normal;
            else
                normal[idx] = polygon.normal;

            if (hasColor)
            {
                if ((mask & hasPolygonColor) == hasPolygonColor)
                    color[idx] = polygon.color;
                else if ((mask & hasVertexColor) == hasVertexColor)
                    color[idx] = v.color;
            }

            if (hasUV)
                uv[idx] = v.uv;

            indeces[idx] = idx;
        }
    }
}

void MeshBuilder::createQuad()
{
    vec3f x(1, 0, 0);
    vec3f y(0, 1, 0);
    vec3f z(0, 0, 1);
    u32 idx = nextVertexIndex();
    addVertex(x * 0 + y * 0);
    addVertex(x * 1 + y * 0);
    addVertex(x * 0 + y * 1);
    vec4f red(1, 0, 0, 1);
    addPolygon(idx+2, idx+1, idx, red);
}

void MeshBuilder::createCube()
{
    vec3f x(1, 0, 0);
    vec3f y(0, 1, 0);
    vec3f z(0, 0, 1);
    vec3f ori(-0.5f, -0.5f, -0.5f);

    u32 idx = nextVertexIndex();
    addVertex(ori);
    addVertex(ori + x);
    addVertex(ori + x + z);
    addVertex(ori + z);
    addVertex(ori + y);
    addVertex(ori + y + x);
    addVertex(ori + y + x + z);
    addVertex(ori + y + z);

    vec4f red(1, 0, 0, 1);
    vec4f green(0, 1, 0, 1);
    vec4f blue(0, 0, 1, 1);
    vec4f black(0, 0, 0, 1);
    addPolygonQuad(idx + 0, idx + 1, idx + 2, idx + 3, red);
    addPolygonQuad(idx + 0, idx + 1, idx + 5, idx + 4, green);
    addPolygonQuad(idx + 2, idx + 3, idx + 7, idx + 6, green);
    addPolygonQuad(idx + 1, idx + 2, idx + 6, idx + 5, blue);
    addPolygonQuad(idx + 0, idx + 4, idx + 7, idx + 3, blue);
    addPolygonQuad(idx + 7, idx + 6, idx + 5, idx + 4, red);
}

void MeshBuilder::readOBJ(const std::string& filename)
{
    std::vector<vec3f> pos;
    std::vector<vec2f> uv;
    std::vector<vec3f> normal;

    pos.reserve(1000);
    normal.reserve(1000);

    LexerPolicy policy(LexerPolicy::pythonComment);
    policy.setWhitespaces(" \t\n\r/");

    Lexer lexer(filename, policy);

    while ( lexer.HasMoreData() )
    {
        std::string token = lexer.ReadToken();
        if (token == "v") {
            vec3f p;
            p.x = lexer.ReadFloat();
            p.y = lexer.ReadFloat();
            p.z = lexer.ReadFloat();
            pos.push_back(p);
        } else if (token == "vt") {
            vec2f v;
            v.x = lexer.ReadFloat();
            v.y = lexer.ReadFloat();
            uv.push_back(v);
        } else if (token == "vn") {
            vec3f n;
            n.x = lexer.ReadFloat();
            n.y = lexer.ReadFloat();
            n.z = lexer.ReadFloat();
            n = normalize(n);
            normal.push_back(n);
        } else if (token == "f") {
            u32 idx = nextVertexIndex();
            for(u32 i=0; i<3; i++) {
                u32 vertexIdx = static_cast<u32>(lexer.ReadFloat());
                if ( uv.size() != 0 && normal.size() != 0 ) {
                    u32 uvIdx = static_cast<u32>(lexer.ReadFloat());
                    u32 normalIdx = static_cast<u32>(lexer.ReadFloat());
                    addVertex(pos[vertexIdx-1], uv[uvIdx-1], normal[normalIdx-1]);
                } else if ( uv.size() != 0 ) {
                    u32 uvIdx = static_cast<u32>(lexer.ReadFloat());
                    addVertex(pos[vertexIdx-1], uv[uvIdx-1]);
                } else if ( normal.size() != 0 ) {
                    u32 normalIdx = static_cast<u32>(lexer.ReadFloat());
                    addVertex(pos[vertexIdx-1], normal[normalIdx-1]);
                } else {
                    addVertex(pos[vertexIdx-1]);
                }
            }
            addPolygon(idx, idx + 1, idx + 2);
        } else if (token == "mtllib") {
            lexer.ReadToken();
        } else if (token == "usemtl") {
            lexer.ReadToken();
        } else if (token == "g") {
            lexer.ReadToken();
        } else {
            ERR("unknown token in OBJ: '%s'", token.c_str());
        }
    }
}

} // namespace imp
} // namespace base
