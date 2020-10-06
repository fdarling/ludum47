#pragma once

#include "Point.h"

#include <algorithm>
#include <iosfwd>
#include <iostream>

template <typename Real>
class LineBase
{
public:
    LineBase() = default;
    LineBase(const PointBase<Real> &a, const PointBase<Real> &b) : p1(a), p2(b) {}
    Real pointTest(const PointBase<Real> &p3) const
    {
        return (p2.x - p1.x) * (p3.y - p1.y) - (p2.y - p1.y) * (p3.x - p1.x);
    }
    PointBase<Real> intersect(const LineBase<Real> &l, bool *onLine = nullptr, bool *bounded = nullptr) const
    {
        const PointBase<Real> delta1 =   p2 -   p1;
        const PointBase<Real> delta2 = l.p2 - l.p1;
        const Real &dxa = delta1.x;
        const Real &dya = delta1.y;
        const Real &dxb = delta2.x;
        const Real &dyb = delta2.y;
        const Real &xia =   p1.x;
        const Real &yia =   p1.y;
        const Real &xib = l.p1.x;
        const Real &yib = l.p1.y;

        const int determinant = dya*dxb - dxa*dyb;
        if (determinant == 0)
        {
            // std::cerr << "lines " << *this << " and " << l << " are colinear!" << std::endl;
            if (onLine)
                *onLine = false;
            if (bounded)
                *bounded = false;
            return PointBase<Real>();
        }
        
        const int x_numerator = (dxa*dxb*(l.p1.y - p1.y) - dxa*dyb*l.p1.x + dxb*dya*p1.x);
        const int y_numerator = (dxb*dya*yib - dxa*dyb*yia - dya*dyb*xib + dya*dyb*xia);
        // const int y_numerator = (dxb*dya*yib - dxa*dyb*yia - dya*dyb*(xib + xia));
        const int x = x_numerator/determinant;
        const int y = y_numerator/determinant;
        // std::cout << "x = " << x << std::endl;
        // std::cout << "y = " << y << std::endl;
        if (x < std::min(p1.x, p2.x) || x > std::max(p1.x, p2.x) || x < std::min(l.p1.x, l.p1.x) || x > std::max(l.p1.x, l.p2.x)
         || y < std::min(p1.y, p2.y) || y > std::max(p1.y, p2.y) || y < std::min(l.p1.y, l.p2.y) || y > std::max(l.p1.y, l.p2.y))
        {
            /*std::cout << "not on line" << std::endl;
            std::cout << PointBase<Real>(x, y) << std::endl;
            std::cout << *this << std::endl;
            std::cout << l << std::endl;
            std::cout << (x < std::min(p1.x, p2.x)) << (x > std::max(p1.x, p2.x)) << (x < std::min(l.p1.x, l.p1.x)) << (x > std::max(l.p1.x, l.p2.x))
         << (y < std::min(p1.y, p2.y)) << (y > std::max(p1.y, p2.y)) << (y < std::min(l.p1.y, l.p2.y)) << (y > std::max(l.p1.y, l.p2.y)) << std::endl;*/
            if (onLine)
                *onLine = false;
            if (bounded)
                *bounded = true;
            return PointBase<Real>();
        }
        if (onLine)
            *onLine = true;
        if (bounded)
            *bounded = true;
        return PointBase<Real>(x, y);
    }
    PointBase<Real> p1;
    PointBase<Real> p2;
};

typedef LineBase<int> Line;
typedef LineBase<double> LineF;

std::ostream & operator<<(std::ostream &stream, const Line &line);
std::ostream & operator<<(std::ostream &stream, const LineF &line);
