#include "ContactListener.h"
#include "globals.h"

#include <box2d/b2_contact.h>
#include <box2d/b2_edge_shape.h>

// #include <iostream>

void ContactListener::PreSolve(b2Contact *contact, const b2Manifold *oldManifold)
{
    // find the player body/fixture
    b2Fixture *playerFixture = contact->GetFixtureA();
    b2Fixture *otherFixture  = contact->GetFixtureB();
    if (otherFixture->GetBody() == player.body)
        std::swap(playerFixture, otherFixture);
    if (playerFixture->GetBody() != player.body)
        return;

    if (otherFixture->GetBody() != world.groundBody)
    {
        // it must be a bullet!
        contact->SetEnabled(false); // make it so the bullets cannot collide with the player
    }
}

void ContactListener::PostSolve(b2Contact *contact, const b2ContactImpulse *impulse)
{
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

    // see if the other body/fixture is the ground
    if (otherFixture->GetBody() == world.groundBody)
    {
        player.beginContact(contact, otherFixture);
    }
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

    // see if the other body/fixture is the ground
    if (otherFixture->GetBody() == world.groundBody)
    {
        player.endContact(contact, otherFixture);
    }
}
