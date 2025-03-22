#include "Grenade.h"
#include "DrawFixtures.h"
#include "globals.h"

#include <box2d/b2_body.h>
#include <box2d/b2_fixture.h>
#include <box2d/b2_circle_shape.h>
#include <box2d/b2_contact.h>

static const float GRENADE_RADIUS_IN_PIXELS = 8.0;
static const SDL_Color GRENADE_COLOR = {128, 165, 32, 255};

Grenade::Grenade(const b2Vec2 &p, const b2Vec2 &v) : body(nullptr), bombFixture(nullptr), radiusFixture(nullptr), bomb_timer(2.0)
{
    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    bodyDef.fixedRotation = true;
    bodyDef.position.Set(p.x, p.y);
    body = Physics::world.CreateBody(&bodyDef);
    body->GetUserData().pointer = reinterpret_cast<uintptr_t>(this);

    b2CircleShape circleShape;
    circleShape.m_radius = GRENADE_RADIUS_IN_PIXELS*Physics::METERS_PER_PIXEL;

    b2FixtureDef fixtureDef;
    fixtureDef.shape = &circleShape;
    fixtureDef.density = 1.0;
    fixtureDef.friction = 0.0;

    bombFixture = body->CreateFixture(&fixtureDef);

    circleShape.m_radius = 150.0*Physics::METERS_PER_PIXEL;
    fixtureDef.isSensor = true;
    radiusFixture = body->CreateFixture(&fixtureDef);

    body->SetLinearVelocity(v);
}

Grenade::~Grenade()
{
    Physics::world.DestroyBody(body);
}

int Grenade::type() const
{
    return GAMEOBJECT_TYPE_GRENADE;
}

void Grenade::advance(float ms)
{
    const bool was_expired = (bomb_timer == 0.0);
    bomb_timer -= ms;
    if (bomb_timer < 0.0)
        bomb_timer = 0.0;

    const bool is_expired = (bomb_timer == 0.0);
    if (is_expired && !was_expired)
    {
        // world.deleteLater(this);
        for (b2Body *other_body : within_radius)
        {
            b2Vec2 impulse_vec = other_body->GetPosition() - body->GetPosition();
            if (!impulse_vec.IsValid())
                continue;
            const float distance = impulse_vec.Normalize();
            if (distance == 0.0)
                continue;
            static const float MAX_IMPULSE = 15.0;
            impulse_vec *= MAX_IMPULSE/distance/distance;
            other_body->ApplyLinearImpulseToCenter(impulse_vec, true);
        }
    }
}

void Grenade::draw(const Point &offset) const
{
    DrawFixture(offset, bombFixture, GRENADE_COLOR);
}

void Grenade::preSolve(b2Contact *contact, const b2Manifold *oldManifold, b2Fixture *ourFixture, b2Fixture *otherFixture)
{
    (void)oldManifold;
    
    if (ourFixture != bombFixture)
        return;

    GameObject * const obj = reinterpret_cast<GameObject*>(otherFixture->GetBody()->GetUserData().pointer);
    if (!obj)
        return;

    // make it so the grenades cannot collide with certain objects
    if (obj->type() == GAMEOBJECT_TYPE_PLAYER || obj->type() == GAMEOBJECT_TYPE_BULLET || obj->type() == GAMEOBJECT_TYPE_GRENADE)
    {
        contact->SetEnabled(false);
    }
}

void Grenade::beginContact(b2Contact *contact, b2Fixture *ourFixture, b2Fixture *otherFixture)
{
    (void)contact;

    if (ourFixture != radiusFixture)
        return;

    within_radius.insert(otherFixture->GetBody());
}

void Grenade::endContact(b2Contact *contact, b2Fixture *ourFixture, b2Fixture *otherFixture)
{
    (void)contact;

    if (ourFixture != radiusFixture)
        return;

    within_radius.erase(otherFixture->GetBody());
}
