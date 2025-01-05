#pragma once

#include <box2d/b2_body.h>
#include <box2d/b2_fixture.h>

#include "Rect.h"

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
    b2Body *body;
    b2Fixture *fixture;
    bool touchingGround;
};
