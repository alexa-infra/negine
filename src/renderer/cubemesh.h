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
	struct Cube {
		math::Vector3 max, min;
		i32 face_start;
	};

	CubeMesh();
	~CubeMesh();
	void draw(const AttributeBinding& binding);
	Cube AddCube( std::vector<Vertex>& v,
					std::vector<Face>& f,
			const math::Vector3& position ) const;
private:
	void SetVertexN( Vertex* v, const math::Vector3& n ) const;
	void SetVertexUV( Vertex* v ) const;
	void SetVertexColor( Vertex* v ) const;
	void SetVertexPos( Vertex* v,
				const math::Vector3& v0,
				const math::Vector3& v1,
				const math::Vector3& v2,
				const math::Vector3& v3 ) const;
	Face SetFace( i16 i1, i16 i2, i16 i3 ) const;
	void AddCubeSide( std::vector<Vertex>& vert,
						std::vector<Face>& face,
							const math::Vector3& v0,
							const math::Vector3& v1,
							const math::Vector3& v2,
							const math::Vector3& v3,
							const math::Vector3& n ) const;
};

} // namespace opengl
} // namespace base