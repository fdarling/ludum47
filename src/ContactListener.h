#pragma once

// #include <Box2D/Dynamics/b2WorldCallbacks.h>
#include <box2d/b2_world_callbacks.h>

class ContactListener : public b2ContactListener
{
public:
    void BeginContact(b2Contact* contact) override;
    void EndContact(b2Contact* contact) override;
};
