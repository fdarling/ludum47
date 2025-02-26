#include "DrawPhysics.h"
#include "globals.h"

#include "SDL2_gfxPrimitives.h"

#include <vector>
// #include <iostream>

void DrawPhysics::DrawPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color)
{
    std::vector<SDL_Point> points;
    points.reserve(vertexCount);
    for (int i = 0; i < vertexCount; i++)
    {
        points.push_back(SDL_Point{
            static_cast<int>(vertices[i].x*Physics::PIXELS_PER_METER) - camera.pos.x,
            static_cast<int>(vertices[i].y*Physics::PIXELS_PER_METER) - camera.pos.y
        });
    }
    SDL_SetRenderDrawColor(renderer, color.r*255.0, color.g*255.0, color.b*255.0, color.a*255.0);
    SDL_RenderDrawLines(renderer, points.data(), vertexCount);
    // std::cout << "DrawPolygon" << std::endl;
}

void DrawPhysics::DrawSolidPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color)
{
    std::vector<Sint16> vx;
    std::vector<Sint16> vy;
    vx.reserve(vertexCount);
    vy.reserve(vertexCount);
    for (int i = 0; i < vertexCount; i++)
    {
        vx.push_back(static_cast<Sint16>(vertices[i].x*Physics::PIXELS_PER_METER - camera.pos.x));
        vy.push_back(static_cast<Sint16>(vertices[i].y*Physics::PIXELS_PER_METER - camera.pos.y));
    }
    // filledPolygonRGBA(renderer, vx.data(), vy.data(), vertexCount, color.r*255.0, color.g*255.0, color.b*255.0, color.a*255.0);
    filledPolygonRGBA(renderer, vx.data(), vy.data(), vertexCount, color.r*255.0, color.g*255.0, color.b*255.0, 128);
    // std::cout << "DrawSolidPolygon " << vertices[0].x << ", " << vertices[0].y << std::endl;
}

void DrawPhysics::DrawCircle(const b2Vec2& center, float radius, const b2Color& color)
{
    const int x = center.x*Physics::PIXELS_PER_METER - camera.pos.x;
    const int y = center.y*Physics::PIXELS_PER_METER - camera.pos.y;
    const int rad = radius*Physics::PIXELS_PER_METER;
    // static const int TOL = 1;
    // if (!SDL_HasIntersection(&SDL_Rect{0, 0, SCREEN_WIDTH, SCREEN_HEIGHT}, &SDL_Rect{x - rad - TOL, y - rad - TOL, rad*2 + TOL*2, rad*2 + TOL*2}))
        // return;
    circleRGBA(renderer, x, y, rad, color.r*255.0, color.g*255.0, color.b*255.0, color.a*255.0);
    // std::cout << "DrawCircle" << std::endl;
}

void DrawPhysics::DrawSolidCircle(const b2Vec2& center, float radius, const b2Vec2& axis, const b2Color& color)
{
    filledCircleRGBA(renderer, center.x*Physics::PIXELS_PER_METER - camera.pos.x, center.y*Physics::PIXELS_PER_METER - camera.pos.y, radius*Physics::PIXELS_PER_METER, color.r*255.0, color.g*255.0, color.b*255.0, color.a*255.0);
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, color.a*255.0);
    SDL_RenderDrawLine(renderer, center.x*Physics::PIXELS_PER_METER - camera.pos.x, center.y*Physics::PIXELS_PER_METER - camera.pos.y, (center.x + axis.x*radius)*Physics::PIXELS_PER_METER - camera.pos.x, (center.y + axis.y*radius)*Physics::PIXELS_PER_METER - camera.pos.y);
    // std::cout << "DrawSolidCircle" << std::endl;
}

void DrawPhysics::DrawSegment(const b2Vec2& p1, const b2Vec2& p2, const b2Color& color)
{
    SDL_SetRenderDrawColor(renderer, color.r*255.0, color.g*255.0, color.b*255.0, color.a*255.0);
    SDL_RenderDrawLine(renderer, p1.x*Physics::PIXELS_PER_METER - camera.pos.x, p1.y*Physics::PIXELS_PER_METER - camera.pos.y, p2.x*Physics::PIXELS_PER_METER - camera.pos.x, p2.y*Physics::PIXELS_PER_METER - camera.pos.y);
    // std::cout << "DrawSegment" << std::endl;
}

void DrawPhysics::DrawTransform(const b2Transform& xf)
{
    (void)xf;
    // std::cout << "DrawTransform" << std::endl;
}

void DrawPhysics::DrawPoint(const b2Vec2 &p, float size, const b2Color &color)
{
    filledCircleRGBA(renderer, p.x*Physics::PIXELS_PER_METER - camera.pos.x, p.y*Physics::PIXELS_PER_METER - camera.pos.y, size/2.0, color.r*255.0, color.g*255.0, color.b*255.0, color.a*255.0);
    // std::cout << "DrawPoint" << std::endl;
}