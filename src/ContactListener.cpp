#include "ContactListener.h"
#include "globals.h"

// #include <iostream>

static int count = 0;

void ContactListener::BeginContact(b2Contact* contact)
{
    b2Body * const bodyA = contact->GetFixtureA()->GetBody();
    b2Body * const bodyB = contact->GetFixtureB()->GetBody();
    if ((bodyA == player.body && bodyB == world.groundBody && contact->GetFixtureA() == player.fixture) ||
        (bodyB == player.body && bodyA == world.groundBody && contact->GetFixtureB() == player.fixture))
    {
        // player->startContact();
        count++;
        if (count == 1)
        {
            player.touchingGround = true;
            // player->fixture->SetFriction(2.0);
            // std::cout << "\tenabling friction " << count << std::endl;
        }
    }
}

void ContactListener::EndContact(b2Contact* contact)
{
    b2Body * const bodyA = contact->GetFixtureA()->GetBody();
    b2Body * const bodyB = contact->GetFixtureB()->GetBody();
    if ((bodyA == player.body && bodyB == world.groundBody && contact->GetFixtureA() == player.fixture) ||
        (bodyB == player.body && bodyA == world.groundBody && contact->GetFixtureB() == player.fixture))
    {
        // player->endContact();
        count--;
        if (count == 0)
        {
            player.touchingGround = false;
            // player->fixture->SetFriction(0.0);
            // std::cout << "\tDISabling friction " << count << std::endl;
        }
    }
}
