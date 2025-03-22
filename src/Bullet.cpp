#include "Bullet.h"
#include "DrawFixtures.h"
#include "globals.h"

#include <box2d/b2_body.h>
#include <box2d/b2_fixture.h>
#include <box2d/b2_circle_shape.h>
#include <box2d/b2_contact.h>

static const float BULLET_RADIUS_IN_PIXELS = 5.0;
static const SDL_Color BULLET_COLOR = {218, 165, 32, 255};

Bullet::Bullet(const b2Vec2 &p, const b2Vec2 &v) : body(nullptr), fixture(nullptr)
{
    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    bodyDef.fixedRotation = true;
    bodyDef.position.Set(p.x, p.y);
    body = Physics::world.CreateBody(&bodyDef);
    body->GetUserData().pointer = reinterpret_cast<uintptr_t>(this);

    b2CircleShape circleShape;
    circleShape.m_radius = BULLET_RADIUS_IN_PIXELS*Physics::METERS_PER_PIXEL;

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

void Bullet::draw(const Point &offset) const
{
    DrawBody(offset, body, BULLET_COLOR);
}

void Bullet::preSolve(b2Contact *contact, const b2Manifold *oldManifold, b2Fixture *ourFixture, b2Fixture *otherFixture)
{
    (void)oldManifold;
    (void)ourFixture;

    GameObject * const obj = reinterpret_cast<GameObject*>(otherFixture->GetBody()->GetUserData().pointer);
    if (!obj)
        return;

    if (obj->type() == GAMEOBJECT_TYPE_PLAYER)
    {
        contact->SetEnabled(false); // make it so the bullets cannot collide with the player
    }
}
