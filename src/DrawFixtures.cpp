#include "DrawFixtures.h"
#include "globals.h"

#include <SDL_pixels.h>
#include <SDL_rect.h>
#include <SDL_render.h>

#include <SDL2_gfxPrimitives.h>

#include <box2d/b2_body.h>
#include <box2d/b2_fixture.h>
#include <box2d/b2_circle_shape.h>
#include <box2d/b2_edge_shape.h>
#include <box2d/b2_polygon_shape.h>

void DrawBody(const Point &offset, b2Body *body, const SDL_Color &color)
{
    DrawFixtures(offset, body->GetFixtureList(), color);
}

void DrawFixtures(const Point &offset, b2Fixture *fixture, const SDL_Color &color)
{
    for (; fixture; fixture = fixture->GetNext())
    {
        DrawFixture(offset, fixture, color);
    }
}

void DrawFixture(const Point &offset, b2Fixture *fixture, const SDL_Color &color)
{
    const b2Vec2 &bodyPos = fixture->GetBody()->GetPosition();
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    const b2Shape * const shape = fixture->GetShape();
    const b2Shape::Type shape_type = shape->GetType();
    if (shape_type == b2Shape::e_circle)
    {
        const b2CircleShape * const circle_shape = reinterpret_cast<const b2CircleShape*>(shape);
        filledCircleRGBA(renderer, (bodyPos.x + circle_shape->m_p.x)*Physics::PIXELS_PER_METER - offset.x, (bodyPos.y + circle_shape->m_p.y)*Physics::PIXELS_PER_METER - offset.y, circle_shape->m_radius*Physics::PIXELS_PER_METER, color.r, color.g, color.b, color.a);
        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND); // HACK, the above function messes with this renderer state :-(
    }
    else if (shape_type == b2Shape::e_edge)
    {
        const b2EdgeShape * const edge_shape = reinterpret_cast<const b2EdgeShape*>(shape);
        const SDL_Point pt1{static_cast<int>((bodyPos.x + edge_shape->m_vertex1.x)*Physics::PIXELS_PER_METER) - offset.x, static_cast<int>((bodyPos.y + edge_shape->m_vertex1.y)*Physics::PIXELS_PER_METER) - offset.y};
        const SDL_Point pt2{static_cast<int>((bodyPos.x + edge_shape->m_vertex2.x)*Physics::PIXELS_PER_METER) - offset.x, static_cast<int>((bodyPos.y + edge_shape->m_vertex2.y)*Physics::PIXELS_PER_METER) - offset.y};
        SDL_RenderDrawLine(renderer, pt1.x, pt1.y, pt2.x, pt2.y);
    }
    else if (shape_type == b2Shape::e_polygon)
    {
        const b2PolygonShape * const polygon_shape = reinterpret_cast<const b2PolygonShape*>(shape);
        if (polygon_shape->m_count == 0)
            return;
        std::vector<SDL_Point> points;
        for (int32 i = 0; i < polygon_shape->m_count; i++)
        {
            const b2Vec2 * const p = polygon_shape->m_vertices + i;
            points.emplace_back(SDL_Point{static_cast<int>((bodyPos.x + p->x)*Physics::PIXELS_PER_METER) - offset.x, static_cast<int>((bodyPos.y + p->y)*Physics::PIXELS_PER_METER) - offset.y});
        }
        {
            const b2Vec2 * const p = polygon_shape->m_vertices;
            points.emplace_back(SDL_Point{static_cast<int>((bodyPos.x + p->x)*Physics::PIXELS_PER_METER) - offset.x, static_cast<int>((bodyPos.y + p->y)*Physics::PIXELS_PER_METER) - offset.y});
        }
        SDL_RenderDrawLines(renderer, points.data(), points.size());
    }
}
