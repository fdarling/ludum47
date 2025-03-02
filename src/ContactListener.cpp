#include "ContactListener.h"
#include "globals.h"

#include <box2d/b2_contact.h>
#include <box2d/b2_edge_shape.h>

// #include <iostream>

void ContactListener::PreSolve(b2Contact *contact, const b2Manifold *oldManifold)
{
    b2Fixture * const fixtureA = contact->GetFixtureA();
    b2Fixture * const fixtureB = contact->GetFixtureB();
    b2Body * const bodyA = fixtureA->GetBody();
    b2Body * const bodyB = fixtureB->GetBody();
    GameObject * const objA = reinterpret_cast<GameObject*>(bodyA->GetUserData().pointer);
    GameObject * const objB = reinterpret_cast<GameObject*>(bodyB->GetUserData().pointer);
    if (objA)
        objA->preSolve(contact, oldManifold, fixtureB);
    if (objB)
        objB->preSolve(contact, oldManifold, fixtureA);
}

void ContactListener::PostSolve(b2Contact *contact, const b2ContactImpulse *impulse)
{
    b2Fixture * const fixtureA = contact->GetFixtureA();
    b2Fixture * const fixtureB = contact->GetFixtureB();
    b2Body * const bodyA = fixtureA->GetBody();
    b2Body * const bodyB = fixtureB->GetBody();
    GameObject * const objA = reinterpret_cast<GameObject*>(bodyA->GetUserData().pointer);
    GameObject * const objB = reinterpret_cast<GameObject*>(bodyB->GetUserData().pointer);
    if (objA)
        objA->postSolve(contact, impulse, fixtureB);
    if (objB)
        objB->postSolve(contact, impulse, fixtureA);
}

void ContactListener::BeginContact(b2Contact *contact)
{
    b2Fixture * const fixtureA = contact->GetFixtureA();
    b2Fixture * const fixtureB = contact->GetFixtureB();
    b2Body * const bodyA = fixtureA->GetBody();
    b2Body * const bodyB = fixtureB->GetBody();
    GameObject * const objA = reinterpret_cast<GameObject*>(bodyA->GetUserData().pointer);
    GameObject * const objB = reinterpret_cast<GameObject*>(bodyB->GetUserData().pointer);
    if (objA)
        objA->beginContact(contact, fixtureB);
    if (objB)
        objB->beginContact(contact, fixtureA);
}

void ContactListener::EndContact(b2Contact *contact)
{
    b2Fixture * const fixtureA = contact->GetFixtureA();
    b2Fixture * const fixtureB = contact->GetFixtureB();
    b2Body * const bodyA = fixtureA->GetBody();
    b2Body * const bodyB = fixtureB->GetBody();
    GameObject * const objA = reinterpret_cast<GameObject*>(bodyA->GetUserData().pointer);
    GameObject * const objB = reinterpret_cast<GameObject*>(bodyB->GetUserData().pointer);
    if (objA)
        objA->endContact(contact, fixtureB);
    if (objB)
        objB->endContact(contact, fixtureA);
}
