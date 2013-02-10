/**
 * \file
 * \brief       vertex buffer obejct wrapper
 * \author      Alexey Vasilyev <alexa.infra@gmail.com>
 * \copyright   MIT License
 **/
#pragma once

#include "render/mesh.h"
#include "render/glbuffer.h"

namespace base
{
namespace opengl
{

//! Wraps vertex buffer - combination of two buffers: vertex and index arrays
//! Uses triangles as data, and u16 indexes
class VertexBuffer
{
public:
    VertexBuffer();
    ~VertexBuffer();

    void SetVertexData( void* vertexes, u32 vertexCount );

    void SetIndexData( void* index, u32 indexCount );

    void EnableAttribute( VertexAttr attr, u32 stride, void* pointer );

    void EnableAttributeMesh( const MeshExt* mesh );

    void Load();

    //! Bind components of vertex to active attributes in current shader program
    void BindAttributes( );

    //! Unbind attributes
    void UnbindAttributes( );
private:
    GLBufferObject* vertexes_;      //!< Vertex buffer object
    GLBufferObject* indexes_;       //!< Index buffer object
    GLuint vao_;

    struct EnabledAttribute
    {
        bool enabled_;
        u32 stride_;
        void* pointer_;

        EnabledAttribute()
            : enabled_(false)
        {
        }
        EnabledAttribute(u32 stride, void* pointer)
            : enabled_(true), stride_(stride), pointer_(pointer)
        {
        }
    };
    std::array< EnabledAttribute, VertexAttrs::Count > enabledAttributes_;
private:
    DISALLOW_COPY_AND_ASSIGN( VertexBuffer );
};

}
}
