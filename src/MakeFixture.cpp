#include "MakeFixture.h"
#include "globals.h"

#include <box2d/b2_polygon_shape.h>

void MakeFixture(b2Body *body, std::vector<b2Vec2> points)
{
    for (std::size_t i = 0; i < points.size(); i++)
    {
        points[i].x *= Physics::METERS_PER_PIXEL;
        points[i].y *= Physics::METERS_PER_PIXEL;
    }
    b2PolygonShape shape;
    shape.Set(points.data(), points.size());
    body->CreateFixture(&shape, 0.0);
}
