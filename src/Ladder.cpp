#include "Ladder.h"
#include "DrawFixtures.h"
#include "globals.h"

#include <box2d/b2_body.h>
#include <box2d/b2_fixture.h>
#include <box2d/b2_polygon_shape.h>

Ladder::Ladder(const b2Vec2 &p1, const b2Vec2 &p2) : body(nullptr), fixture(nullptr)
{
    b2BodyDef bodyDef;
    bodyDef.fixedRotation = true;
    const b2Vec2 center((p1.x + p2.x)*Physics::METERS_PER_PIXEL/2.0, (p1.y + p2.y)*Physics::METERS_PER_PIXEL/2.0);
    bodyDef.position.Set(center.x, center.y);
    body = Physics::world.CreateBody(&bodyDef);
    body->GetUserData().pointer = reinterpret_cast<uintptr_t>(this);

    const float w = std::abs(p2.x-p1.x)*Physics::METERS_PER_PIXEL;
    const float h = std::abs(p2.y-p1.y)*Physics::METERS_PER_PIXEL;

    b2PolygonShape boxShape;
    boxShape.SetAsBox(w/2.0, h/2.0);

    b2FixtureDef fixtureDef;
    fixtureDef.shape = &boxShape;
    fixtureDef.density = 1.0;
    fixtureDef.friction = 0.0;
    fixtureDef.isSensor = true;

    fixture = body->CreateFixture(&fixtureDef);
}

int Ladder::type() const
{
    return GAMEOBJECT_TYPE_LADDER;
}

void Ladder::draw(const Point &offset) const
{
    static const SDL_Color COLOR = {255, 255, 0, 255};
    DrawFixtures(offset, body->GetFixtureList(), COLOR);
}
