#include "MovingPlatform.h"
#include "DrawFixtures.h"
#include "globals.h"

#include <box2d/b2_body.h>
#include <box2d/b2_polygon_shape.h>
#include <box2d/b2_contact.h>
#include <box2d/b2_prismatic_joint.h>

// #include <iostream>
#include <algorithm> // for std::clamp()

MovingPlatform::MovingPlatform(const b2Vec2 &p1, const b2Vec2 &p2) : body(nullptr), fixture(nullptr), state(MOVINGPLATFORM_STATE_WAIT_AT_P1), countdown(0.0)
{
    b2Vec2 pt1 = p1;
    b2Vec2 pt2 = p2;
    pt1 *= Physics::METERS_PER_PIXEL;
    pt2 *= Physics::METERS_PER_PIXEL;
    _p1 = pt1;
    _p2 = pt2;

    b2BodyDef bodyDef;
    bodyDef.type = b2_kinematicBody;
    bodyDef.fixedRotation = true;
    bodyDef.position.Set(pt1.x, pt1.y);
    body = Physics::world.CreateBody(&bodyDef);
    body->GetUserData().pointer = reinterpret_cast<uintptr_t>(this);

    const float w = 64*Physics::METERS_PER_PIXEL;
    const float h = 16*Physics::METERS_PER_PIXEL;

    b2PolygonShape boxShape;
    boxShape.SetAsBox(w/2.0, h/2.0);

    b2FixtureDef fixtureDef;
    fixtureDef.shape = &boxShape;
    fixtureDef.density = 1.0;
    fixtureDef.friction = 1.0;

    fixture = body->CreateFixture(&fixtureDef);

    body->SetLinearVelocity(b2Vec2(0.0, 3.0));
}

int MovingPlatform::type() const
{
    return GAMEOBJECT_TYPE_MOVINGPLATFORM;
}

void MovingPlatform::advance(float ms)
{
    // advance timer
    countdown -= ms;
    if (countdown < 0.0)
        countdown = 0.0;

    // platform constants
    static const float MIN_SPEED = 0.05;
    static const float MAX_SPEED = 3.0;
    static const float WAIT_SECONDS = 0.25;
    static const float ACCEL_DISTANCE = 32.0*Physics::METERS_PER_PIXEL;

    // figure out accel/decel reference point (and distance to/from it)
    const bool use_p2_as_reference = (state >= MOVINGPLATFORM_STATE_CRUISE_FROM_P1_TO_P2);
    const b2Vec2 ref_distance_vec  = (use_p2_as_reference ? _p2 : _p1) - body->GetPosition();
    const b2Vec2 ref_distance_vec2 = (use_p2_as_reference ? _p1 : _p2) - body->GetPosition();
    const float ref_distance  = ref_distance_vec.Length();
    const float ref_distance2 = ref_distance_vec2.Length();

    // figure out what speed we'll be using
    const float speed_ratio = std::clamp(ref_distance/ACCEL_DISTANCE, 0.0f, 1.0f);
    const float chosen_speed = std::max(MIN_SPEED, MAX_SPEED*speed_ratio);

    // calculate speed/direction to move in
    b2Vec2 chosen_vel = _p2 - _p1;
    if (!use_p2_as_reference)
        chosen_vel *= -1.0;
    chosen_vel.Normalize();
    chosen_vel *= chosen_speed;

    // print debugging information
    // static int old_state = state;
    // if (state != old_state)
    // {
        // std::cout << "state = " << state << "; chosen_speed = " << chosen_speed << "; chosen_vel = (" << chosen_vel.x << ", " << chosen_vel.y << ")" << std::endl;
    // }
    // old_state = state;

    switch (state)
    {
        case MOVINGPLATFORM_STATE_CRUISE_FROM_P2_TO_P1: // 0
        if (chosen_speed < MAX_SPEED)
        {
            state = MOVINGPLATFORM_STATE_DECEL_TOWARDS_P1;
        }
        break;

        case MOVINGPLATFORM_STATE_DECEL_TOWARDS_P1: // 1
        if (chosen_speed <= MIN_SPEED)
        {
            state = MOVINGPLATFORM_STATE_WAIT_AT_P1;
            countdown = WAIT_SECONDS;
            chosen_vel.SetZero();
            // body->SetTransform(_p1, 0.0);
        }
        break;

        case MOVINGPLATFORM_STATE_WAIT_AT_P1: // 2
        if (countdown == 0.0)
        {
            state = MOVINGPLATFORM_STATE_ACCEL_AWAY_FROM_P1;
            chosen_vel *= -1.0;
        }
        else
        {
            chosen_vel.SetZero();
        }
        break;

        case MOVINGPLATFORM_STATE_ACCEL_AWAY_FROM_P1: // 3
        chosen_vel *= -1.0;
        if (ref_distance2 < ref_distance)
        {
            state = MOVINGPLATFORM_STATE_DECEL_TOWARDS_P2;
        }
        else if (chosen_speed >= MAX_SPEED)
        {
            state = MOVINGPLATFORM_STATE_CRUISE_FROM_P1_TO_P2;
        }
        break;

        case MOVINGPLATFORM_STATE_CRUISE_FROM_P1_TO_P2: // 4
        if (chosen_speed < MAX_SPEED)
        {
            state = MOVINGPLATFORM_STATE_DECEL_TOWARDS_P2;
        }
        break;

        case MOVINGPLATFORM_STATE_DECEL_TOWARDS_P2: // 5
        if (chosen_speed <= MIN_SPEED)
        {
            state = MOVINGPLATFORM_STATE_WAIT_AT_P2;
            countdown = WAIT_SECONDS;
            chosen_vel.SetZero();
            // body->SetTransform(_p2, 0.0);
        }
        break;

        case MOVINGPLATFORM_STATE_WAIT_AT_P2: // 6
        if (countdown == 0.0)
        {
            state = MOVINGPLATFORM_STATE_ACCEL_AWAY_FROM_P2;
            chosen_vel *= -1.0;
        }
        else
        {
            chosen_vel.SetZero();
        }
        break;

        case MOVINGPLATFORM_STATE_ACCEL_AWAY_FROM_P2: // 7
        chosen_vel *= -1.0;
        if (ref_distance2 < ref_distance)
        {
            state = MOVINGPLATFORM_STATE_DECEL_TOWARDS_P1;
        }
        else if (chosen_speed >= MAX_SPEED)
        {
            state = MOVINGPLATFORM_STATE_CRUISE_FROM_P2_TO_P1;
        }
        break;
    }

    // apply chosen velocity
    body->SetLinearVelocity(chosen_vel);
}

void MovingPlatform::draw(const Point &offset) const
{
    static const SDL_Color COLOR = {255, 0, 0, 255};
    DrawBody(offset, body, COLOR);
}
