#pragma once

#include <box2d/b2_body.h>
#include <box2d/b2_fixture.h>

class Bullet
{
public:
    Bullet(const b2Vec2 &p, const b2Vec2 &v);
public:
    b2Body *body;
    b2Fixture *fixture;
};
