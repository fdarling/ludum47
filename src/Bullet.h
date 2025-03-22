#pragma once

#include "GameObject.h"

class b2Vec2; // forward declaration instead of #include <box2d/b2_math.h>
class b2Body; // forward declaration instead of #include <box2d/b2_body.h>
class b2Fixture; // forward declaration instead of #include <box2d/b2_fixture.h>

class Bullet : public GameObject
{
public:
    Bullet(const b2Vec2 &p, const b2Vec2 &v);
    int type() const override;
    void draw(const Point &offset) const;
    void preSolve(b2Contact *contact, const b2Manifold *oldManifold, b2Fixture *ourFixture, b2Fixture *otherFixture) override;
public:
    b2Body *body;
    b2Fixture *fixture;
};
