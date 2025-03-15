#pragma once

#include "GameObject.h"

#include <set>

class b2Vec2; // forward declaration instead of #include <box2d/b2_math.h>
class b2Body; // forward declaration instead of #include <box2d/b2_body.h>
class b2Fixture; // forward declaration instead of #include <box2d/b2_fixture.h>

class Grenade : public GameObject
{
public:
    Grenade(const b2Vec2 &p, const b2Vec2 &v);
    ~Grenade();
    int type() const override;
    void advance(float ms) override;
    void preSolve(b2Contact *contact, const b2Manifold *oldManifold, b2Fixture *ourFixture, b2Fixture *otherFixture) override;
    void beginContact(b2Contact *contact, b2Fixture *ourFixture, b2Fixture *otherFixture) override;
    void endContact(b2Contact *contact, b2Fixture *ourFixture, b2Fixture *otherFixture) override;
public:
    b2Body *body;
    b2Fixture *bombFixture;
    b2Fixture *radiusFixture;
    float bomb_timer;
    std::set<b2Body*> within_radius;
};
