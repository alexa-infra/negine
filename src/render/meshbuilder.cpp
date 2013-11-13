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
}

u32 MeshBuilder::addVertex(const vec3f& p)
{
    u32 pIdx = posData.size();
    posData.push_back(p);
    Vertex v(pIdx);
    u32 idx = vertexList.size();
    vertexList.push_back(v);
    return idx;
}

u32 MeshBuilder::addVertex(const vec3f& p, const vec2f& uv)
{
    mask |= hasVertexUV;
    u32 idx = addVertex(p);
    Vertex& vertex = vertexList[idx];
    vertex.uv = uvData.size();
    uvData.push_back(uv);
    return idx;
}

u32 MeshBuilder::addVertex(const vec3f& p, const vec3f& n)
{
    mask |= hasVertexNormal;
    u32 idx = addVertex(p);
    Vertex& vertex = vertexList[idx];
    vertex.normal = normalData.size();
    normalData.push_back(n);
    return idx;
}

u32 MeshBuilder::addVertex(const vec3f& p, const vec2f& uv, const vec3f& n)
{
    u32 idx = addVertex(p, uv);
    mask |= hasVertexNormal;
    Vertex& vertex = vertexList[idx];
    vertex.normal = normalData.size();
    normalData.push_back(n);
    return idx;
}

u32 MeshBuilder::addVertex(const vec3f& p, const vec4f& color)
{
    mask |= hasVertexColor;
    u32 idx = addVertex(p);
    Vertex& vertex = vertexList[idx];
    vertex.color = colorData.size();
    colorData.push_back(color);
    return idx;
}

void MeshBuilder::addPolygonQuad(const math::vec3f& a1, const vec3f& a2, const vec3f& a3, const vec3f& a4, const vec4f& color)
{
    u32 idx1 = addVertex(a1, color);
    u32 idx2 = addVertex(a2, color);
    u32 idx3 = addVertex(a3, color);
    u32 idx4 = addVertex(a4, color);
    addPolygon(idx1, idx2, idx3);
    addPolygon(idx1, idx3, idx4);
}

u32 MeshBuilder::addPolygon(u32 x, u32 y, u32 z)
{
    Polygon p;
    p.v[0] = x;
    p.v[1] = y;
    p.v[2] = z;
    Vertex& a = vertexList[x];
    Vertex& b = vertexList[y];
    Vertex& c = vertexList[z];
    //vec3f xy = b.position - a.position;
    //vec3f xz = c.position - a.position;
    //p.normal = cross(xz, xy);
    u32 idx = polygonList.size();
    polygonList.push_back(p);
    return idx;
}

u32 MeshBuilder::addLine(u32 x, u32 y)
{
    Line line;
    line.v[0] = x;
    line.v[1] = y;
    u32 idx = lineList.size();
    lineList.push_back(line);
    return idx;
}

u32 MeshBuilder::addLine(const math::vec3f& a, const math::vec3f& b, const math::vec4f& color)
{
    u32 aIdx = addVertex(a, color);
    u32 bIdx = addVertex(b, color);
    return addLine(aIdx, bIdx);
}

void MeshBuilder::getLineList(opengl::Mesh& mesh)
{
    u32 vertexCount = lineList.size() * 2;
    mesh.addAttribute(opengl::VertexAttrs::tagPosition);
    mesh.addAttribute(opengl::VertexAttrs::tagColor);
    mesh.vertexCount(vertexCount);
    mesh.indexCount(vertexCount, opengl::IndexTypes::UInt32);
    mesh.complete();

    vec3f* position = mesh.findAttributeTyped<vec3f>(opengl::VertexAttrs::tagPosition);
    vec4f* color = mesh.findAttributeTyped<vec4f>(opengl::VertexAttrs::tagColor);
    u32* indeces = reinterpret_cast<u32*>(mesh.indices());
    bool vertexColor = (mask & hasVertexColor) == hasVertexColor;

    u32 idx = 0;
    for(u32 i=0; i<lineList.size(); i++) {
        const Line& line = lineList[i];
        for(u32 j=0; j<2; j++, idx++) {
            const Vertex& v = vertexList[line.v[j]];
            position[idx] = posData[v.pos];
            if (vertexColor)
                color[idx] = colorData[v.color];
            indeces[idx] = idx;
        }
    }
}

void MeshBuilder::getDrawingList(opengl::Mesh& mesh)
{
    u32 vertexCount = polygonList.size() * 3;
    mesh.addAttribute(opengl::VertexAttrs::tagPosition);
    bool hasNormal = (mask & hasVertexNormal) == hasVertexNormal;
    if (hasNormal)
        mesh.addAttribute(opengl::VertexAttrs::tagNormal);
    bool hasUV = (mask & hasVertexUV) == hasVertexUV;
    if (hasUV)
        mesh.addAttribute(opengl::VertexAttrs::tagTexture);
    bool hasColor = (mask & hasVertexColor) == hasVertexColor;
    if (hasColor)
        mesh.addAttribute(opengl::VertexAttrs::tagColor);
    mesh.vertexCount(vertexCount);
    mesh.indexCount(vertexCount, opengl::IndexTypes::UInt32);
    mesh.complete();

    vec3f* position = mesh.findAttributeTyped<vec3f>(opengl::VertexAttrs::tagPosition);
    vec3f* normal = nullptr;
    if (hasNormal)
        normal = mesh.findAttributeTyped<vec3f>(opengl::VertexAttrs::tagNormal);
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
            position[idx] = posData[v.pos];

            if (hasNormal)
                normal[idx] = normalData[v.normal];

            if (hasColor)
                color[idx] = colorData[v.color];

            if (hasUV)
                uv[idx] = uvData[v.uv];

            indeces[idx] = idx;
        }
    }
}

