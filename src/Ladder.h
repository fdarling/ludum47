#pragma once

#include "GameObject.h"

class b2Vec2; // forward declaration instead of #include <box2d/b2_math.h>
class b2Body; // forward declaration instead of #include <box2d/b2_body.h>
class b2Fixture; // forward declaration instead of #include <box2d/b2_fixture.h>

class Ladder : public GameObject
{
public:
    Ladder(const b2Vec2 &p1, const b2Vec2 &p2);
    int type() const override;
    void draw(const Point &offset) const override;
public:
    b2Body *body;
    b2Fixture *fixture;
};
