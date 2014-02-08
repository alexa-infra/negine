#include "meshbuilder.h"
#include "base/log.h"
#include "base/debug.h"
#include "render/mesh.h"

namespace base {
namespace imp {

using namespace math;
using namespace foundation;

MeshBuilder::MeshBuilder()
    : mask(0)
    , posData(memory_globals::default_allocator())
    , normalData(memory_globals::default_allocator())
    , uvData(memory_globals::default_allocator())
    , colorData(memory_globals::default_allocator())
    , surfaces(memory_globals::default_allocator())
    , vertexList(memory_globals::default_allocator())
    , polygonList(memory_globals::default_allocator())
    , lineList(memory_globals::default_allocator())
{
}

u32 MeshBuilder::addVertex(const vec3f& p)
{
    u32 pIdx = array::size(posData);
    array::push_back(posData, p);
    Vertex v(pIdx);
    u32 idx = array::size(vertexList);
    array::push_back(vertexList, v);
    return idx;
}

u32 MeshBuilder::addVertex(const vec3f& p, const vec2f& uv)
{
    mask |= hasVertexUV;
    u32 idx = addVertex(p);
    Vertex& vertex = vertexList[idx];
    vertex.uv = array::size(uvData);
    array::push_back(uvData, uv);
    return idx;
}

u32 MeshBuilder::addVertex(const vec3f& p, const vec3f& n)
{
    mask |= hasVertexNormal;
    u32 idx = addVertex(p);
    Vertex& vertex = vertexList[idx];
    vertex.normal = array::size(normalData);
    array::push_back(normalData, n);
    return idx;
}

u32 MeshBuilder::addVertex(const vec3f& p, const vec2f& uv, const vec3f& n)
{
    u32 idx = addVertex(p, uv);
    mask |= hasVertexNormal;
    mask |= hasVertexUV;
    Vertex& vertex = vertexList[idx];
    vertex.uv = array::size(uvData);
    array::push_back(uvData, uv);
    vertex.normal = array::size(normalData);
    array::push_back(normalData, n);
    return idx;
}

u32 MeshBuilder::addVertex(const vec3f& p, const vec4f& color)
{
    mask |= hasVertexColor;
    u32 idx = addVertex(p);
    Vertex& vertex = vertexList[idx];
    vertex.color = array::size(colorData);
    array::push_back(colorData, color);
    return idx;
}

u32 MeshBuilder::addVertex(i32 p, i32 n, i32 v, i32 c)
{
    Vertex ver(u32(-1), u32(-1), u32(-1), u32(-1));
    if (p < 0)
        ver.pos = array::size(posData) + p;
    else if (p > 0)
        ver.pos = currentSurface.pos + p - 1;

    if (n < 0)
        ver.normal = array::size(normalData) + n;
    else if (n > 0)
        ver.normal = currentSurface.normal + n - 1;

    if (v < 0)
        ver.uv = array::size(uvData) + v;
    else if (v > 0)
        ver.uv = currentSurface.uv + v - 1;

    if (c < 0)
        ver.color = array::size(colorData) + c;
    else if (c > 0)
        ver.color = currentSurface.uv + c - 1;

    u32 idx = array::size(vertexList);
    array::push_back(vertexList, ver);
    return idx;
}

void MeshBuilder::addPolygonQuad(const vec3f& a1, const vec3f& a2, const vec3f& a3, const vec3f& a4, const vec4f& color)
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
    u32 idx = array::size(polygonList);
    array::push_back(polygonList, p);
    return idx;
}

u32 MeshBuilder::addLine(u32 x, u32 y)
{
    Line line;
    line.v[0] = x;
    line.v[1] = y;
    u32 idx = array::size(lineList);
    array::push_back(lineList, line);
    return idx;
}

void MeshBuilder::beginSurface()
{
    Surface s;
    s.pos = array::size(posData);
    s.normal = array::size(normalData);
    s.uv = array::size(uvData);
    s.color = array::size(colorData);
    s.vertex = array::size(vertexList);
    s.polygon = array::size(polygonList);

    currentSurface = s;
}

void MeshBuilder::endSurface()
{
    currentSurface.polygonCount = array::size(polygonList) - currentSurface.polygon;
    array::push_back(surfaces, currentSurface);
}

u32 MeshBuilder::addLine(const vec3f& a, const vec3f& b, const vec4f& color)
{
    u32 aIdx = addVertex(a, color);
    u32 bIdx = addVertex(b, color);
    return addLine(aIdx, bIdx);
}

void MeshBuilder::getLineList(opengl::Mesh& mesh)
{
    u32 lineCount = array::size(lineList);
    u32 vertexCount = lineCount * 2;
    mesh.addAttribute(opengl::VertexAttrs::tagPosition);
    mesh.addAttribute(opengl::VertexAttrs::tagColor);
    mesh.vertexCount(vertexCount);
    mesh.indexCount(vertexCount, opengl::IndexTypes::UInt32);
    mesh.complete();

    vec3f* position = mesh.findAttribute<vec3f>(opengl::VertexAttrs::tagPosition);
    vec4f* color = mesh.findAttribute<vec4f>(opengl::VertexAttrs::tagColor);
    u32* indeces = reinterpret_cast<u32*>(mesh.indices());
    bool vertexColor = (mask & hasVertexColor) == hasVertexColor;

    u32 idx = 0;
    for(u32 i=0; i<lineCount; i++) {
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
    u32 polygonCount = array::size(polygonList);
    u32 vertexCount = polygonCount * 3;
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

    vec3f* position = mesh.findAttribute<vec3f>(opengl::VertexAttrs::tagPosition);
    vec3f* normal = nullptr;
    if (hasNormal)
        normal = mesh.findAttribute<vec3f>(opengl::VertexAttrs::tagNormal);
    vec4f* color = nullptr;
    if (hasColor)
        color = mesh.findAttribute<vec4f>(opengl::VertexAttrs::tagColor);
    vec2f* uv = nullptr;
    if (hasUV)
        uv = mesh.findAttribute<vec2f>(opengl::VertexAttrs::tagTexture);
    u32* indeces = reinterpret_cast<u32*>(mesh.indices());

    for(u32 i=0, j=0; i<polygonCount; i++, j+=3)
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
    const vec3f x = {1, 0, 0};
    const vec3f y = {0, 1, 0};
    const vec3f z = {0, 0, 1};
    const vec3f ori = {-0.5f, -0.5f, -0.5f};
    
    vec3f pos[8];
    pos[0] = ori;
    pos[1] = ori + x;
    pos[2] = pos[1] + z;
    pos[3] = ori + z;
    pos[4] = ori + y;
    pos[5] = pos[4] + x;
    pos[6] = pos[5] + z;
    pos[7] = pos[4] + z;
    
    const vec4f red   = { 1, 0, 0, 1 };
    const vec4f green = { 0, 1, 0, 1 };
    const vec4f blue  = { 0, 0, 1, 1 };
    const vec4f black = { 0, 0, 0, 1 };

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

    const f32 cellMinorSize = gridSize / static_cast<f32>(minorLineCount);
    const f32 cellMajorSize = gridSize / static_cast<f32>(majorLineCount);

    const vec4f minorLineColor = {0, 0, 0, 1};
    const vec4f majorLineColor = {0, 1, 0, 1};

    const f32 startPos = -gridSize / 2.0f;
    const f32 endPos = gridSize / 2.0f;
    for (u32 i = 0; i <= minorLineCount; i++)
    {
        const f32 pos = startPos + cellMinorSize * i;
        const vec3f a = {pos, 0.0f, startPos};
        const vec3f b = {pos, 0.0f, endPos};
        if (i % majorLineCount == 0)
            addLine(a, b, majorLineColor);
        else
            addLine(a, b, minorLineColor);
    }

    for (u32 i = 0; i <= minorLineCount; i++)
    {
        const f32 pos = startPos + cellMinorSize * i;
        const vec3f a = {startPos, 0.0f, pos};
        const vec3f b = {endPos, 0.0f, pos};
        if (i % majorLineCount == 0)
            addLine(a, b, majorLineColor);
        else
            addLine(a, b, minorLineColor);
    }
}

} // namespace imp
} // namespace base
