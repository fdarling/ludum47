#include "Line.h"

#include <iostream>

std::ostream & operator<<(std::ostream &stream, const Line &line)
{
    stream << "Line(" << line.p1 << ", " << line.p2 << ")";
    return stream;
}

std::ostream & operator<<(std::ostream &stream, const LineF &line)
{
    stream << "Line(" << line.p1 << ", " << line.p2 << ")";
    return stream;
}
