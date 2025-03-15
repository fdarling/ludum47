#include "World.h"
#include "MovingPlatform.h"
#include "Ladder.h"
#include "Spring.h"
#include "SpeedBooster.h"
#include "LoadTexture.h"
#include "MakeFixture.h"
#include "globals.h"

#include <SDL2/SDL.h>

#include <box2d/b2_body.h>

#include <algorithm>

World::World() : groundBody(NULL), /*_atlas(NULL), _bg(NULL), */_lastTime(SDL_GetTicks())
{
    // make body
    b2BodyDef groundBodyDef;
    // groundBodyDef.position.Set(0.0*Physics::METERS_PER_PIXEL, 0.0*Physics::METERS_PER_PIXEL);
    groundBody = Physics::world.CreateBody(&groundBodyDef);
    groundBody->GetUserData().pointer = reinterpret_cast<uintptr_t>(this);

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
    points.emplace_back(120.0, 756.0);
    points.emplace_back(120.0, 744.0);
    points.emplace_back(144.0, 744.0);
    points.emplace_back(144.0, 756.0);
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

    // add a ladder
    addChild(new Ladder(b2Vec2(32+288.0, 576.0 - 64.0), b2Vec2(32+304.0, 784.0 - 92.0)));

    addChild(new MovingPlatform(b2Vec2(320.0 - 128.0, 512.0 - 96.0), b2Vec2(336.0 - 64.0, 692.0 - 96.0)));

    // add a spring
    addChild(new Spring(b2Vec2(192.0, 800.0), b2Vec2(192.0 + 24, 800.0 - 12)));

    addChild(new SpeedBooster(b2Vec2(192.0 - 256.0*3, 800.0), b2Vec2(224.0 - 256.0, 792.0)));

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

int World::type() const
{
    return GAMEOBJECT_TYPE_WORLD;
}

bool World::init()
{
    // I'm not sure if you are intentionally avoiding it, but check out IMG_LoadTexture() from SDL2_image:
    // IMG_LoadTexture(renderer, "assets/tiles.png");
    _atlas = LoadTexture("assets/tiles.png");
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

    for (GameObject *child : _children)
    {
        child->advance(timeStep);
    }
    performDeletions();

    Physics::world.Step(timeStep, velocityIterations, positionIterations);
}

void World::addChild(GameObject *child)
{
    _children.push_back(child);
}

void World::removeChild(GameObject *child)
{
    std::vector<GameObject*>::iterator it = std::find(_children.begin(), _children.end(), child);
    if (it != _children.end())
        _children.erase(it);
}

void World::deleteLater(GameObject *child)
{
    _deleteLater.push_back(child);
}

void World::performDeletions()
{
    for (GameObject *child : _deleteLater)
    {
        std::vector<GameObject*>::iterator it = std::find(_children.begin(), _children.end(), child);
        if (it != _children.end())
        {
            _children.erase(it);
            delete child;
        }
    }
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
    static const int tile_size = 12;
    SDL_Rect src = {6 * tile_size, tile_size, tile_size, tile_size};
    SDL_Rect dst = {0, 0, tile_size, tile_size};
    for (int row = 0; row < (SCREEN_HEIGHT / tile_size) + 3; row++)
    {
        // dst.y = (row * tile_size) - offset.y;
        dst.y = ((row - 1) * tile_size) - (offset.y % tile_size);
        for (int col = 0; col < (SCREEN_WIDTH / tile_size) + 3; col++)
        {
            // dst.x = (col * tile_size) - offset.x;
            dst.x = ((col - 1) * tile_size) - (offset.x % tile_size);
            SDL_RenderCopy(renderer, _atlas, &src, &dst);
        }
    }

    // Temporary test render:
    dst.x = 192 - offset.x;
    dst.y = 788 - offset.y;
    src.x = 14 * tile_size;
    src.y = 15 * tile_size;
    SDL_RenderCopy(renderer, _atlas, &src, &dst);
    dst.x = 192 - offset.x + tile_size;
    SDL_RenderCopy(renderer, _atlas, &src, &dst);
    src.x = 0;
    src.y = 1 * tile_size;
    dst.x = 120 - offset.x;
    dst.y = 744 - offset.y;
    SDL_RenderCopy(renderer, _atlas, &src, &dst);
    dst.x = 120 - offset.x + tile_size;
    SDL_RenderCopy(renderer, _atlas, &src, &dst);

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



















