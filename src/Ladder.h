#pragma once

#include "GameObject.h"

#include <box2d/b2_body.h>
#include <box2d/b2_fixture.h>

class Ladder : public GameObject
{
public:
    Ladder(const b2Vec2 &p1, const b2Vec2 &p2);
    int type() const override;
public:
    b2Body *body;
    b2Fixture *fixture;
};
