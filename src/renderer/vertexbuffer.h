/**
 * \file
 * \brief       vertex buffer obejct wrapper
 * \author      Alexey Vasilyev <alexa.infra@gmail.com>
 * \copyright   MIT License
 **/
#pragma once

#include "renderer/mesh.h"
#include "renderer/glbuffer.h"

namespace base {
namespace opengl {

//! Wraps vertex buffer - combination of two buffers: vertex and index arrays
//! Uses triangles as data, and u16 indexes
class VertexBuffer {
protected:
    u32 vertex_count_;              //!< Number of vertexes
    u32 faces_count_;               //!< Number of indexes

public:
    VertexBuffer();

    //! Sets up data in buffer
    //! \param          vertexes        Array of vertex data
    //! \param          vcount          Length of vertex array
    //! \param          faces           Array of faces (triangles)
    //! \param          fcount          Length of faces array
    //! \param          usage           Expected usage of buffer
    virtual void SetData(Vertex* vertexes, u16 vcount, Face* faces, u16 fcount, BufferUsage usage = BufferUsages::DynamicDraw) = 0;

    virtual ~VertexBuffer() {}

    //! Maps GPU buffer of vertex-buffer-object to memory
    virtual Vertex* Lock(BufferAccess access = BufferAccesses::ReadWrite) = 0;
    
    //! Unmap vertex-buffer-object
    virtual void Unlock() = 0;

    //! Perform drawing of triangles on vertex-buffer-object, according to attribute binding at current shader program
    void Draw(const AttributeBinding& binding) {
        Draw(binding, 0, faces_count());
    }

    //! Perform drawing of specified triangle range on vertex-buffer-object
    void Draw(const AttributeBinding& binding, u32 from_face, u32 count_faces) {
        BindAttributes(binding);
        DrawOnly(from_face, count_faces);
        UnbindAttributes(binding);
    }

    //! Draw without bind-unbind attributes
    virtual void DrawOnly(u32 from_face, u32 count_faces) = 0;

    //! Gets number of triangles
    u32 faces_count() const { return faces_count_; }

    //! Get number of indexes
    u32 index_count() const { return faces_count_ * 3; }

    //! Gets size of index buffer
    u32 index_array_size() const { return index_count() * sizeof(u16); }

    //! Gets number of vertexes
    u32 vertex_count() const { return vertex_count_; }

    //! Gets size of vertex buffer
    u32 vertex_array_size() const { return vertex_count_ * sizeof(Vertex); }

    //! Convert face to index
    u32 face_to_index(u32 face) const { return face * 3; }

    //! Bind components of vertex to active attributes in current shader program
    virtual void BindAttributes(const AttributeBinding& binding) = 0;

    //! Unbind attributes
    virtual void UnbindAttributes(const AttributeBinding& binding) = 0;

private:
    DISALLOW_COPY_AND_ASSIGN(VertexBuffer);
};

class VertexBufferGPU : public VertexBuffer {
protected:
    GLBufferObject* vertexes_;      //!< Vertex buffer object
    GLBufferObject* indexes_;       //!< Index buffer object

public:
    VertexBufferGPU();
    VertexBufferGPU(Vertex* vertexes, u16 vcount, Face* faces, u16 fcount, BufferUsage usage = BufferUsages::DynamicDraw); 

    //! Sets up data in buffer
    //! \param          vertexes        Array of vertex data
    //! \param          vcount          Length of vertex array
    //! \param          faces           Array of faces (triangles)
    //! \param          fcount          Length of faces array
    //! \param          usage           Expected usage of buffer
    void SetData(Vertex* vertexes, u16 vcount, Face* faces, u16 fcount, BufferUsage usage = BufferUsages::DynamicDraw);

    ~VertexBufferGPU();

    //! Maps GPU buffer of vertex-buffer-object to memory
    Vertex* Lock(BufferAccess access = BufferAccesses::ReadWrite);
    
    //! Unmap vertex-buffer-object
    void Unlock();

    //! Draw without bind-unbind attributes
    void DrawOnly(u32 from_face, u32 count_faces);

    //! Bind components of vertex to active attributes in current shader program
    void BindAttributes(const AttributeBinding& binding);

    //! Unbind attributes
    void UnbindAttributes(const AttributeBinding& binding);

private:
    DISALLOW_COPY_AND_ASSIGN(VertexBufferGPU);
};

class VertexBufferMemory : public VertexBuffer {
protected:
    Vertex* vertexes_;              //!< Vertex buffer
    Face* indexes_;                 //!< Index buffer

public:
    VertexBufferMemory();
    VertexBufferMemory(Vertex* vertexes, u16 vcount, Face* faces, u16 fcount, BufferUsage usage = BufferUsages::DynamicDraw); 

    //! Sets up data in buffer
    //! \param          vertexes        Array of vertex data
    //! \param          vcount          Length of vertex array
    //! \param          faces           Array of faces (triangles)
    //! \param          fcount          Length of faces array
    //! \param          usage           Expected usage of buffer
    void SetData(Vertex* vertexes, u16 vcount, Face* faces, u16 fcount, BufferUsage usage = BufferUsages::DynamicDraw);

    ~VertexBufferMemory();

    //! Maps GPU buffer of vertex-buffer-object to memory
    Vertex* Lock(BufferAccess access = BufferAccesses::ReadWrite) { return vertexes_; }
    
    //! Unmap vertex-buffer-object
    void Unlock() {}

    //! Draw without bind-unbind attributes
    void DrawOnly(u32 from_face, u32 count_faces);

    //! Bind components of vertex to active attributes in current shader program
    void BindAttributes(const AttributeBinding& binding);

    //! Unbind attributes
    void UnbindAttributes(const AttributeBinding& binding);

private:
    DISALLOW_COPY_AND_ASSIGN(VertexBufferMemory);
};

}
}