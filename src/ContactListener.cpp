#include "ContactListener.h"
#include "globals.h"

#include <box2d/b2_contact.h>
#include <box2d/b2_edge_shape.h>

// #include <iostream>

void ContactListener::PreSolve(b2Contact *contact, const b2Manifold *oldManifold)
{
    (void)oldManifold;

    // find the player body/fixture
    b2Fixture *playerFixture = contact->GetFixtureA();
    b2Fixture *otherFixture  = contact->GetFixtureB();
    if (otherFixture->GetBody() == player.body)
        std::swap(playerFixture, otherFixture);
    if (playerFixture->GetBody() != player.body)
        return;

    GameObject * const obj = reinterpret_cast<GameObject*>(otherFixture->GetBody()->GetUserData().pointer);
    if (!obj)
        return;

    if (obj->type() == GAMEOBJECT_TYPE_BULLET)
    {
        contact->SetEnabled(false); // make it so the bullets cannot collide with the player
    }
}

void ContactListener::PostSolve(b2Contact *contact, const b2ContactImpulse *impulse)
{
    (void)contact;
    (void)impulse;
}

void ContactListener::BeginContact(b2Contact *contact)
{
    // find the player body/fixture
    b2Fixture *playerFixture = contact->GetFixtureA();
    b2Fixture *otherFixture  = contact->GetFixtureB();
    if (otherFixture->GetBody() == player.body)
        std::swap(playerFixture, otherFixture);
    if (playerFixture->GetBody() != player.body)
        return;

    player.beginContact(contact, otherFixture);
}

void ContactListener::EndContact(b2Contact *contact)
{
    // find the player body/fixture
    b2Fixture *playerFixture = contact->GetFixtureA();
    b2Fixture *otherFixture  = contact->GetFixtureB();
    if (otherFixture->GetBody() == player.body)
        std::swap(playerFixture, otherFixture);
    if (playerFixture->GetBody() != player.body)
        return;

    player.endContact(contact, otherFixture);
}
