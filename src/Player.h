#pragma once

#include <box2d/b2_body.h>
#include <box2d/b2_fixture.h>
#include <box2d/b2_edge_shape.h>
#include <box2d/b2_joint.h>

#include "Rect.h"

#include <set>

class Player
{
public:
    Player();
    ~Player();
    bool init();
    void walkLeft();
    void walkRight();
    void shootBullet();
    void setJetpack(bool on);
    void setGrappling(bool on);
    void jump();
    void advance();
    void draw(const Point &offset) const;
    bool isStandingOnGround() const {return !standing_on.empty();}
    void beginContact(b2Contact *contact, b2Fixture *other);
    void endContact(b2Contact *contact, b2Fixture *other);
    Rect rect;
    // SDL_Texture *_standingTexture;
    SDL_Texture *_walkingTexture;
    int _frameIndex;
    bool _walkingLeft;
    bool _jetpackOn;
    bool _grappling;
    Point _lastPos;
    b2Body *body;
    b2Fixture *fixture;
    b2Joint *hanging_joint;
    std::set<b2Fixture*> standing_on;
    std::set<b2EdgeShape*> hanging_under;
};
