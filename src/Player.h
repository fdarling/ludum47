#pragma once

// #include <Box2D/Box2D.h>
#include <box2d/box2d.h>

#include "Rect.h"

// #include <BulletCollision/CollisionShapes/btCollisionShape.h>
// #include <BulletDynamics/Dynamics/btRigidBody.h>

class Player
{
public:
    Player();
    ~Player();
    bool init();
    void walkLeft();
    void walkRight();
    void jump();
    void advance();
    void draw(const Point &offset) const;
    Rect rect;
    // SDL_Texture *_standingTexture;
    SDL_Texture *_walkingTexture;
    int _frameIndex;
    bool _walkingLeft;
    Point _lastPos;
    // btCollisionShape *shape;
    // btRigidBody *body;
    b2Body *body;
    b2Fixture *fixture;
    bool touchingGround;
};
