#pragma once

#include <cmath>
#include <iosfwd>

template <typename Real>
class PointBase
{
public:
    PointBase() : x(0), y(0)
    {
    }
    PointBase(Real xx, Real yy) : x(xx), y(yy)
    {
    }
    template <typename T>
    PointBase(const PointBase<T> &pt) : x(pt.x), y(pt.y)
    {
    }
    PointBase<Real> operator-() const
    {
        return PointBase<Real>(-x, -y);
    }
    PointBase<Real> operator+(const PointBase<Real> &other) const
    {
        return PointBase<Real>(x + other.x, y + other.y);
    }
    PointBase<Real> operator-(const PointBase<Real> &other) const
    {
        return PointBase<Real>(x - other.x, y - other.y);
    }
    PointBase<Real> operator*(const Real scalar) const
    {
        return PointBase<Real>(x * scalar, y * scalar);
    }
    PointBase<Real> operator/(Real scalar) const
    {
        return PointBase<Real>(x / scalar, y / scalar);
    }
    Real length() const
    {
        return std::sqrt(x*x + y*y);
    }
    Real dot(const PointBase<Real> &pt) const
    {
        return x*pt.x + y*pt.y;
    }
    PointBase<Real> normalized() const
    {
        const Real len = length();
        if (len == static_cast<Real>(0))
        {
            return PointBase<Real>();
        }
        return PointBase<Real>(x / len, y / len);
    }
    Real x;
    Real y;
};

typedef PointBase<int> Point;
typedef PointBase<double> PointF;

std::ostream & operator<<(std::ostream &stream, const Point &pt);
std::ostream & operator<<(std::ostream &stream, const PointF &pt);
