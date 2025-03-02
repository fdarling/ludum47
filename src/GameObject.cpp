#include "GameObject.h"

GameObject::GameObject() = default;

GameObject::~GameObject() = default;

void GameObject::preSolve(b2Contact *contact, const b2Manifold *oldManifold, b2Fixture *other)
{
    (void)contact;
    (void)oldManifold;
}

void GameObject::postSolve(b2Contact *contact, const b2ContactImpulse *impulse, b2Fixture *other)
{
    (void)contact;
    (void)impulse;
}

void GameObject::beginContact(b2Contact *contact, b2Fixture *other)
{
    (void)contact;
    (void)other;
}

void GameObject::endContact(b2Contact *contact, b2Fixture *other)
{
    (void)contact;
    (void)other;
}
