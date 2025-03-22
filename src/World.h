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
    void draw(const Point &offset) const override;
    void advance(float ms) override;
    void addChild(GameObject *child);
    void removeChild(GameObject *child);
    void deleteLater(GameObject *child);
    void performDeletions();
    b2Body *groundBody;
protected:
    SDL_Texture *_atlas;
    // SDL_Texture *_bg;
    DrawPhysics _debugDraw;
    ContactListener _contactListener;
    std::vector<GameObject*> _children;
    std::vector<GameObject*> _deleteLater;
};
