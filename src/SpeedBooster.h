#pragma once

#include "GameObject.h"

class b2Vec2; // forward declaration instead of #include <box2d/b2_math.h>
class b2Body; // forward declaration instead of #include <box2d/b2_body.h>
class b2Fixture; // forward declaration instead of #include <box2d/b2_fixture.h>

class SpeedBooster : public GameObject
{
public:
    SpeedBooster(const b2Vec2 &p1, const b2Vec2 &p2);
    int type() const override;
    void draw(const Point &offset) const;
    void beginContact(b2Contact *contact, b2Fixture *ourFixture, b2Fixture *otherFixture) override;
public:
    b2Body *body;
    b2Fixture *fixture;
};
