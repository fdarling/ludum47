#pragma once

struct SDL_Color;
class b2Body;
class b2Fixture;
class b2Vec2;

template <class> class PointBase;
typedef PointBase<int> Point;

void DrawBody(const Point &offset, b2Body *body, const SDL_Color &color);
void DrawFixtures(const Point &offset, b2Fixture *fixture, const SDL_Color &color);
void DrawFixture(const Point &offset, b2Fixture *fixture, const SDL_Color &color);
