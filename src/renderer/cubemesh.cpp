#include "renderer/cubemesh.h"

namespace base
{
namespace opengl
{

using namespace math;

CubeMesh::CubeMesh()
	: buffer_(nullptr)
{
	std::vector<Vertex> vv;
	std::vector<Face> ff;
	AddCube( vv, ff, Vector3( 0.0f ) );
	buffer_ = new VertexBufferMemory( &vv.front(), vv.size(), &ff.front(), ff.size() );
}

CubeMesh::~CubeMesh()
{
	delete buffer_;
}

void CubeMesh::draw(const AttributeBinding& binding)
{
	buffer_->Draw( binding );
}

CubeMesh::Cube CubeMesh::AddCube( std::vector<Vertex>& v,
				std::vector<Face>& f,
		const math::Vector3& position ) const
{
	f32 s = 10;
	Vector3 v0 = Vector3( 0.0f, 0.0f, 0.0f ) + position;
	Vector3 v1 = Vector3( 0.0f, 0.0f, s ) + position;
	Vector3 v2 = Vector3( s, 0.0f, s ) + position;
	Vector3 v3 = Vector3( s, 0.0f, 0.0f ) + position;
	Vector3 v4 = Vector3( 0.0f, s, 0.0f ) + position;
	Vector3 v5 = Vector3( 0.0f, s, s ) + position;
	Vector3 v6 = Vector3( s, s, s ) + position;
	Vector3 v7 = Vector3( s, s, 0.0f ) + position;
	Cube cube;
	cube.face_start = f.size();
	cube.max = v6;
	cube.min = v0;
	AddCubeSide( v, f, v0, v1, v2, v3, Vector3( 0.0f, -1.0f, 0.0f ) );
	AddCubeSide( v, f, v1, v5, v6, v2, Vector3( 0.0f, 0.0f, 1.0f ) );
	AddCubeSide( v, f, v5, v4, v7, v6, Vector3( 0.0f, 0.0f, 1.0f ) );
	AddCubeSide( v, f, v4, v0, v3, v7, Vector3( 0.0f, 0.0f, -1.0f ) );
	AddCubeSide( v, f, v0, v4, v5, v1, Vector3( -1.0f, 0.0f, 0.0f ) );
	AddCubeSide( v, f, v2, v6, v7, v3, Vector3( 1.0f, 0.0f, 0.0f ) );
	return cube;
}

void CubeMesh::SetVertexN( Vertex* v, const math::Vector3& n ) const {
	for( u32 i = 0; i < 4; i++ ) {
		v[i].n = n;
	}
}

void CubeMesh::SetVertexUV( Vertex* v ) const {
	v[0].tex = math::Vector2( 0.0f, 0.0f );
	v[1].tex = math::Vector2( 1.0f, 0.0f );
	v[2].tex = math::Vector2( 1.0f, 1.0f );
	v[3].tex = math::Vector2( 0.0f, 1.0f );
}

void CubeMesh::SetVertexColor( Vertex* v ) const
{
	v[0].color = math::Vector4( 1.0f, 0.0f, 0.0f, 1.0f );
	v[1].color = math::Vector4( 0.0f, 1.0f, 0.0f, 1.0f );
	v[2].color = math::Vector4( 0.0f, 0.0f, 1.0f, 1.0f );
	v[3].color = math::Vector4( 1.0f, 1.0f, 1.0f, 1.0f );
}

void CubeMesh::SetVertexPos( Vertex* v, const Vector3& v0, const Vector3& v1, const Vector3& v2, const Vector3& v3 ) const {
	v[0].pos = v0;
	v[1].pos = v1;
	v[2].pos = v2;
	v[3].pos = v3;
}

Face CubeMesh::SetFace( i16 i1, i16 i2, i16 i3 ) const {
	Face f;
	f.index[0] = i1;
	f.index[1] = i2;
	f.index[2] = i3;
	return f;
}
void CubeMesh::AddCubeSide( std::vector<Vertex>& vert,
					std::vector<Face>& face,
						const Vector3& v0,
						const Vector3& v1,
						const Vector3& v2,
						const Vector3& v3,
						const Vector3& n ) const
{
	Vertex v[4];
	SetVertexPos( v, v0, v1, v2, v3 );
	SetVertexUV( v );
	SetVertexN( v, n );
	SetVertexColor( v );
	i16 i = static_cast<i16>( vert.size() );
	face.push_back( SetFace( i + 0, i + 1, i + 2 ) );
	face.push_back( SetFace( i + 2, i + 3, i + 0 ) );

	for( int j = 0; j < 4; j++ ) {
		vert.push_back( v[j] );
	}
}

}
}