#include "Point.h"

#include <iostream>

std::ostream & operator<<(std::ostream &stream, const Point &pt)
{
    stream << "Point(" << pt.x << ", " << pt.y << ")";
    return stream;
}

std::ostream & operator<<(std::ostream &stream, const PointF &pt)
{
    stream << "Point(" << pt.x << ", " << pt.y << ")";
    return stream;
}
