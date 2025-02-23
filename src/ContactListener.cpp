#include "ContactListener.h"
#include "globals.h"

#include <box2d/b2_contact.h>

// #include <iostream>

void ContactListener::BeginContact(b2Contact* contact)
{
    b2Body * const bodyA = contact->GetFixtureA()->GetBody();
    b2Body * const bodyB = contact->GetFixtureB()->GetBody();
    const bool aMatches = (bodyA == player.body && bodyB == world.groundBody && contact->GetFixtureA() == player.fixture);
    const bool bMatches = (bodyB == player.body && bodyA == world.groundBody && contact->GetFixtureB() == player.fixture);
    if (aMatches || bMatches)
    {
        // player->startContact();
        b2Fixture * const floor_fixture = aMatches ? contact->GetFixtureB() : contact->GetFixtureA();
        b2WorldManifold contactWorldManifold;
        contact->GetWorldManifold(&contactWorldManifold);

        // determine if we are standing on the colliding object
        if (contactWorldManifold.normal.y < 0.0 && contactWorldManifold.normal.y < -0.4)
        {
            player.standing_on.insert(floor_fixture);
            contact->SetFriction(0.5);
        }
        else
        {
            player.standing_on.erase(floor_fixture);
            contact->SetFriction(0.0);
        }

        // std::cout << contactWorldManifold.normal.x << "," << contactWorldManifold.normal.y << std::endl;
    }
}

void ContactListener::EndContact(b2Contact* contact)
{
    b2Body * const bodyA = contact->GetFixtureA()->GetBody();
    b2Body * const bodyB = contact->GetFixtureB()->GetBody();
    const bool aMatches = (bodyA == player.body && bodyB == world.groundBody && contact->GetFixtureA() == player.fixture);
    const bool bMatches = (bodyB == player.body && bodyA == world.groundBody && contact->GetFixtureB() == player.fixture);
    if (aMatches || bMatches)
    {
        // player->endContact();
        b2Fixture * const floor_fixture = aMatches ? contact->GetFixtureB() : contact->GetFixtureA();
        player.standing_on.erase(floor_fixture);
    }
}
