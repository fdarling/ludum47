#include "SpeedBooster.h"
#include "MakeFixture.h"
#include "globals.h"

#include <box2d/b2_body.h>
#include <box2d/b2_fixture.h>
#include <box2d/b2_polygon_shape.h>
#include <box2d/b2_contact.h>

SpeedBooster::SpeedBooster(const b2Vec2 &p1, const b2Vec2 &p2) : body(nullptr), fixture(nullptr)
{
    b2BodyDef bodyDef;
    bodyDef.fixedRotation = true;
    // const b2Vec2 center((p1.x + p2.x)*Physics::METERS_PER_PIXEL/2.0, (p1.y + p2.y)*Physics::METERS_PER_PIXEL/2.0);
    // bodyDef.position.Set(center.x, center.y);
    body = Physics::world.CreateBody(&bodyDef);
    body->GetUserData().pointer = reinterpret_cast<uintptr_t>(this);

    // const float w = std::abs(p2.x-p1.x)*Physics::METERS_PER_PIXEL;
    // const float h = std::abs(p2.y-p1.y)*Physics::METERS_PER_PIXEL;

    // b2PolygonShape boxShape;
    // boxShape.SetAsBox(w/2.0, h/2.0);

    // b2FixtureDef fixtureDef;
    // fixtureDef.shape = &boxShape;
    // fixtureDef.density = 1.0;
    // fixtureDef.friction = 0.0;
    // fixtureDef.isSensor = true;
    
    std::vector<b2Vec2> pts;
    pts.emplace_back(p1.x, p2.y); // top left
    pts.emplace_back(p2.x, p2.y); // top right
    pts.emplace_back(p2.x, p1.y); // bottom right
    pts.emplace_back(p1.x, p1.y); // bottom left
    MakeFixture(body, pts);

    // fixture = body->CreateFixture(&fixtureDef);
}

int SpeedBooster::type() const
{
    return GAMEOBJECT_TYPE_SPEEDBOOSTER;
}

void SpeedBooster::beginContact(b2Contact *contact, b2Fixture *ourFixture, b2Fixture *otherFixture)
{
    (void)ourFixture;

    GameObject * const obj = reinterpret_cast<GameObject*>(otherFixture->GetBody()->GetUserData().pointer);
    if (!obj)
        return;

    contact->SetTangentSpeed(50.0);
}
