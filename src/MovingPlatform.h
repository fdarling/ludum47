#pragma once

#include "GameObject.h"

#include <box2d/b2_math.h>
class b2Body; // forward declaration instead of #include <box2d/b2_body.h>
class b2Fixture; // forward declaration instead of #include <box2d/b2_fixture.h>

class MovingPlatform : public GameObject
{
public:
    MovingPlatform(const b2Vec2 &p, const b2Vec2 &v);
    int type() const override;
    void advance(float ms) override;
public:
    enum MovingPlatformState
    {
        MOVINGPLATFORM_STATE_CRUISE_FROM_P2_TO_P1,
        MOVINGPLATFORM_STATE_DECEL_TOWARDS_P1,
        MOVINGPLATFORM_STATE_WAIT_AT_P1,
        MOVINGPLATFORM_STATE_ACCEL_AWAY_FROM_P1,

        MOVINGPLATFORM_STATE_CRUISE_FROM_P1_TO_P2,
        MOVINGPLATFORM_STATE_DECEL_TOWARDS_P2,
        MOVINGPLATFORM_STATE_WAIT_AT_P2,
        MOVINGPLATFORM_STATE_ACCEL_AWAY_FROM_P2
    };
public:
    b2Body *body;
    b2Fixture *fixture;
    b2Vec2 _p1;
    b2Vec2 _p2;
    MovingPlatformState state;
    float countdown;
};
