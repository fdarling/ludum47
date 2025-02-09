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
        
        const bool blocked_before = !(player.blocked_left_by.empty() && player.blocked_right_by.empty());

        // determine if we are standing on the colliding object
        if (contactWorldManifold.normal.y < -0.4)
            player.standing_on.insert(floor_fixture);
        else
            player.standing_on.erase(floor_fixture);

        // determine if we are being blocked horizontally by the colliding object
        if (contactWorldManifold.normal.x >= 0.9)
            player.blocked_left_by.insert(floor_fixture);
        else
            player.blocked_left_by.erase(floor_fixture);
        if (contactWorldManifold.normal.x <= -0.9)
            player.blocked_right_by.insert(floor_fixture);
        else
            player.blocked_right_by.erase(floor_fixture);

        const bool touching_before = player.touchingGround;
        player.touchingGround = !player.standing_on.empty();
        const bool touching_after = player.touchingGround;
        const bool blocked_after = !(player.blocked_left_by.empty() && player.blocked_right_by.empty());
        const bool blocked_changed = (blocked_after != blocked_before);
        const bool jumping_changed = (touching_after != touching_before);
        // if (jumping_changed)
        // {
            // std::cout << "Touching ground?: " << (touching_after ? "yes" : "no") << std::endl;
        // }
        // if (blocked_changed)
            // std::cout << "Blocked?: " << (blocked_after ? "yes" : "no") << std::endl;
            // std::cout << "Blocked?: L = " << (!player.blocked_left_by.empty() ? "yes" : "no") << ", R = " << (!player.blocked_right_by.empty() ? "yes" : "no") << std::endl;
        const bool needs_friction = player.touchingGround;
        const float chosen_friction = needs_friction ? 2.0 : 0.0;
        const float old_friction = player.fixture->GetFriction();
        if (chosen_friction != old_friction)
        {
            // std::cout << "changing friction to: " << chosen_friction << std::endl;
            // player.fixture->SetFriction(chosen_friction);
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
        const bool blocked_before = !(player.blocked_left_by.empty() && player.blocked_right_by.empty());
        player.standing_on.erase(floor_fixture);
        player.blocked_left_by.erase(floor_fixture);
        player.blocked_right_by.erase(floor_fixture);
        const bool touching_before = player.touchingGround;
        player.touchingGround = !player.standing_on.empty();
        const bool touching_after = player.touchingGround;
        const bool blocked_after = !(player.blocked_left_by.empty() && player.blocked_right_by.empty());
        const bool blocked_changed = (blocked_after != blocked_before);
        const bool jumping_changed = (touching_after != touching_before);
        // if (jumping_changed)
        // {
            // std::cout << "Touching ground?: " << (touching_after ? "yes" : "no") << std::endl;
        // }
        // if (blocked_changed)
            // std::cout << "Blocked?: L = " << (!player.blocked_left_by.empty() ? "yes" : "no") << ", R = " << (!player.blocked_right_by.empty() ? "yes" : "no") << std::endl;
        const bool needs_friction = player.touchingGround;
        const float chosen_friction = needs_friction ? 2.0 : 0.0;
        const float old_friction = player.fixture->GetFriction();
        if (chosen_friction != old_friction)
        {
            // std::cout << "changing friction to: " << chosen_friction << std::endl;
            // player.fixture->SetFriction(chosen_friction);
        }
    }
}
