#include "base/math/matrix.h"

#include <string.h>

namespace base {
namespace math {

const Matrix4 Matrix4::Zero(0.f, 0.f, 0.f, 0.f,
                            0.f, 0.f, 0.f, 0.f,
                            0.f, 0.f, 0.f, 0.f,
                            0.f, 0.f, 0.f, 0.f);

const Matrix4 Matrix4::Identity(1.f, 0.f, 0.f, 0.f,
                            0.f, 1.f, 0.f, 0.f,
                            0.f, 0.f, 1.f, 0.f,
                            0.f, 0.f, 0.f, 1.f);

Matrix4::Matrix4()
    : xx(1.f), xy(0.f), xz(0.f), xw(0.f)
    , yx(0.f), yy(1.f), yz(0.f), yw(0.f)
    , zx(0.f), zy(0.f), zz(1.f), zw(0.f)
    , wx(0.f), wy(0.f), wz(0.f), ww(1.f) {
}

Matrix4::Matrix4(f32 vxx, f32 vxy, f32 vxz, f32 vxw, f32 vyx, f32 vyy, f32 vyz, f32 vyw, f32 vzx, f32 vzy, f32 vzz, f32 vzw, f32 vwx, f32 vwy, f32 vwz, f32 vww)
    : xx(vxx), xy(vxy), xz(vxz), xw(vxw)
    , yx(vyx), yy(vyy), yz(vyz), yw(vyw)
    , zx(vzx), zy(vzy), zz(vzz), zw(vzw)
    , wx(vwx), wy(vwy), wz(vwz), ww(vww) {
}


Matrix4::Matrix4(const f32 *m) {
	memcpy(array1d, m, sizeof(array1d));
}

Matrix4 Matrix4::GetScale(const Vector3 &_scale_vector) {
	Matrix4 _matrix = Zero;

	_matrix.array2d[0][0]	= _scale_vector.x;
	_matrix.array2d[1][1]	= _scale_vector.y;
	_matrix.array2d[2][2]	= _scale_vector.z;
	_matrix.array2d[3][3]	= 1.0f;

	return _matrix;
}

Matrix4 Matrix4::GetTranslation(const Vector3 &_translation_vector) {
	Matrix4 _matrix = Identity;

	_matrix.array2d[0][3]	= _translation_vector.x;
	_matrix.array2d[1][3]	= _translation_vector.y;
	_matrix.array2d[2][3]	= _translation_vector.z;

	return _matrix;
}

Matrix4 Matrix4::GetRotation(const Vector3 &_axis, const f32 &_angle) {
	Matrix4 _matrix = Identity;

	Vector3 __axis = _axis;
	__axis.Normalize();

	f32 c = cosf(_angle);
	f32 s = sinf(_angle);
	f32 t = 1.0f - c;

	_matrix.array2d[0][0]  = t*__axis.x*__axis.x + c;
    _matrix.array2d[0][1]  = t*__axis.x*__axis.y - s*__axis.z;	
    _matrix.array2d[0][2]  = t*__axis.x*__axis.z + s*__axis.y;
	_matrix.array2d[1][0]  = t*__axis.x*__axis.y + s*__axis.z;	
    _matrix.array2d[1][1]  = t*__axis.y*__axis.y + c; 			
    _matrix.array2d[1][2]  = t*__axis.y*__axis.z - s*__axis.x;
	_matrix.array2d[2][0]  = t*__axis.x*__axis.z - s*__axis.y;	
    _matrix.array2d[2][1]  = t*__axis.y*__axis.z + s*__axis.x;	
    _matrix.array2d[2][2] = t*__axis.z*__axis.z + c;

	return _matrix;
}

Matrix4 Matrix4::GetRotationX(const f32 &_angle) {
	Matrix4 _matrix = Identity;

	f32 c = cosf(_angle);
	f32 s = sinf(_angle);

	_matrix.array2d[1][1]  = c;
	_matrix.array2d[1][2]  = -s;
	_matrix.array2d[2][1]  = s;
	_matrix.array2d[2][2] = c;

	return _matrix;
}

Matrix4 Matrix4::GetRotationY(const f32 &_angle) {
	Matrix4 _matrix = Identity;

	f32 c = cosf(_angle);
	f32 s = sinf(_angle);

	_matrix.array2d[0][0]  = c;
	_matrix.array2d[0][2]  = s;
	_matrix.array2d[2][0]  = -s;
	_matrix.array2d[2][2] = c;

	return _matrix;
}

Matrix4 Matrix4::GetRotationZ(const f32 &_angle) {
	Matrix4 _matrix = Identity;

	f32 c = cosf(_angle);
	f32 s = sinf(_angle);

	_matrix.array2d[0][0] = c;
	_matrix.array2d[0][1] = -s;
	_matrix.array2d[1][0] = s;
	_matrix.array2d[1][1] = c;

	return _matrix;
}

Matrix4 Matrix4::GetProjection(const Plane &_plane) {
	Matrix4 _matrix = Identity;

	_matrix.array2d[0][0] = 1 - _plane.A()*_plane.A();	
    _matrix.array2d[0][1] = -_plane.A()*_plane.B();		
    _matrix.array2d[0][2] = -_plane.A()*_plane.C();		
    _matrix.array2d[0][3] = -_plane.A()*_plane.D();
	_matrix.array2d[1][0] = -_plane.A()*_plane.B();		
    _matrix.array2d[1][1] = 1 - _plane.B()*_plane.B();	
    _matrix.array2d[1][2] = -_plane.B()*_plane.C();		
    _matrix.array2d[1][3] = -_plane.B()*_plane.D();
	_matrix.array2d[2][0] = -_plane.A()*_plane.C();		
    _matrix.array2d[2][1] = -_plane.B()*_plane.C();		
    _matrix.array2d[2][2] = 1 - _plane.C()*_plane.C();	
    _matrix.array2d[2][3] = -_plane.C()*_plane.D();

	return _matrix;
}

Matrix4 Matrix4::GetReflection(const Plane &_plane) {
	Matrix4 _matrix = Identity;

	_matrix.array2d[0][0]  = 1 - 2*_plane.A()*_plane.A();	
    _matrix.array2d[0][1]  = -2*_plane.A()*_plane.B();		
    _matrix.array2d[0][2]  = -2*_plane.A()*_plane.C();		
    _matrix.array2d[0][3]  = -2*_plane.A()*_plane.D();
	_matrix.array2d[1][0]  = -2*_plane.A()*_plane.B(); 	
    _matrix.array2d[1][1]  = 1 - 2*_plane.B()*_plane.B();	
    _matrix.array2d[1][2]  = -2*_plane.B()*_plane.C();		
    _matrix.array2d[1][3]  = -2*_plane.B()*_plane.D();
	_matrix.array2d[2][0]  = -2*_plane.A()*_plane.C();		
    _matrix.array2d[2][1]  = -2*_plane.B()*_plane.C();		
    _matrix.array2d[2][2] = 1 - 2*_plane.C()*_plane.C();	
    _matrix.array2d[2][3] = -2*_plane.C()*_plane.D();

	return _matrix;
}

void Matrix4::SetIdentity() {
	*this = Identity;
}

void Matrix4::Scale(const Vector3 &_scale_vector) {
	*this *= GetScale(_scale_vector);
}

void Matrix4::Translate(const Vector3 &_translation_vector) {
	*this *= GetTranslation(_translation_vector);
}

void Matrix4::Rotate(const Vector3 &_axis, const f32 &_angle) {
	*this *= GetRotation(_axis, _angle);
}

void Matrix4::RotateX(const f32 &_angle) {
	*this *= GetRotationX(_angle);
}

void Matrix4::RotateY(const f32 &_angle) {
	*this *= GetRotationY(_angle);
}

void Matrix4::RotateZ(const f32 &_angle) {
	*this *= GetRotationZ(_angle);
}

void Matrix4::Project(const Plane &_plane) {
	*this *= GetProjection(_plane);
}

void Matrix4::Reflect(const Plane &_plane) {
	*this *= GetReflection(_plane);
}

void Matrix4::Transpose() {
/*	MatrixSwap(0, 1);
	MatrixSwap(0, 2);
	MatrixSwap(0, 3);
	MatrixSwap(1, 2);
	MatrixSwap(1, 3);
	MatrixSwap(2, 3);*/
}

void Matrix4::Invert() {
	Matrix4 _matrix;

	int i;

	f32 tmp[12];
	f32 src[16];
	f32 det;

	for (i = 0; i < 4; i++)
	{
		src[i + 0] = array2d[i][0];
		src[i + 4] = array2d[i][1];
		src[i + 8] = array2d[i][2];
		src[i + 12] = array2d[i][3];
	}

	tmp[0] = src[10]*src[15];
	tmp[1] = src[11]*src[14];
	tmp[2] = src[9]*src[15];
	tmp[3] = src[11]*src[13];
	tmp[4] = src[9]*src[14];
	tmp[5] = src[10]*src[13];
	tmp[6] = src[8]*src[15];
	tmp[7] = src[11]*src[12];
	tmp[8] = src[8]*src[14];
	tmp[9] = src[10]*src[12];
	tmp[10] = src[8]*src[13];
	tmp[11] = src[9]*src[12];

	_matrix.array2d[0][0] = tmp[0]*src[5] + tmp[3]*src[6] + tmp[4]*src[7];
	_matrix.array2d[0][0]-= tmp[1]*src[5] + tmp[2]*src[6] + tmp[5]*src[7];
	_matrix.array2d[0][1] = tmp[1]*src[4] + tmp[6]*src[6] + tmp[9]*src[7];
	_matrix.array2d[0][1]-= tmp[0]*src[4] + tmp[7]*src[6] + tmp[8]*src[7];
	_matrix.array2d[0][2] = tmp[2]*src[4] + tmp[7]*src[5] + tmp[10]*src[7];
	_matrix.array2d[0][2]-= tmp[3]*src[4] + tmp[6]*src[5] + tmp[11]*src[7];
	_matrix.array2d[0][3] = tmp[5]*src[4] + tmp[8]*src[5] + tmp[11]*src[6];
	_matrix.array2d[0][3]-= tmp[4]*src[4] + tmp[9]*src[5] + tmp[10]*src[6];
	_matrix.array2d[1][0] = tmp[1]*src[1] + tmp[2]*src[2] + tmp[5]*src[3];
	_matrix.array2d[1][0]-= tmp[0]*src[1] + tmp[3]*src[2] + tmp[4]*src[3];
	_matrix.array2d[1][1] = tmp[0]*src[0] + tmp[7]*src[2] + tmp[8]*src[3];
	_matrix.array2d[1][1]-= tmp[1]*src[0] + tmp[6]*src[2] + tmp[9]*src[3];
	_matrix.array2d[1][2] = tmp[3]*src[0] + tmp[6]*src[1] + tmp[11]*src[3];
	_matrix.array2d[1][2]-= tmp[2]*src[0] + tmp[7]*src[1] + tmp[10]*src[3];
	_matrix.array2d[1][3] = tmp[4]*src[0] + tmp[9]*src[1] + tmp[10]*src[2];
	_matrix.array2d[1][3]-= tmp[5]*src[0] + tmp[8]*src[1] + tmp[11]*src[2];

	tmp[0] = src[2]*src[7];
	tmp[1] = src[3]*src[6];
	tmp[2] = src[1]*src[7];
	tmp[3] = src[3]*src[5];
	tmp[4] = src[1]*src[6];
	tmp[5] = src[2]*src[5];
	tmp[6] = src[0]*src[7];
	tmp[7] = src[3]*src[4];
	tmp[8] = src[0]*src[6];
	tmp[9] = src[2]*src[4];
	tmp[10] = src[0]*src[5];
	tmp[11] = src[1]*src[4];

	_matrix.array2d[2][0] = tmp[0]*src[13] + tmp[3]*src[14] + tmp[4]*src[15];
	_matrix.array2d[2][0]-= tmp[1]*src[13] + tmp[2]*src[14] + tmp[5]*src[15];
	_matrix.array2d[2][1] = tmp[1]*src[12] + tmp[6]*src[14] + tmp[9]*src[15];
	_matrix.array2d[2][1]-= tmp[0]*src[12] + tmp[7]*src[14] + tmp[8]*src[15];
	_matrix.array2d[2][2] = tmp[2]*src[12] + tmp[7]*src[13] + tmp[10]*src[15];
	_matrix.array2d[2][2]-= tmp[3]*src[12] + tmp[6]*src[13] + tmp[11]*src[15];
	_matrix.array2d[2][3] = tmp[5]*src[12] + tmp[8]*src[13] + tmp[11]*src[14];
	_matrix.array2d[2][3]-= tmp[4]*src[12] + tmp[9]*src[13] + tmp[10]*src[14];
	_matrix.array2d[3][0] = tmp[2]*src[10] + tmp[5]*src[11] + tmp[1]*src[9];
	_matrix.array2d[3][0]-= tmp[4]*src[11] + tmp[0]*src[9] + tmp[3]*src[10];
	_matrix.array2d[3][1] = tmp[8]*src[11] + tmp[0]*src[8] + tmp[7]*src[10];
	_matrix.array2d[3][1]-= tmp[6]*src[10] + tmp[9]*src[11] + tmp[1]*src[8];
	_matrix.array2d[3][2] = tmp[6]*src[9] + tmp[11]*src[11] + tmp[3]*src[8];
	_matrix.array2d[3][2]-= tmp[10]*src[11] + tmp[2]*src[8] + tmp[7]*src[9];
	_matrix.array2d[3][3] = tmp[10]*src[10] + tmp[4]*src[8] + tmp[9]*src[9];
	_matrix.array2d[3][3]-= tmp[8]*src[9] + tmp[11]*src[10] + tmp[5]*src[8];

	det = src[0]*_matrix.array2d[0][0] + src[1]*_matrix.array2d[0][1] + src[2]*_matrix.array2d[0][2] + src[3]*_matrix.array2d[0][3];
	det = 1.0f / det;

	for (i = 0; i < 16; i++)
	{
		_matrix.array1d[i] *= det;
	}

	(*this) = _matrix;
}

Matrix4 &Matrix4::operator *= (const Matrix4& _matrix) {
	*this = (*this)*_matrix;
	return *this;
}

Matrix4 Matrix4::operator * (const Matrix4 &_matrix) const {
	Matrix4 result;

	result.array2d[0][0]  = array2d[0][0]*_matrix.array2d[0][0] + array2d[0][1]*_matrix.array2d[1][0] + array2d[0][2]*_matrix.array2d[2][0]  + array2d[0][3]*_matrix.array2d[3][0];
	result.array2d[0][1]  = array2d[0][0]*_matrix.array2d[0][1] + array2d[0][1]*_matrix.array2d[1][1] + array2d[0][2]*_matrix.array2d[2][1]  + array2d[0][3]*_matrix.array2d[3][1];
	result.array2d[0][2]  = array2d[0][0]*_matrix.array2d[0][2] + array2d[0][1]*_matrix.array2d[1][2] + array2d[0][2]*_matrix.array2d[2][2] + array2d[0][3]*_matrix.array2d[3][2];
	result.array2d[0][3]  = array2d[0][0]*_matrix.array2d[0][3] + array2d[0][1]*_matrix.array2d[1][3] + array2d[0][2]*_matrix.array2d[2][3] + array2d[0][3]*_matrix.array2d[3][3];

	result.array2d[1][0]  = array2d[1][0]*_matrix.array2d[0][0] + array2d[1][1]*_matrix.array2d[1][0] + array2d[1][2]*_matrix.array2d[2][0]  + array2d[1][3]*_matrix.array2d[3][0];
	result.array2d[1][1]  = array2d[1][0]*_matrix.array2d[0][1] + array2d[1][1]*_matrix.array2d[1][1] + array2d[1][2]*_matrix.array2d[2][1]  + array2d[1][3]*_matrix.array2d[3][1];
	result.array2d[1][2]  = array2d[1][0]*_matrix.array2d[0][2] + array2d[1][1]*_matrix.array2d[1][2] + array2d[1][2]*_matrix.array2d[2][2] + array2d[1][3]*_matrix.array2d[3][2];
	result.array2d[1][3]  = array2d[1][0]*_matrix.array2d[0][3] + array2d[1][1]*_matrix.array2d[1][3] + array2d[1][2]*_matrix.array2d[2][3] + array2d[1][3]*_matrix.array2d[3][3];

	result.array2d[2][0]  = array2d[2][0]*_matrix.array2d[0][0] + array2d[2][1]*_matrix.array2d[1][0] + array2d[2][2]*_matrix.array2d[2][0]  + array2d[2][3]*_matrix.array2d[3][0];
	result.array2d[2][1]  = array2d[2][0]*_matrix.array2d[0][1] + array2d[2][1]*_matrix.array2d[1][1] + array2d[2][2]*_matrix.array2d[2][1]  + array2d[2][3]*_matrix.array2d[3][1];
	result.array2d[2][2] = array2d[2][0]*_matrix.array2d[0][2] + array2d[2][1]*_matrix.array2d[1][2] + array2d[2][2]*_matrix.array2d[2][2] + array2d[2][3]*_matrix.array2d[3][2];
	result.array2d[2][3] = array2d[2][0]*_matrix.array2d[0][3] + array2d[2][1]*_matrix.array2d[1][3] + array2d[2][2]*_matrix.array2d[2][3] + array2d[2][3]*_matrix.array2d[3][3];

	result.array2d[3][0] = array2d[3][0]*_matrix.array2d[0][0] + array2d[3][1]*_matrix.array2d[1][0] + array2d[3][2]*_matrix.array2d[2][0]  + array2d[3][3]*_matrix.array2d[3][0];
	result.array2d[3][1] = array2d[3][0]*_matrix.array2d[0][1] + array2d[3][1]*_matrix.array2d[1][1] + array2d[3][2]*_matrix.array2d[2][1]  + array2d[3][3]*_matrix.array2d[3][1];
	result.array2d[3][2] = array2d[3][0]*_matrix.array2d[0][2] + array2d[3][1]*_matrix.array2d[1][2] + array2d[3][2]*_matrix.array2d[2][2] + array2d[3][3]*_matrix.array2d[3][2];
	result.array2d[3][3] = array2d[3][0]*_matrix.array2d[0][3] + array2d[3][1]*_matrix.array2d[1][3] + array2d[3][2]*_matrix.array2d[2][3] + array2d[3][3]*_matrix.array2d[3][3];

	return result;
}

Matrix4 Matrix4::operator * (const f32 &_factor) const {
	Matrix4 result = Zero;

	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			for (int k = 0; k < 4; k++)
				result.array2d[i][j] += array2d[k][j]*_factor;

	return result;
}

Vector3 Matrix4::operator * (const Vector3 &_vector) const {
	return Vector3(
        (array2d[0][0]*_vector.x) + (array2d[0][1]*_vector.y) + (array2d[0][2] *_vector.z) + array2d[0][3],
		(array2d[1][0]*_vector.x) + (array2d[1][1]*_vector.y) + (array2d[1][2] *_vector.z) + array2d[1][3],
		(array2d[2][0]*_vector.x) + (array2d[2][1]*_vector.y) + (array2d[2][2]*_vector.z) + array2d[2][3]
    );
}

Vector4 Matrix4::operator * (const Vector4 &_vector) const {
	return Vector4(
        array2d[0][0]*_vector.x + array2d[0][1]*_vector.y + array2d[0][2]*_vector.z + array2d[0][3]*_vector.w,
		array2d[1][0]*_vector.x + array2d[1][1]*_vector.y + array2d[1][2]*_vector.z + array2d[1][3]*_vector.w,
		array2d[2][0]*_vector.x + array2d[2][1]*_vector.y + array2d[2][2]*_vector.z + array2d[2][3]*_vector.w,
		array2d[3][0]*_vector.x + array2d[3][1]*_vector.y + array2d[3][2]*_vector.z + array2d[3][3]*_vector.w
    ); 
}

Matrix4& Matrix4::operator = (const Matrix4& m) {
    memcpy(array1d, m.array1d, sizeof(array1d));
    return *this;
}

/*
void Matrix4::MatrixSwap(const int &_a, const int &_b) {
	MathLib::Swap(&array2d[_a][_b], &array1d[_b][_a]);
}*/

}
}
