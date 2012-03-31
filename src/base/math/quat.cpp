/**
 * \file
 * \author      Alexey Vasilyev <alexa.infra@gmail.com>
 * \copyright   MIT License
 **/
#include "base/math/quat.h"
#include <assert.h>

namespace base {
namespace math {

Quat::Quat()
{
}

Quat::Quat(const f32 *_q)
{
    Set(_q);
}

Quat::Quat(f32 _x, f32 _y, f32 _z, f32 _w)
{
    Set(_x, _y, _z, _w);
}

Quat::Quat(const Vector3& axis, f32 angle)
{
    Set(axis, angle);
}

Quat::Quat(const Matrix4& _matrix)
{
    Set(_matrix);
}

void Quat::Set(const f32 *_q)
{
    x = _q[0];
    y = _q[1];
    z = _q[2];
    w = _q[3];
}

void Quat::Set(f32 _x, f32 _y, f32 _z, f32 _w)
{
    x = _x;
    y = _y;
    z = _z;
    w = _w;
}

void Quat::Set(const Vector3& axis, f32 angle)
{
    (*this) = GetRotation(axis, angle);
}

void Quat::Set(const Matrix4& _matrix)
{
}

Quat Quat::operator+ (const Quat& _q) const
{
    Quat q(*this);
    q.x+=_q.x;
    q.y+=_q.y;
    q.z+=_q.z;
    q.w+=_q.w;
    return q;
}

Quat Quat::operator* (const f32 _number)   const
{
    Quat q(*this);

    q.x*=_number;
    q.y*=_number;
    q.z*=_number;
    q.w*=_number;
    return q;
}

Quat Quat::operator/ (const f32 _number)   const
{
    Quat q(*this);

    q.x/=_number;
    q.y/=_number;
    q.z/=_number;
    q.w/=_number;
    return q;
}

Quat Quat::operator- (const Quat& _q) const
{
    Quat q(*this);
    q.x-=_q.x;
    q.y-=_q.y;
    q.z-=_q.z;
    q.w-=_q.w;
    return q;
}

Quat Quat::operator* (const Quat& _q) const
{
    Quat q(*this);

    f32 A, B, C, D, E, F, G, H;

    A = (_q.w + _q.x) * (w + x);
    B = (_q.z - _q.y) * (y - z);
    C = (_q.x - _q.w) * (y + z);
    D = (_q.y + _q.z) * (x - w);
    E = (_q.x + _q.z) * (x + y);
    F = (_q.x - _q.z) * (x - y);
    G = (_q.w + _q.y) * (w - z);
    H = (_q.w - _q.y) * (w + z);

    q.w = B + (-E - F + G + H) * 0.5f;
    q.x = A - ( E + F + G + H) * 0.5f; 
    q.y =-C + ( E - F + G - H) * 0.5f;
    q.z =-D + ( E - F - G + H) * 0.5f;

    return q;
};

Quat Quat::GetInversed()
{
    return GetConjugated()/GetNorm();
}

Quat Quat::GetConjugated()
{
    Quat q(*this);

    q.x = -x;
    q.y = -y;
    q.z = -z;
    q.w = w;
    return q;
}


f32 Quat::GetNorm() const 
{
    return x*x + y*y + z*z + w*w;
}

f32 Quat::GetLength() const
{
    return sqrt(GetNorm());
}

Quat Quat::GetZero()
{
    return Quat(0, 0, 0, 1);
}

Quat Quat::GetRotation(const Vector3& axis, f32 angle)
{
    Quat q = GetZero();
    f32 len = axis.Length();
    if (len)
    {   
        len = 1.0f/len;
        f32 sinangle = sin(angle/2.0f);
        q.x = axis.x*len*sinangle;
        q.y = axis.y*len*sinangle;
        q.z = axis.z*len*sinangle;
        q.w = cos(angle/2.0f);
    }
    else
    {
        q.x = q.y = q.z = 0.0f;
        q.w = 1.0f;
    };
    return q;
}

Quat Quat::GetRotationX(f32 angle)
{
    return Quat::GetRotation(Vector3(1, 0, 0), angle);
}

Quat Quat::GetRotationY(f32 angle)
{
    return Quat::GetRotation(Vector3(0, 1, 0), angle);
}

Quat Quat::GetRotationZ(f32 angle)
{
    return Quat::GetRotation(Vector3(0, 0, 1), angle);
}

void Quat::Rotate(const Vector3& axis, const f32& angle)
{
    *this *= GetRotation(axis, angle);
}

void Quat::RotateX(const f32& angle)
{
    *this *= GetRotationX(angle);
}

void Quat::RotateY(const f32& angle)
{
    *this *= GetRotationY(angle);
}

void Quat::RotateZ(const f32& angle)
{
    *this *= GetRotationZ(angle);
}


Quat Quat::GetSlerp(const Quat& q0, const Quat& q1, f32 _time)
{
    f32 k0,k1;
    f32 cos_omega = q0.x * q1.x + q0.y * q1.y + q0.z * q1.z + q0.w * q1.w;
    Quat q;

    if(cos_omega < 0.0) 
    {
        cos_omega = -cos_omega;
        q.x = -q1.x;
        q.y = -q1.y;
        q.z = -q1.z;
        q.w = -q1.w;
    } 
    else 
    {
        q.x = q1.x;
        q.y = q1.y;
        q.z = q1.z;
        q.w = q1.w;
    };

    if(1.0 - cos_omega > 1e-6) 
    {
        f32 omega = acos(cos_omega);
        f32 sin_omega = sin(omega);
        k0 = sin((1.0f - _time) * omega) / sin_omega;
        k1 = sin(_time * omega) / sin_omega;
    } 
    else 
    {
        k0 = 1.0f - _time;
        k1 = _time;
    };

    return Quat(
            q0.x * k0 + q.x * k1,
            q0.y * k0 + q.y * k1,
            q0.z * k0 + q.z * k1,
            q0.w * k0 + q.w * k1
                    );
}


Matrix4 Quat::GetMatrix() const
{
    Matrix4 m;
    m = Matrix4::Identity;

    f32 wx, wy, wz, xx, yy, yz, xy, xz, zz, x2, y2, z2;
    f32 s  = 2.0f/GetNorm();

    x2 = x * s;    y2 = y * s;    z2 = z * s;
    xx = x * x2;   xy = x * y2;   xz = x * z2;
    yy = y * y2;   yz = y * z2;   zz = z * z2;
    wx = w * x2;   wy = w * y2;   wz = w * z2;

    m.array1d[0+0*4] = 1.0f - (yy + zz);
    m.array1d[1+0*4] = xy - wz;
    m.array1d[2+0*4] = xz + wy;

    m.array1d[0+1*4] = xy + wz;
    m.array1d[1+1*4] = 1.0f - (xx + zz);
    m.array1d[2+1*4] = yz - wx;

    m.array1d[0+2*4] = xz - wy;
    m.array1d[1+2*4] = yz + wx;
    m.array1d[2+2*4] = 1.0f - (xx + yy);

    return m;
}

Quat& Quat::operator+= (const Quat& _q)
{
    *this = *this + _q;
    return *this;
}

Quat& Quat::operator-= (const Quat& _q)
{
    *this = *this - _q;
    return *this;
}

Quat& Quat::operator*= (const Quat& _q)
{
    *this = *this * _q;
    return *this;
}

Quat& Quat::operator*= (f32 _number)
{
    *this = *this * _number;
    return *this;
}

Quat& Quat::operator/= (f32 _number)
{
    *this = *this / _number;
    return *this;
}


} // namespace base
} // namespace math