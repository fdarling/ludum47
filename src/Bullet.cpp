#include "Bullet.h"
#include "globals.h"

#include <box2d/b2_body.h>
#include <box2d/b2_fixture.h>
#include <box2d/b2_circle_shape.h>

Bullet::Bullet(const b2Vec2 &p, const b2Vec2 &v) : body(nullptr), fixture(nullptr)
{
    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    bodyDef.fixedRotation = true;
    bodyDef.position.Set(p.x, p.y);
    body = Physics::world.CreateBody(&bodyDef);
    body->GetUserData().pointer = reinterpret_cast<uintptr_t>(this);

    b2CircleShape circleShape;
    circleShape.m_radius = 5.0*Physics::METERS_PER_PIXEL;

    b2FixtureDef fixtureDef;
    fixtureDef.shape = &circleShape;
    fixtureDef.density = 1.0;
    fixtureDef.friction = 0.0;

    fixture = body->CreateFixture(&fixtureDef);

    body->SetLinearVelocity(v);
}

int Bullet::type() const
{
    return GAMEOBJECT_TYPE_BULLET;
}
