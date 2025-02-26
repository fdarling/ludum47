#pragma once

#include <box2d/b2_body.h>
#include <box2d/b2_fixture.h>

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
    void setJetpack(bool on);
    void jump();
    void advance();
    void draw(const Point &offset) const;
    bool isStandingOnGround() const {return !standing_on.empty();}
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
    std::set<b2Fixture*> standing_on;
};
