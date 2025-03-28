#pragma once

#include "GameObject.h"

#include <set>
#include <memory>

class b2Vec2; // forward declaration instead of #include <box2d/b2_math.h>
class b2Body; // forward declaration instead of #include <box2d/b2_body.h>
class b2Fixture; // forward declaration instead of #include <box2d/b2_fixture.h>

struct WeakBodyPtrLess
{
    bool operator()(const std::weak_ptr<b2Body> &a, const std::weak_ptr<b2Body> &b) const
    {
        b2Body * const a_ptr = a.lock().get();
        b2Body * const b_ptr = b.lock().get();
        // std::cout << "comparing: " << a_ptr << "," << b_ptr << std::endl;
        return std::less<void*>()(a_ptr, b_ptr);
    }
};

class Grenade : public GameObject
{
public:
    Grenade(const b2Vec2 &p, const b2Vec2 &v);
    ~Grenade();
    int type() const override;
    void advance(float ms) override;
    void draw(const Point &offset) const;
    void preSolve(b2Contact *contact, const b2Manifold *oldManifold, b2Fixture *ourFixture, b2Fixture *otherFixture) override;
    void beginContact(b2Contact *contact, b2Fixture *ourFixture, b2Fixture *otherFixture) override;
    void endContact(b2Contact *contact, b2Fixture *ourFixture, b2Fixture *otherFixture) override;
public:
    b2Body *body;
    b2Fixture *bombFixture;
    b2Fixture *radiusFixture;
    float bomb_timer;
    std::set< std::weak_ptr<b2Body>, WeakBodyPtrLess > within_radius;
};