void MeshBuilder::createCube()
{
    vec3f x(1, 0, 0);
    vec3f y(0, 1, 0);
    vec3f z(0, 0, 1);
    vec3f ori(-0.5f, -0.5f, -0.5f);

    std::vector<vec3f> pos;
    pos.push_back(ori);
    pos.push_back(ori + x);
    pos.push_back(ori + x + z);
    pos.push_back(ori + z);
    pos.push_back(ori + y);
    pos.push_back(ori + y + x);
    pos.push_back(ori + y + x + z);
    pos.push_back(ori + y + z);

    vec4f red(1, 0, 0, 1);
    vec4f green(0, 1, 0, 1);
    vec4f blue(0, 0, 1, 1);
    vec4f black(0, 0, 0, 1);
    addPolygonQuad(pos[0], pos[1], pos[2], pos[3], red);
    addPolygonQuad(pos[0], pos[1], pos[5], pos[4], green);
    addPolygonQuad(pos[2], pos[3], pos[7], pos[6], green);
    addPolygonQuad(pos[1], pos[2], pos[6], pos[5], blue);
    addPolygonQuad(pos[0], pos[4], pos[7], pos[3], blue);
    addPolygonQuad(pos[7], pos[6], pos[5], pos[4], red);
}

void MeshBuilder::createGrid()
{
    const u32 minorLineCount = 100;
    const u32 majorLineCount = 10;
    const f32 gridSize = 100.0f;
    f32 cellMinorSize = gridSize / static_cast<f32>(minorLineCount);
    f32 cellMajorSize = gridSize / static_cast<f32>(majorLineCount);

    math::vec4f minorLineColor(0, 0, 0, 1);
    math::vec4f majorLineColor(0, 1, 0, 1);

    f32 startPos = -gridSize / 2.0f;
    f32 endPos = gridSize / 2.0f;
    for (u32 i = 0; i <= minorLineCount; i++)
    {
        f32 pos = startPos + cellMinorSize * i;
        math::vec3f a(pos, 0.0f, startPos);
        math::vec3f b(pos, 0.0f, endPos);
        if (i % majorLineCount == 0)
            addLine(a, b, majorLineColor);
        else
            addLine(a, b, minorLineColor);
    }

    for (u32 i = 0; i <= minorLineCount; i++)
    {
        f32 pos = startPos + cellMinorSize * i;
        math::vec3f a(startPos, 0.0f, pos);
        math::vec3f b(endPos, 0.0f, pos);
        if (i % majorLineCount == 0)
            addLine(a, b, majorLineColor);
        else
            addLine(a, b, minorLineColor);
    }
}

void MeshBuilder::readOBJ(const std::string& filename)
{
    LexerPolicy policy(LexerPolicy::POLICY_PYTHON_COMMENT);
    policy.setWhitespaces(" \t\n\r/");

    Lexer lexer(filename, policy);

    while ( lexer.hasMoreData() )
    {
        std::string token = lexer.readToken();
        if (token == "v") {
            vec3f p;
            p.x = lexer.readFloat();
            p.y = lexer.readFloat();
            p.z = lexer.readFloat();
            posData.push_back(p);
        } else if (token == "vt") {
            vec2f v;
            v.x = lexer.readFloat();
            v.y = lexer.readFloat();
            uvData.push_back(v);
        } else if (token == "vn") {
            vec3f n;
            n.x = lexer.readFloat();
            n.y = lexer.readFloat();
            n.z = lexer.readFloat();
            n = normalize(n);
            normalData.push_back(n);
        } else if (token == "f") {
            u32 idx[3];
            for(u32 i=0; i<3; i++) {
                u32 vertexIdx = static_cast<u32>(lexer.readFloat());
                if ( uvData.size() != 0 && normalData.size() != 0 ) {
                    u32 uvIdx = static_cast<u32>(lexer.readFloat());
                    u32 normalIdx = static_cast<u32>(lexer.readFloat());
                    idx[i] = addVertex(posData[vertexIdx-1], uvData[uvIdx-1], normalData[normalIdx-1]);
                } else if ( uvData.size() != 0 ) {
                    u32 uvIdx = static_cast<u32>(lexer.readFloat());
                    idx[i] = addVertex(posData[vertexIdx-1], uvData[uvIdx-1]);
                } else if ( normalData.size() != 0 ) {
                    u32 normalIdx = static_cast<u32>(lexer.readFloat());
                    idx[i] = addVertex(posData[vertexIdx-1], normalData[normalIdx-1]);
                } else {
                    idx[i] = addVertex(posData[vertexIdx-1]);
                }
            }
            addPolygon(idx[0], idx[1], idx[2]);
        } else if (token == "mtllib") {
            lexer.readToken();
        } else if (token == "usemtl") {
            lexer.readToken();
        } else if (token == "g") {
            lexer.readToken();
        } else {
            ERR("unknown token in OBJ: '%s'", token.c_str());
        }
    }
}

} // namespace imp
} // namespace base
