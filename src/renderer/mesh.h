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
#include <vector>

namespace base {
namespace opengl {

using base::math::Vector2;
using base::math::Vector3;
using base::math::Vector4;

//! Simple vertex
struct Vertex
{
    Vector3 pos;        //!< Position in 3D space
    Vector3 n;          //!< Normal to surface
    Vector2 tex;        //!< Texture coordinates
    Vector3 tangent;    //!< Tangent
    Vector3 binormal;   //!< Binormal
    Vector4 color;      //!< Color of vertex

    Vertex() {}
};

namespace VertexAttrs
{
    //! Tags for vertex components
    enum VertexAttr
    {
        tagPosition,
        tagNormal,
        tagTexture,
        tagTangent,
        tagBinormal,
        tagColor,

        Count
    };

    //! Gets offset of attribute in vertex structure
    uptr GetOffset(VertexAttr attr);

    //! Gets component count in attribute
    u8 GetComponentCount(VertexAttr attr);
}
typedef VertexAttrs::VertexAttr VertexAttr;

typedef std::list< std::pair<VertexAttr, u32> > AttributeBinding;

//! Triangle face
struct Face
{
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
