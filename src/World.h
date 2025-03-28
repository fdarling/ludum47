#pragma once

#include <SDL2/SDL.h>

class b2Body; // forward declaration instead of #include <box2d/b2_body.h>

#include "GameObject.h"
#include "Point.h"
#include "DrawPhysics.h"
#include "ContactListener.h"

#include <vector>
#include <memory>

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
    void addChild(std::shared_ptr<GameObject> child);
    void removeChild(std::weak_ptr<GameObject> child);
    std::weak_ptr<GameObject> findChild(GameObject *child);
    void deleteLater(std::weak_ptr<GameObject> child);
    void performDeletions();
    b2Body *groundBody;
protected:
    SDL_Texture *_atlas;
    // SDL_Texture *_bg;
    DrawPhysics _debugDraw;
    ContactListener _contactListener;
    std::vector< std::shared_ptr<GameObject> > _children;
    std::vector< std::weak_ptr<GameObject> > _deleteLater;
};
