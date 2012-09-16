#include "base/math/matrix.h"

#include <string.h>

namespace base
{
namespace math
{

const Matrix4 Matrix4::LookAt( const Vector3& eyePos, const Vector3& lookAtPos, const Vector3& upVec )
{
    Matrix4 m4EyeFrame;
    Vector3 v3X, v3Y, v3Z;
    v3Y = Normalize( upVec );
    v3Z = Normalize( eyePos - lookAtPos );
    v3X = Normalize( Cross( v3Y, v3Z ) );
    v3Y = Cross( v3Z, v3X );
    m4EyeFrame = Matrix4(
                     Vector4( v3X ),
                     Vector4( v3Y ),
                     Vector4( v3Z ),
                     Vector4( eyePos )
                 );
    return OrthoInverse( m4EyeFrame );
}

const Matrix4 Matrix4::Perspective( f32 fovyRadians, f32 aspect, f32 zNear, f32 zFar )
{
    f32 f = tanf( ( ( f32 )( pi_over_2 ) - ( 0.5f * fovyRadians ) ) );
    f32 rangeInv = ( 1.0f / ( zNear - zFar ) );
    return Matrix4(
               Vector4( ( f / aspect ), 0.0f, 0.0f, 0.0f ),
               Vector4( 0.0f, f, 0.0f, 0.0f ),
               Vector4( 0.0f, 0.0f, ( ( zNear + zFar ) * rangeInv ), -1.0f ),
               Vector4( 0.0f, 0.0f, ( ( ( zNear * zFar ) * rangeInv ) * 2.0f ), 0.0f )
           );
}
const Matrix4 Matrix4::Frustum( f32 left, f32 right, f32 bottom, f32 top, f32 zNear, f32 zFar )
{
    f32 sum_rl = ( right + left );
    f32 sum_tb = ( top + bottom );
    f32 sum_nf = ( zNear + zFar );
    f32 inv_rl = ( 1.0f / ( right - left ) );
    f32 inv_tb = ( 1.0f / ( top - bottom ) );
    f32 inv_nf = ( 1.0f / ( zNear - zFar ) );
    f32 n2 = ( zNear + zNear );
    return Matrix4(
               Vector4( ( n2 * inv_rl ), 0.0f, 0.0f, 0.0f ),
               Vector4( 0.0f, ( n2 * inv_tb ), 0.0f, 0.0f ),
               Vector4( ( sum_rl * inv_rl ), ( sum_tb * inv_tb ), ( sum_nf * inv_nf ), -1.0f ),
               Vector4( 0.0f, 0.0f, ( ( n2 * inv_nf ) * zFar ), 0.0f )
           );
}

const Matrix4 Matrix4::Orthographic( f32 left, f32 right, f32 bottom, f32 top, f32 zNear, f32 zFar )
{
    f32 sum_rl = ( right + left );
    f32 sum_tb = ( top + bottom );
    f32 sum_nf = ( zNear + zFar );
    f32 inv_rl = ( 1.0f / ( right - left ) );
    f32 inv_tb = ( 1.0f / ( top - bottom ) );
    f32 inv_nf = ( 1.0f / ( zNear - zFar ) );
    return Matrix4(
               Vector4( ( inv_rl + inv_rl ), 0.0f, 0.0f, 0.0f ),
               Vector4( 0.0f, ( inv_tb + inv_tb ), 0.0f, 0.0f ),
               Vector4( 0.0f, 0.0f, ( inv_nf + inv_nf ), 0.0f ),
               Vector4( ( -sum_rl * inv_rl ), ( -sum_tb * inv_tb ), ( sum_nf * inv_nf ), 1.0f )
           );
}

const Matrix4 Inverse( const Matrix4& m )
{
    Vector4 res0, res1, res2, res3;
    f32 mA = m.Col0().x;
    f32 mB = m.Col0().y;
    f32 mC = m.Col0().z;
    f32 mD = m.Col0().w;
    f32 mE = m.Col1().x;
    f32 mF = m.Col1().y;
    f32 mG = m.Col1().z;
    f32 mH = m.Col1().w;
    f32 mI = m.Col2().x;
    f32 mJ = m.Col2().y;
    f32 mK = m.Col2().z;
    f32 mL = m.Col2().w;
    f32 mM = m.Col3().x;
    f32 mN = m.Col3().y;
    f32 mO = m.Col3().z;
    f32 mP = m.Col3().w;
    f32 tmp0 = ( ( mK * mD ) - ( mC * mL ) );
    f32 tmp1 = ( ( mO * mH ) - ( mG * mP ) );
    f32 tmp2 = ( ( mB * mK ) - ( mJ * mC ) );
    f32 tmp3 = ( ( mF * mO ) - ( mN * mG ) );
    f32 tmp4 = ( ( mJ * mD ) - ( mB * mL ) );
    f32 tmp5 = ( ( mN * mH ) - ( mF * mP ) );
    res0 = Vector4(
               ( ( ( mJ * tmp1 ) - ( mL * tmp3 ) ) - ( mK * tmp5 ) ),
               ( ( ( mN * tmp0 ) - ( mP * tmp2 ) ) - ( mO * tmp4 ) ),
               ( ( ( mD * tmp3 ) + ( mC * tmp5 ) ) - ( mB * tmp1 ) ),
               ( ( ( mH * tmp2 ) + ( mG * tmp4 ) ) - ( mF * tmp0 ) )
           );
    f32 detInv = ( 1.0f / ( ( ( ( mA * res0.x ) + ( mE * res0.y ) ) + ( mI * res0.z ) ) + ( mM * res0.w ) ) );
    res1 = Vector4(
               ( mI * tmp1 ),
               ( mM * tmp0 ),
               ( mA * tmp1 ),
               ( mE * tmp0 )
           );
    res3 = Vector4(
               ( mI * tmp3 ),
               ( mM * tmp2 ),
               ( mA * tmp3 ),
               ( mE * tmp2 )
           );
    res2 = Vector4(
               ( mI * tmp5 ),
               ( mM * tmp4 ),
               ( mA * tmp5 ),
               ( mE * tmp4 )
           );
    tmp0 = ( ( mI * mB ) - ( mA * mJ ) );
    tmp1 = ( ( mM * mF ) - ( mE * mN ) );
    tmp2 = ( ( mI * mD ) - ( mA * mL ) );
    tmp3 = ( ( mM * mH ) - ( mE * mP ) );
    tmp4 = ( ( mI * mC ) - ( mA * mK ) );
    tmp5 = ( ( mM * mG ) - ( mE * mO ) );
    res2.x = ( ( ( ( mL * tmp1 ) - ( mJ * tmp3 ) ) + res2.x ) );
    res2.y = ( ( ( ( mP * tmp0 ) - ( mN * tmp2 ) ) + res2.y ) );
    res2.z = ( ( ( ( mB * tmp3 ) - ( mD * tmp1 ) ) - res2.z ) );
    res2.w = ( ( ( ( mF * tmp2 ) - ( mH * tmp0 ) ) - res2.w ) );
    res3.x = ( ( ( ( mJ * tmp5 ) - ( mK * tmp1 ) ) + res3.x ) );
    res3.y = ( ( ( ( mN * tmp4 ) - ( mO * tmp0 ) ) + res3.y ) );
    res3.z = ( ( ( ( mC * tmp1 ) - ( mB * tmp5 ) ) - res3.z ) );
    res3.w = ( ( ( ( mG * tmp0 ) - ( mF * tmp4 ) ) - res3.w ) );
    res1.x = ( ( ( ( mK * tmp3 ) - ( mL * tmp5 ) ) - res1.x ) );
    res1.y = ( ( ( ( mO * tmp2 ) - ( mP * tmp4 ) ) - res1.y ) );
    res1.z = ( ( ( ( mD * tmp5 ) - ( mC * tmp3 ) ) + res1.z ) );
    res1.w = ( ( ( ( mH * tmp4 ) - ( mG * tmp2 ) ) + res1.w ) );
    return Matrix4(
               ( res0 * detInv ),
               ( res1 * detInv ),
               ( res2 * detInv ),
               ( res3 * detInv )
           );
}

const Matrix4 AffineInverse( const Matrix4& m )
{
    Vector3 tmp0 = Cross( m.Col1().xyz(), m.Col2().xyz() );
    Vector3 tmp1 = Cross( m.Col2().xyz(), m.Col0().xyz() );
    Vector3 tmp2 = Cross( m.Col0().xyz(), m.Col1().xyz() );
    f32 detinv = ( 1.0f / Dot( m.Col2().xyz(), tmp2 ) );
    Vector3 inv0 = Vector3( tmp0.x, tmp1.x, tmp2.x ) * detinv;
    Vector3 inv1 = Vector3( tmp0.y, tmp1.y, tmp2.y ) * detinv;
    Vector3 inv2 = Vector3( tmp0.z, tmp1.z, tmp2.z ) * detinv;
    Vector3 tr = Vector3( m.Col3() );
    Vector3 v = -( ( inv0 * tr.x ) + ( inv1 * tr.y ) + ( inv2 * tr.z ) );
    return Matrix4(
               Vector4( inv0, 0.0f ),
               Vector4( inv1, 0.0f ),
               Vector4( inv2, 0.0f ),
               Vector4( v, 1.0f )
           );
}

const Matrix4 OrthoInverse( const Matrix4& m )
{
    Vector3 inv0 = Vector3( m.Row( 0 ) );
    Vector3 inv1 = Vector3( m.Row( 1 ) );
    Vector3 inv2 = Vector3( m.Row( 2 ) );
    Vector3 tr = Vector3( m.Col( 3 ) );
    Vector3 v = -( ( inv0 * tr.x ) + ( ( inv1 * tr.y ) + ( inv2 * tr.z ) ) );
    return Matrix4(
               Vector4( inv0, 0.0f ),
               Vector4( inv1, 0.0f ),
               Vector4( inv2, 0.0f ),
               Vector4( v, 1.0f )
           );
}

f32 Determinant( const Matrix4& m )
{
    Vector4 res0, res1, res2, res3;
    f32 mA = m.Col0().x;
    f32 mB = m.Col0().y;
    f32 mC = m.Col0().z;
    f32 mD = m.Col0().w;
    f32 mE = m.Col1().x;
    f32 mF = m.Col1().y;
    f32 mG = m.Col1().z;
    f32 mH = m.Col1().w;
    f32 mI = m.Col2().x;
    f32 mJ = m.Col2().y;
    f32 mK = m.Col2().z;
    f32 mL = m.Col2().w;
    f32 mM = m.Col3().x;
    f32 mN = m.Col3().y;
    f32 mO = m.Col3().z;
    f32 mP = m.Col3().w;
    f32 tmp0 = ( ( mK * mD ) - ( mC * mL ) );
    f32 tmp1 = ( ( mO * mH ) - ( mG * mP ) );
    f32 tmp2 = ( ( mB * mK ) - ( mJ * mC ) );
    f32 tmp3 = ( ( mF * mO ) - ( mN * mG ) );
    f32 tmp4 = ( ( mJ * mD ) - ( mB * mL ) );
    f32 tmp5 = ( ( mN * mH ) - ( mF * mP ) );
    res0 = Vector4(
               ( ( ( mJ * tmp1 ) - ( mL * tmp3 ) ) - ( mK * tmp5 ) ),
               ( ( ( mN * tmp0 ) - ( mP * tmp2 ) ) - ( mO * tmp4 ) ),
               ( ( ( mD * tmp3 ) + ( mC * tmp5 ) ) - ( mB * tmp1 ) ),
               ( ( ( mH * tmp2 ) + ( mG * tmp4 ) ) - ( mF * tmp0 ) )
           );
    return ( ( ( ( mA * res0.x ) + ( mE * res0.y ) ) + ( mI * res0.z ) ) + ( mM * res0.w ) );
}

}
}
