#pragma once

#include <SDL2/SDL.h>

#include <box2d/b2_world.h>

#include "Rect.h"
// #include "Physics.h"
#include "World.h"
#include "Player.h"
#include "Camera.h"

static const int ZOOM_FACTOR = 3;
static const int SCREEN_WIDTH_RAW  = 800;
static const int SCREEN_HEIGHT_RAW = 600;
static const int SCREEN_WIDTH  = SCREEN_WIDTH_RAW/ZOOM_FACTOR;
static const int SCREEN_HEIGHT = SCREEN_HEIGHT_RAW/ZOOM_FACTOR;
static const int TILE_WIDTH  = 32;
static const int TILE_HEIGHT = 16;
static const int ATLAS_ROWS = 1;
static const int ATLAS_COLS = 2;

namespace Physics {

static const float METERS_PER_PIXEL =  0.0254; // 1 inch in meters
static const float PIXELS_PER_METER = 39.3701; // 1 meter in inches

extern b2World world;

} // namespace Physics

extern SDL_Window *window;
extern SDL_Renderer *renderer;
// extern Physics physics;
extern World world;
extern Player player;
extern Camera camera;

enum GameObjectType
{
    GAMEOBJECT_TYPE_NONE,
    GAMEOBJECT_TYPE_PLAYER,
    GAMEOBJECT_TYPE_BULLET,
    GAMEOBJECT_TYPE_GRENADE,
    GAMEOBJECT_TYPE_SPEEDBOOSTER,
    GAMEOBJECT_TYPE_SPRING,
    GAMEOBJECT_TYPE_LADDER,
    GAMEOBJECT_TYPE_MOVINGPLATFORM,
    GAMEOBJECT_TYPE_WORLD
};
