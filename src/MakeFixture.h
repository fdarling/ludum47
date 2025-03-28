#pragma once

#include <box2d/b2_math.h> // for b2Vec2
class b2Body; // forward declaration instead of #include <box2d/b2_body.h>
class b2Fixture; // forward declaration instead of #include <box2d/b2_fixture.h>

#include <vector>

void MakeFixture(b2Body *body, std::vector<b2Vec2> points);
