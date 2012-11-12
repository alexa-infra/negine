/**
 * \file
 * \brief       defines mesh-related things (vertexes, etc)
 * \author      Alexey Vasilyev <alexa.infra@gmail.com>
 * \copyright   MIT License
 **/
#pragma once

#include "base/types.h"
#include "base/math/vector.h"
#include <list>
#include <map>
#include <vector>

namespace base
{
namespace opengl
{

//! Simple vertex
struct Vertex {
    math::Vector3 pos;        //!< Position in 3D space
    math::Vector3 n;          //!< Normal to surface
    math::Vector2 tex;        //!< Texture coordinates
    math::Vector3 tangent;    //!< Tangent
    math::Vector3 binormal;   //!< Binormal
    math::Vector4 color;      //!< Color of vertex

    Vertex() {}
};

namespace VertexAttrs
{
//! Tags for vertex components
enum VertexAttr {
    tagPosition,
    tagNormal,
    tagTexture,
    tagTangent,
    tagBinormal,
    tagColor,

    Count
};

//! Gets offset of attribute in vertex structure
uptr GetOffset( VertexAttr attr );

//! Gets component count in attribute
u8 GetComponentCount( VertexAttr attr );
}
typedef VertexAttrs::VertexAttr VertexAttr;

typedef std::map< VertexAttr, u32 > AttributeBinding;

//! Triangle face
struct Face {
    u16 index[3];       //!< Indexes of vertexes in vertex array
};

//! Mesh object, triangle data
class Mesh
{
public:
    std::string name;       //!< Name of mesh

    Vertex* vertexes;       //!< Vertexes array
    Face*   faces;          //!< Faces array
    u32     num_vertexes;   //!< Number of vertexes
    u32     num_faces;      //!< Number of faces

    Mesh();
    ~Mesh();
    void GenerateLightningInfo();
};


} // namespace opengl
} // base
