#include "Spring.h"
#include "globals.h"

#include <box2d/b2_body.h>
#include <box2d/b2_fixture.h>
#include <box2d/b2_polygon_shape.h>
#include <box2d/b2_contact.h>

Spring::Spring(const b2Vec2 &p1, const b2Vec2 &p2) : body(nullptr), fixture(nullptr)
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
    // fixtureDef.isSensor = true;

    fixture = body->CreateFixture(&fixtureDef);
}

int Spring::type() const
{
    return GAMEOBJECT_TYPE_SPRING;
}

void Spring::beginContact(b2Contact *contact, b2Fixture *ourFixture, b2Fixture *otherFixture)
{
    (void)contact;
    (void)ourFixture;

    GameObject * const obj = reinterpret_cast<GameObject*>(otherFixture->GetBody()->GetUserData().pointer);
    if (!obj)
        return;

    // if (obj->type() == GAMEOBJECT_TYPE_PLAYER)

    b2WorldManifold contactWorldManifold;
    contact->GetWorldManifold(&contactWorldManifold);

    if (contactWorldManifold.normal.y <= -0.4)
    {
        // bounce method (you bounce higher each time though)
        // contact->SetRestitution(1.5); // bounce factor

        // finite energy addition method
        // contact->SetRestitution(0.0); // TODO: dead-stop first for impulse consistency?
        // other->GetBody()->ApplyLinearImpulseToCenter(b2Vec2(0.0, -5.0), true); // TODO should be considering mass of target?

        // constant initial speed method
        b2Vec2 v = otherFixture->GetBody()->GetLinearVelocity();
        v.y = -7.0;
        otherFixture->GetBody()->SetLinearVelocity(v);
    }
}
