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
public:
    b2Body *body;
    b2Fixture *fixture;
};
