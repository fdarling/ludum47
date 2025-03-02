#include "MakeFixture.h"
#include "globals.h"

// #define USE_POLYGON_INSTEAD_OF_EDGES

#include <box2d/b2_body.h>
#ifdef USE_POLYGON_INSTEAD_OF_EDGES
#include <box2d/b2_polygon_shape.h>
#else // USE_POLYGON_INSTEAD_OF_EDGES
#include <box2d/b2_edge_shape.h>
#endif // USE_POLYGON_INSTEAD_OF_EDGES

void MakeFixture(b2Body *body, std::vector<b2Vec2> points)
{
    for (std::size_t i = 0; i < points.size(); i++)
    {
        points[i].x *= Physics::METERS_PER_PIXEL;
        points[i].y *= Physics::METERS_PER_PIXEL;
    }
#ifdef USE_POLYGON_INSTEAD_OF_EDGES
    b2PolygonShape shape;
    shape.Set(points.data(), points.size());
    body->CreateFixture(&shape, 0.0);
#else // USE_POLYGON_INSTEAD_OF_EDGES
    b2EdgeShape shape;
    const size_t len = points.size();
    for (std::size_t i = 0; i < len; i++)
    {
        const size_t idx0 = (i+len-1)%len;
        const size_t idx1 = (i+len  )%len;
        const size_t idx2 = (i+len+1)%len;
        const size_t idx3 = (i+len+2)%len;
        shape.SetOneSided(points[idx0], points[idx1], points[idx2], points[idx3]);
        body->CreateFixture(&shape, 0.0);
    }
#endif // USE_POLYGON_INSTEAD_OF_EDGES
}
