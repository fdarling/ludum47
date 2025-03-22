#pragma once

class b2Body; // forward declaration instead of #include <box2d/b2_body.h>
class b2Fixture; // forward declaration instead of #include <box2d/b2_fixture.h>
class b2EdgeShape; // forward declaration instead of #include <box2d/b2_edge_shape.h>
class b2Joint; // forward declaration instead of #include <box2d/b2_joint.h>

#include "GameObject.h"
#include "Rect.h"

#include <set>

class Player : public GameObject
{
public:
    Player();
    ~Player();
    int type() const override;
    bool init();
    void walkLeft();
    void walkRight();
    void climbUp();
    void climbDown();
    void shootBullet();
    void shootGrenade();
    void setJetpack(bool on);
    void setGrappling(bool on);
    void jump();
    void advance();
    void draw(const Point &offset) const override;
    bool isStandingOnGround() const {return !standing_on.empty();}
    bool isClimbing() const {return !climbing_up.empty();}
    void beginContact(b2Contact *contact, b2Fixture *ourFixture, b2Fixture *otherFixture) override;
    void endContact(b2Contact *contact, b2Fixture *ourFixture, b2Fixture *otherFixture) override;
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
    std::set<b2Fixture*> climbing_up;
    std::set<b2EdgeShape*> hanging_under;
};
