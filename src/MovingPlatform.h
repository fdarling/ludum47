#pragma once

#include "GameObject.h"

class b2Vec2; // forward declaration instead of #include <box2d/b2_math.h>
class b2Body; // forward declaration instead of #include <box2d/b2_body.h>
class b2Fixture; // forward declaration instead of #include <box2d/b2_fixture.h>
class b2Joint; // forward declaration instead of #include <box2d/b2_joint.h>

class MovingPlatform : public GameObject
{
public:
    MovingPlatform(const b2Vec2 &p, const b2Vec2 &v);
    int type() const override;
    void advance(float ms) override;
public:
    b2Body *body;
    b2Fixture *fixture;
    b2Joint *joint;
    float countdown;
};
