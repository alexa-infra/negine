/**
 * @author  Alexey Vasilyev <alexa.infra@gmail.com>
 * @section LICENSE
 * 
 * This file is part of Negine (https://alexadotlife.com/hg/negine)
 * Licensed under LGPLv3 (or GPLv3) - see LICENSE file for details
 **/
#pragma once

#include "base/types.h"
#include "base/math/vector.h"
#include <list>
#include <vector>

namespace base {
namespace opengl {

using namespace base::math;

//! Simple vertex
struct Vertex
{
    Vector3 pos;        //!< Position in 3D space
    Vector3 n;          //!< Normal to surface
    Vector2 tex;        //!< Texture coordinates
    Vector4 color;      //!< Color of vertex
};

namespace VertexAttrs
{
    //! Tags for vertex components
    enum VertexAttr
    {
        tagPosition,
        tagNormal,
        tagTexture,
        tagColor
    };

    //! Gets offset of attribute in vertex structure
    u8 GetOffset(VertexAttr attr);

    //! Gets component count in attribute
    u8 GetComponentCount(VertexAttr attr);
}
typedef VertexAttrs::VertexAttr VertexAttr;

typedef std::list< std::pair<VertexAttr, u32> > AttributeBinding;

//! Triangle face
struct Face
{
    u32 index[3];       //!< Indexes of vertexes in vertex array
    Vector3 n;          //!< Normal of face
};

//! Single edge
struct Edge
{
    u32 a, b;           //!< Indexes of vertexes
    u32 c, d;           //!< Indexes of faces
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
};


} // namespace opengl
} // base
