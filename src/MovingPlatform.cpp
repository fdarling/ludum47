#include "MovingPlatform.h"
#include "globals.h"

#include <box2d/b2_body.h>
#include <box2d/b2_fixture.h>
#include <box2d/b2_polygon_shape.h>
#include <box2d/b2_contact.h>
#include <box2d/b2_prismatic_joint.h>
// #include <iostream>
MovingPlatform::MovingPlatform(const b2Vec2 &p1, const b2Vec2 &p2) : body(nullptr), fixture(nullptr), joint(nullptr), countdown(0.0)
{
    b2Vec2 pt1 = p1;
    b2Vec2 pt2 = p2;
    pt1 *= Physics::METERS_PER_PIXEL;
    pt2 *= Physics::METERS_PER_PIXEL;

    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    bodyDef.fixedRotation = true;
    bodyDef.gravityScale = 1.0;
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
    fixtureDef.friction = 0.0;

    fixture = body->CreateFixture(&fixtureDef);

    b2Vec2 elevator_dir = pt2 - pt1;
    const float elevator_len = elevator_dir.Normalize();

    b2PrismaticJointDef testJointDef;
    testJointDef.Initialize(world.groundBody, body, pt1, elevator_dir);
    testJointDef.enableLimit = true;
    testJointDef.lowerTranslation = 0.0;
    testJointDef.upperTranslation = elevator_len;
    joint = Physics::world.CreateJoint(&testJointDef);
}

int MovingPlatform::type() const
{
    return GAMEOBJECT_TYPE_MOVINGPLATFORM;
}

void MovingPlatform::advance(float ms)
{
    (void)ms;
    // countdown -= ms;
    // if (countdown < 0.0)
        // countdown = 0.0;

    b2Vec2 vel = body->GetLinearVelocity();
    // std::cout << "vel.y = " << vel.y << std::endl;
    // if (countdown == 0.0)
    if (std::abs(vel.y) < 0.0001)
    {
        // std::cout << "GOTTI" << std::endl;
        const b2Vec2 pos = body->GetPosition();
        b2Vec2 dir1 = pos - joint->GetAnchorA();
        b2Vec2 dir2 = pos - joint->GetAnchorB();
        
        // const bool a_is_closer = (dir1.Length() < dir2.Length());
        
        dir1.Normalize();
        dir2.Normalize();
        dir1 *= 5.0;
        dir2 *= 5.0;
        
        // vel.y = std::copysign(5.0, a_is_closer ? dir2.y : dir1.y);
        // std::cout << "NEW vel.y = " << (a_is_closer ? dir2 : dir1).y << std::endl;
        // body->SetLinearVelocity(a_is_closer ? dir2 : dir1);
        body->SetGravityScale(-std::copysign(1.0, body->GetGravityScale()));
    }
}
