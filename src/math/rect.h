/**
 * \file
 * \brief       Rectangle class for integer/float types
 * \author      Alexey Vasilyev <alexa.infra@gmail.com>
 * \copyright   MIT License
 **/
#pragma once

#include "base/types.h"
#include "math/mathlib.h"
#include "math/vec2.h"

namespace base
{
namespace math
{

class Rect
{
public:
    vec2i position;
    vec2i size;

    Rect() : position( 0 ), size( 0 )
    {
    }
    Rect( i32 x, i32 y, i32 w, i32 h )
        : position(x, y), size(w, h)
    {
    }
    Rect( const vec2i& p, const vec2i& s )
        : position(p), size(s)
    {
    }
    i32 GetArea() const {
        return size.x * size.y;
    }

    i32 Bottom() const {
        return position.y + size.y;
    }
    i32 Right() const {
        return position.x + size.x;
    }
    i32 Left() const {
        return position.x;
    }
    i32 Top() const {
        return position.y;
    }

    void SetBottom( i32 bottom ) {
        size.x = bottom - position.y;
    }
    void SetRight( i32 right ) {
        size.y = right - position.x;
    }

    vec2i LeftTopCorner() const {
        return vec2i( Left(), Top() );
    }
    vec2i RightTopCorner() const {
        return vec2i( Right(), Top() );
    }
    vec2i LeftBottomCorner() const {
        return vec2i( Left(), Bottom() );
    }
    vec2i RightBottomCorner() const {
        return vec2i( Right(), Bottom() );
    }
    vec2i Center() const {
        return vec2i( position.x + size.x / 2, position.y + size.y / 2 );
    }
    vec2i RightCenter() const {
        return vec2i( Right(), position.y + size.y / 2 );
    }
    vec2i LeftCenter() const {
        return vec2i( Left(), position.y + size.y / 2 );
    }
    vec2i TopCenter() const {
        return vec2i( position.x + size.x / 2, Top() );
    }
    vec2i BottomCenter() const {
        return vec2i( position.x + size.x / 2, Bottom() );
    }

    void Scale( f32 k ) {
        size *= k;
    }
    void Scale( f32 kw, f32 kh ) {
        size *= vec2i(kw, kh);
    }

    bool Contains( const vec2i& p ) const {
        return ( p.x >= Left()
            && p.y >= Top()
            && p.x < Right()
            && p.y < Bottom() );
    }
    bool Contains( const i32& x, const i32& y ) const {
        return Contains( vec2i( x, y ) );
    }
    bool Contains( const Rect& rect ) const {
        return rect.Left() >= Left()
            && rect.Top() >= Top()
            && rect.Right() < Right()
            && rect.Bottom() < Bottom();
    }
};


struct RectF {
    vec2f position;
    vec2f size;
    f32 angle;

    RectF()
        : size( 1.f )
        , angle( 0.f ) {
    }

    RectF( const vec2f& pos_, const vec2f& size_, f32 angle_ )
        : position( pos_ )
        , size( size_ )
        , angle( angle_ ) {
    }

    RectF( const vec2f& pos_, f32 size_ )
        : position( pos_ )
        , size( size_ )
        , angle( 0.f ) {
    }

    void Points( vec2f* v ) const {
        f32 c = cosf( angle );
        f32 s = sinf( angle );
        v[0] = position + rotate( point1(), c, s );
        v[1] = position + rotate( point2(), c, s );
        v[2] = position + rotate( point3(), c, s );
        v[3] = position + rotate( point4(), c, s );
    }

private:
    vec2f point1() const {
        return vec2f( -size.x, size.y );
    }
    vec2f point2() const {
        return vec2f( size.x, size.y );
    }
    vec2f point3() const {
        return vec2f( size.x, -size.y );
    }
    vec2f point4() const {
        return vec2f( -size.x, -size.y );
    }

    vec2f rotate( vec2f const& v, f32 c, f32 s ) const {
        return vec2f( c * v.x - s * v.y, s * v.x + c * v.y );
    }
};


}
}
