#pragma once

#include <box2d/b2_body.h>
#include <box2d/b2_fixture.h>

#include <vector>

void MakeFixture(b2Body *body, std::vector<b2Vec2> points);
void MakeFixture(b2Body *body, b2FixtureDef fixtureDef, std::vector<b2Vec2> points);
