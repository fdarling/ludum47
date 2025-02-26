#include "World.h"
#include "LoadTexture.h"
#include "MakeFixture.h"
#include "globals.h"

#include <SDL2/SDL.h>

World::World() : groundBody(NULL), /*_atlas(NULL), _bg(NULL), */_lastTime(SDL_GetTicks())
{
    // make body
    b2BodyDef groundBodyDef;
    // groundBodyDef.position.Set(0.0*Physics::METERS_PER_PIXEL, 0.0*Physics::METERS_PER_PIXEL);
    groundBody = Physics::world.CreateBody(&groundBodyDef);

    // add ground
    std::vector<b2Vec2> points;
    points.emplace_back(  0.0, 800.0);
    points.emplace_back(500.0, 800.0);
    points.emplace_back(800.0, 800.0);
    points.emplace_back(800.0, 900.0);
    points.emplace_back(  0.0, 900.0);
    MakeFixture(groundBody, points);
    
    // add small platform #1
    points.clear();
    points.emplace_back(0.0, 784.0);
    points.emplace_back(0.0, 768.0);
    points.emplace_back(32.0, 768.0);
    points.emplace_back(32.0, 784.0);
    MakeFixture(groundBody, points);

    // add small platform #2
    points.clear();
    points.emplace_back(64.0, 784.0-16);
    points.emplace_back(64.0, 768.0-16);
    points.emplace_back(96.0, 768.0-16);
    points.emplace_back(96.0, 784.0-16);
    MakeFixture(groundBody, points);

    // add small platform #3
    points.clear();
    points.emplace_back(128.0, 752.0);
    points.emplace_back(128.0, 736.0);
    points.emplace_back(160.0, 736.0);
    points.emplace_back(160.0, 752.0);
    MakeFixture(groundBody, points);

    // add small platform #4
    points.clear();
    points.emplace_back(160.0, 720.0);
    points.emplace_back(160.0, 704.0);
    points.emplace_back(192.0, 704.0);
    points.emplace_back(192.0, 720.0);
    MakeFixture(groundBody, points);
    
    // add large platform #5
    points.clear();
    points.emplace_back(160.0, 720.0 - 64.0);
    points.emplace_back(160.0, 704.0 - 64.0);
    points.emplace_back(288.0, 704.0 - 64.0);
    points.emplace_back(288.0, 720.0 - 64.0);
    MakeFixture(groundBody, points);

    // add hill #1
    points.clear();
    points.emplace_back(224.0, 800.0);
    points.emplace_back(352.0, 736.0);
    points.emplace_back(480.0, 800.0);
    MakeFixture(groundBody, points);

    // add hill #2
    points.clear();
    points.emplace_back(576.0, 800.0);
    points.emplace_back(576.0, 784.0);
    points.emplace_back(704.0, 784.0);
    points.emplace_back(704.0, 800.0);
    MakeFixture(groundBody, points);

    // show graphical debugging
    uint32 flags = 0;
    flags |= b2Draw::e_shapeBit;
    flags |= b2Draw::e_jointBit;
    // flags |= b2Draw::e_aabbBit;
    flags |= b2Draw::e_pairBit;
    flags |= b2Draw::e_centerOfMassBit;
    _debugDraw.SetFlags(flags);
    Physics::world.SetDebugDraw(&_debugDraw);

    // monitor contact
    Physics::world.SetContactListener(&_contactListener);
}

World::~World()
{
    Physics::world.SetDebugDraw(nullptr);
    /*if (_atlas)
        SDL_DestroyTexture(_atlas);
    if (_bg)
        SDL_DestroyTexture(_bg);*/
}

bool World::init()
{
    /*_atlas = LoadTexture("assets/stairs.png");
    _bg = LoadTexture("assets/background_smaller.png");
    return _atlas && _bg;*/
    return true;
}

void World::quit()
{
    /*SDL_DestroyTexture(_bg);
    SDL_DestroyTexture(_atlas);*/
}

void World::advance()
{
    const Uint32 newTime = SDL_GetTicks();
    const float timeStep = static_cast<float>(newTime - _lastTime) / 1000.0;
    _lastTime = newTime;
    int32 velocityIterations = 6;
    int32 positionIterations = 2;

    Physics::world.Step(timeStep, velocityIterations, positionIterations);
}

void World::draw(const Point &offset)
{
    SDL_SetRenderDrawColor(renderer, 64, 64, 64, 128);
    for (int y = -TILE_HEIGHT; y < SCREEN_HEIGHT + TILE_HEIGHT; y += TILE_HEIGHT)
    {
        const int y_offset = offset.y%TILE_HEIGHT;
        SDL_RenderDrawLine(renderer, 0, y-y_offset, SCREEN_WIDTH-1, y-y_offset);
    }
    for (int x = -TILE_WIDTH; x < SCREEN_WIDTH + TILE_WIDTH; x += TILE_WIDTH)
    {
        const int x_offset = offset.x%TILE_WIDTH;
        SDL_RenderDrawLine(renderer, x-x_offset, 0, x-x_offset, SCREEN_HEIGHT-1);
    }
    /*static const int LAST_ROW = SCREEN_HEIGHT/TILE_HEIGHT;
    static const int LAST_COL = SCREEN_WIDTH/TILE_WIDTH;
    for (int row = 0; row < LAST_ROW; row++)
    {
        for (int col = 0; col < LAST_COL; col++)
        {
            const int tileIndex = (row+col)%2;
            const int atlasRow = tileIndex/ATLAS_COLS;
            const int atlasCol = tileIndex%ATLAS_COLS;
            const SDL_Rect src = {atlasCol*TILE_WIDTH, atlasRow*TILE_HEIGHT, TILE_WIDTH, TILE_HEIGHT};
            const SDL_Rect dst = {col*TILE_WIDTH - offset.x, row*TILE_HEIGHT - offset.y, TILE_WIDTH, TILE_HEIGHT};
            SDL_RenderCopy(renderer, _atlas, &src, &dst);
        }
    }*/
    // const SDL_Rect src = {atlasCol*TILE_WIDTH, atlasRow*TILE_HEIGHT, TILE_WIDTH, TILE_HEIGHT};
    // static const int BG_WIDTH  = 1280;
    // static const int BG_HEIGHT = 1920;
    /*static const int BG_WIDTH  = 800;
    static const int BG_HEIGHT = 1200;
    const SDL_Rect rects[] =
    {
        {-BG_WIDTH-offset.x,  BG_HEIGHT/3-offset.y, BG_WIDTH, BG_HEIGHT},
        {         -offset.x,             -offset.y, BG_WIDTH, BG_HEIGHT},
        { BG_WIDTH-offset.x, -BG_HEIGHT/3-offset.y, BG_WIDTH, BG_HEIGHT}
    };
    for (std::size_t i = 0; i < sizeof(rects)/sizeof(rects[0]); i++)
        SDL_RenderCopy(renderer, _bg, NULL, rects + i);*/
}
