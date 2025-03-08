#pragma once

#include <SDL2/SDL.h>

class b2Body; // forward declaration instead of #include <box2d/b2_body.h>

#include "GameObject.h"
#include "Point.h"
#include "DrawPhysics.h"
#include "ContactListener.h"

#include <vector>

class World : public GameObject
{
public:
    World();
    ~World();
    int type() const;
    bool init();
    void quit();
    void draw(const Point &cameraPos);
    void advance();
    b2Body *groundBody;
protected:
    SDL_Texture *_atlas;
    // SDL_Texture *_bg;
    Uint32 _lastTime;
    DrawPhysics _debugDraw;
    ContactListener _contactListener;
    std::vector<GameObject*> _children;
};
