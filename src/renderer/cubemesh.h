#pragma once

#include <vector>
#include "renderer/mesh.h"
#include "renderer/vertexbuffer.h"

namespace base
{
namespace opengl
{

class CubeMesh
{
private:
	VertexBuffer* buffer_;
public:
    struct CubeVertex {
        math::Vector3 position;
        math::Vector3 normal;
        math::Vector2 tex;
        math::Vector4 color;
    };
	struct Cube {
		math::Vector3 max, min;
		i32 face_start;
	};

	CubeMesh();
	~CubeMesh();
	void draw( );
	Cube AddCube( std::vector<CubeVertex>& v,
					std::vector<u32>& f,
			const math::Vector3& position ) const;
private:
	void SetVertexN( CubeMesh::CubeVertex* v, const math::Vector3& n ) const;
	void SetVertexUV( CubeMesh::CubeVertex* v ) const;
	void SetVertexColor( CubeMesh::CubeVertex* v ) const;
	void SetVertexPos( CubeMesh::CubeVertex* v,
				const math::Vector3& v0,
				const math::Vector3& v1,
				const math::Vector3& v2,
				const math::Vector3& v3 ) const;
	void SetFace( std::vector<u32>& face, u32 i1, u32 i2, u32 i3 ) const;
	void AddCubeSide( std::vector<CubeVertex>& vert,
						std::vector<u32>& face,
							const math::Vector3& v0,
							const math::Vector3& v1,
							const math::Vector3& v2,
							const math::Vector3& v3,
							const math::Vector3& n ) const;
};

} // namespace opengl
} // namespace base
