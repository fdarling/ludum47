#pragma once

#include <SDL2/SDL.h>
#include <box2d/b2_body.h>

#include "Point.h"
#include "DrawPhysics.h"
#include "ContactListener.h"

class World
{
public:
    World();
    ~World();
    bool init();
    void quit();
    void draw(const Point &cameraPos);
    void advance();
    b2Body *groundBody;
protected:
    // SDL_Texture *_atlas;
    // SDL_Texture *_bg;
    Uint32 _lastTime;
    DrawPhysics _debugDraw;
    ContactListener _contactListener;
};
