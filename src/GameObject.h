#pragma once

class b2Contact; // forward declaration instead of #include <box2d/b2_contact.h>
class b2Manifold; // forward declaration instead of #include <box2d/b2_collision.h>
class b2ContactImpulse; // forward declaration instead of #include <box2d/b2_world_callbacks.h>
class b2Fixture; // forward declaration instead of #include <box2d/b2_fixture.h>

class GameObject
{
public:
    GameObject();
    virtual ~GameObject();
    virtual int type() const = 0;
    virtual void preSolve(b2Contact *contact, const b2Manifold *oldManifold, b2Fixture *other);
    virtual void postSolve(b2Contact *contact, const b2ContactImpulse *impulse, b2Fixture *other);
    virtual void beginContact(b2Contact *contact, b2Fixture *other);
    virtual void endContact(b2Contact *contact, b2Fixture *other);
};
