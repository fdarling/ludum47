#include "globals.h"

namespace Physics {

static b2Vec2 gravity(0.0, 9.8);
b2World world(gravity);

} // namespace Physics

SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;
// Physics physics;
World world;
Camera camera;
Player player;
