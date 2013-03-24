#include "render/cubemesh.h"
#include "render/glcontext.h"

namespace base
{
namespace opengl
{

using namespace math;

CubeMesh::CubeMesh()
	: buffer_(nullptr)
{
    std::vector<CubeMesh::CubeVertex> vv;
    std::vector<u32> ff;
    AddCube( vv, ff, Vector3( 0.0f ) );
//    buffer_ = new VertexBuffer();
    buffer_->SetVertexData(&vv.front(), vv.size() * sizeof(CubeVertex));
    buffer_->SetIndexData(&ff.front(), ff.size() * sizeof(u32));
    buffer_->EnableAttribute(VertexAttrs::tagPosition, sizeof(CubeVertex), (void*)offsetof(CubeVertex, position));
    buffer_->EnableAttribute(VertexAttrs::tagNormal, sizeof(CubeVertex), (void*)offsetof(CubeVertex, normal));
    //buffer_->EnableAttribute(VertexAttrs::tagTexture, sizeof(CubeVertex), (void*)offsetof(CubeVertex, tex));
    buffer_->EnableAttribute(VertexAttrs::tagColor, sizeof(CubeVertex), (void*)offsetof(CubeVertex, color));
    buffer_->Load();
}

CubeMesh::~CubeMesh()
{
    delete buffer_;
}

void CubeMesh::draw()
{
    buffer_->BindAttributes( );
    //glDrawElements(GL_TRIANGLES,
    //    6 * 2 * 3,
    //    GL_UNSIGNED_INT,
    //    0);
    buffer_->UnbindAttributes();
}

CubeMesh::Cube CubeMesh::AddCube( std::vector<CubeMesh::CubeVertex>& v,
				std::vector<u32>& f,
		const math::Vector3& position ) const
{
	f32 s = 5;
	Vector3 v0 = Vector3( -s, -s, -s ) + position;
	Vector3 v1 = Vector3( -s, -s, s ) + position;
	Vector3 v2 = Vector3( s, -s, s ) + position;
	Vector3 v3 = Vector3( s, -s, -s ) + position;
	Vector3 v4 = Vector3( -s, s, -s ) + position;
	Vector3 v5 = Vector3( -s, s, s ) + position;
	Vector3 v6 = Vector3( s, s, s ) + position;
	Vector3 v7 = Vector3( s, s, -s ) + position;
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

void CubeMesh::SetVertexN( CubeMesh::CubeVertex* v, const math::Vector3& n ) const {
	for( u32 i = 0; i < 4; i++ ) {
		v[i].normal = n;
	}
}

void CubeMesh::SetVertexUV( CubeMesh::CubeVertex* v ) const {
	v[0].tex = math::Vector2( 0.0f, 0.0f );
	v[1].tex = math::Vector2( 1.0f, 0.0f );
	v[2].tex = math::Vector2( 1.0f, 1.0f );
	v[3].tex = math::Vector2( 0.0f, 1.0f );
}

void CubeMesh::SetVertexColor( CubeMesh::CubeVertex* v ) const
{
	v[0].color = math::Vector4( 1.0f, 0.0f, 0.0f, 1.0f );
	v[1].color = math::Vector4( 0.0f, 1.0f, 0.0f, 1.0f );
	v[2].color = math::Vector4( 0.0f, 0.0f, 1.0f, 1.0f );
	v[3].color = math::Vector4( 1.0f, 1.0f, 1.0f, 1.0f );
}

void CubeMesh::SetVertexPos( CubeMesh::CubeVertex* v, const Vector3& v0, const Vector3& v1, const Vector3& v2, const Vector3& v3 ) const {
	v[0].position = v0;
	v[1].position = v1;
	v[2].position = v2;
	v[3].position = v3;
}

void CubeMesh::SetFace( std::vector<u32>& face, u32 i1, u32 i2, u32 i3 ) const {
    face.push_back(i1);
    face.push_back(i2);
    face.push_back(i3);
}
void CubeMesh::AddCubeSide( std::vector<CubeMesh::CubeVertex>& vert,
					std::vector<u32>& face,
						const Vector3& v0,
						const Vector3& v1,
						const Vector3& v2,
						const Vector3& v3,
						const Vector3& n ) const
{
	CubeMesh::CubeVertex v[4];
	SetVertexPos( v, v0, v1, v2, v3 );
	SetVertexUV( v );
	SetVertexN( v, n );
	SetVertexColor( v );
	u32 i = vert.size();
	SetFace( face, i + 0, i + 1, i + 2 );
	SetFace( face, i + 2, i + 3, i + 0 );

	for( int j = 0; j < 4; j++ ) {
		vert.push_back( v[j] );
	}
}

}
}
