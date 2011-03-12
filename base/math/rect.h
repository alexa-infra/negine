#pragma once

#include "base/types.h"

namespace base {
namespace math {

struct Point {
    i32 x;
    i32 y;

    void set(i32 _x, i32 _y) {
        x = _x; y = _y;
    }
    bool operator== (const Point& p) const {
        return x == p.x && y == p.y;
    }
    bool operator!= (const Point& p) const {
        return x != p.x || y == p.y;
    }
    static Point init(i32 x, i32 y) {
        Point p;
        p.x = x; p.y = y;
        return p;
    }
};

struct Size {
    i32 Width;
    i32 Height;
    
    void set(i32 w, i32 h) {
        Width = w; Height = h;
    }
    void set(i32 hw) {
        Width = hw; Height = hw;
    }
    bool operator== (const Size& s) const {
        return Width == s.Width && Height == s.Height;
    }
    bool operator!= (const Size& s) const {
        return Width != s.Width || Height != s.Height;
    }
};

class Rect {
public:
    union {
        struct {
            i32 x;
            i32 y;
            i32 Width;
            i32 Height;
        };
        struct {
            Point Position;
            Size Dimensions;
        };
    };
    Rect() : x(0), y(0), Width(0), Height(0) {}
    Rect(i32 _x, i32 _y, i32 _w, i32 _h) 
        : x(_x), y(_y), Width(_w), Height(_h) {}
    Rect(const Point& p, const Size& s) {
        Position = p;
        Dimensions = s;
    }
    i32 GetArea() const { return Width * Height; }

    i32 Bottom() const { return y + Height; }
    i32 Right() const { return x + Width; }
    i32 Left() const { return x; }
    i32 Top() const { return y; }

    void SetBottom(i32 bottom) { 
        Width = bottom - y; 
    }
    void SetRight(i32 right) {
        Height = right - x;
    }

    Point LeftTopCorner() const {
        return Point::init(Left(), Top());
    }
    Point RightTopCorner() const {
        return Point::init(Right(), Top());
    }
    Point LeftBottomCorner() const {
        return Point::init(Left(), Bottom());
    }
    Point RightBottomCorner() const {
        return Point::init(Right(), Bottom());
    }
    Point Center() const {
        return Point::init(x + Width/2, y + Height/2);
    }
    Point RightCenter() const {
        return Point::init(Right(), y + Height/2);
    }
    Point LeftCenter() const {
        return Point::init(Left(), y + Height/2);
    }
    Point TopCenter() const {
        return Point::init(x + Width/2, Top());
    }
    Point BottomCenter() const {
        return Point::init(x + Width/2, Bottom());
    }

    void Scale(f32 k) {
        Width = Width * k;
        Height = Height * k;
    }
    void Scale(f32 kw, f32 kh) {
        Width = Width * kw;
        Height = Height * kh;
    }

    bool Contains(const Point& p) const {
        return (p.x >= x && p.y >= y && p.x < Right() && p.y < Bottom());
    }
    bool Contains(const i32& x, const i32& y) const {
        return Contains(Point::init(x, y));
    }
    bool Contains(const Rect& rect) const {
        return rect.x >= x && rect.y >= y && rect.Right() < Right() && rect.Bottom() < Bottom();
    }
    bool Intersec(const Rect& rect) const {
        if (Contains(rect))
            return true;
        if (rect.Contains(*this))
            return true;

        if (Contains(rect.LeftTopCorner())) 
            return true;
        if (Contains(rect.RightTopCorner())) 
            return true;
        if (Contains(rect.LeftBottomCorner())) 
            return true;
        if (Contains(rect.RightBottomCorner())) 
            return true;
        
        return false;
    }
    Rect Union(const Rect& rect) const
    {
        Rect ret;
        ret.x = (x > rect.x) ? x : rect.x;
        ret.y = (y > rect.y) ? y : rect.y;
        ret.SetRight((Right() < rect.Right()) ? Right() : rect.Right());
        ret.SetBottom((Bottom() < rect.Bottom()) ? Bottom() : rect.Bottom());
        return ret;
    }
    Rect Combine(const Rect& rect) const
    {
        Rect ret;
        ret.x = (x < rect.x) ? x : rect.x;
        ret.y = (y < rect.y) ? y : rect.y;
        ret.SetRight((Right() > rect.Right()) ? Right() : rect.Right());
        ret.SetBottom((Bottom() > rect.Bottom()) ? Bottom() : rect.Bottom());
        return ret;
    }
};

}
}
