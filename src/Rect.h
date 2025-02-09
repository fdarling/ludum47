#pragma once

#include "Point.h"

#include <SDL2/SDL.h>

template<typename Real>
class RectBase
{
public:
    RectBase()
    {
    }
    RectBase(Real xx, Real yy, Real ww, Real hh) : x(xx), y(yy), w(ww), h(hh)
    {
    }
    RectBase scaled(const Real &scalar) const
    {
        return RectBase<Real>(x*scalar, y*scalar, w*scalar, h*scalar);
    }
    RectBase translated(const PointBase<Real> &pt) const
    {
        return RectBase<Real>(x + pt.x, y + pt.y, w, h);
    }
    SDL_Rect toSDL() const { return SDL_Rect{x, y, w, h}; };
    Real left() const { return x; }
    void setLeft(Real c) { x = c; }
    Real right() const { return x + w; }
    void setRight(Real c) { x = c - w; }
    Real top() const { return y; }
    void setTop(Real c) { y = c; }
    Real bottom() const { return y + h; }
    void setBottom(Real c) { y = c - h; }
    Real centerX() const { return x + w/2; }
    void setCenterX(Real c) { x = c - w/2; }
    Real centerY() const { return y + h/2; }
    void setCenterY(Real c) { y = c - h/2; }
    PointBase<Real> topLeft() const { return PointBase<Real>(left(), top()); }
    PointBase<Real> midTop() const { return PointBase<Real>(centerX(), top()); }
    PointBase<Real> topRight() const { return PointBase<Real>(right(), top()); }
    PointBase<Real> midLeft() const { return PointBase<Real>(left(), centerY()); }
    PointBase<Real> center() const { return PointBase<Real>(centerX(), centerY()); }
    PointBase<Real> midRight() const { return PointBase<Real>(right(), centerY()); }
    PointBase<Real> bottomLeft() const { return PointBase<Real>(left(), bottom()); }
    PointBase<Real> midBottom() const { return PointBase<Real>(centerX(), bottom()); }
    PointBase<Real> bottomRight() const { return PointBase<Real>(right(), bottom()); }
    Real x;
    Real y;
    Real w;
    Real h;
};

typedef RectBase<int> Rect;
typedef RectBase<double> RectF;
