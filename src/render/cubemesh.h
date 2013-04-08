#pragma once

#include <vector>
#include "render/mesh.h"
#include "render/vertexbuffer.h"
#include "math/vec2.h"
#include "math/vec3.h"
#include "math/vec4.h"

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
        math::vec3f position;
        math::vec3f normal;
        math::vec2f tex;
        math::vec4f color;
    };
	struct Cube {
		math::vec3f max, min;
		i32 face_start;
	};

	CubeMesh();
	~CubeMesh();
	void draw( );
	Cube AddCube( std::vector<CubeVertex>& v,
					std::vector<u32>& f,
			const math::vec3f& position ) const;
private:
	void SetVertexN( CubeMesh::CubeVertex* v, const math::vec3f& n ) const;
	void SetVertexUV( CubeMesh::CubeVertex* v ) const;
	void SetVertexColor( CubeMesh::CubeVertex* v ) const;
	void SetVertexPos( CubeMesh::CubeVertex* v,
				const math::vec3f& v0,
				const math::vec3f& v1,
				const math::vec3f& v2,
				const math::vec3f& v3 ) const;
	void SetFace( std::vector<u32>& face, u32 i1, u32 i2, u32 i3 ) const;
	void AddCubeSide( std::vector<CubeVertex>& vert,
						std::vector<u32>& face,
							const math::vec3f& v0,
							const math::vec3f& v1,
							const math::vec3f& v2,
							const math::vec3f& v3,
							const math::vec3f& n ) const;
};

} // namespace opengl
} // namespace base
