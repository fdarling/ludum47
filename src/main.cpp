#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif  // __EMSCRIPTEN__

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <box2d/b2_body.h>

#include <cstdio>
#include <cstring>
#include <stdint.h>
#include <iostream>
#include <filesystem>

#include "Line.h"
#include "globals.h"

static Uint32 _lastTime;

static Player *player;

static bool main_loop()
{
    // process queued events
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        if (event.type == SDL_QUIT || (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE))
        {
            return false;
        }
        else if (event.type == SDL_KEYDOWN)
        {
            switch (event.key.keysym.sym)
            {
                case SDLK_SPACE:
                {
                    player->shootBullet();
                }
                break;

                case SDLK_g:
                {
                    player->shootGrenade();
                }
                break;
                // case SDLK_LEFT:
                // camera.pos.x -= 1;
                // break;
                // case SDLK_RIGHT:
                // camera.pos.x += 1;
                // break;
                // case SDLK_w:
                // player->jump();
                // break;
            }
        }
    }
    // process buttons being held
    {
        bool walking_left_or_right = false;
        const Uint8 * const keys = SDL_GetKeyboardState(NULL);
             if ((keys[SDL_SCANCODE_D] && !keys[SDL_SCANCODE_A])
              || (keys[SDL_SCANCODE_RIGHT] && !keys[SDL_SCANCODE_LEFT]))
        {
            player->walkRight();
            walking_left_or_right = true;
        }
        else if ((keys[SDL_SCANCODE_A] && !keys[SDL_SCANCODE_D])
              || (keys[SDL_SCANCODE_LEFT] && !keys[SDL_SCANCODE_RIGHT]))
        {
            player->walkLeft();
            walking_left_or_right = true;
        }
        if (keys[SDL_SCANCODE_W] || keys[SDL_SCANCODE_UP])
        {
            if (player->isClimbing())
                player->climbUp();
            else
                player->jump();
        }
        else if (keys[SDL_SCANCODE_S] || keys[SDL_SCANCODE_DOWN])
        {
            if (player->isClimbing())
                player->climbDown();
        }
        else
        {
            if (player->isClimbing())
            {
                b2Vec2 vel = player->body->GetLinearVelocity();
                vel.y = 0.0;
                if (!walking_left_or_right)
                    vel.x = 0.0;
                player->body->SetLinearVelocity(vel);
            }
        }
        player->setJetpack(keys[SDL_SCANCODE_LSHIFT] || keys[SDL_SCANCODE_RSHIFT]);
        player->setGrappling(keys[SDL_SCANCODE_LCTRL] || keys[SDL_SCANCODE_RCTRL]);
    }

    // sychronize the camera to the player
    camera.pos.x = -SCREEN_WIDTH /2 + player->rect.x + player->rect.w/2;
    camera.pos.y = -SCREEN_HEIGHT/2 + player->rect.y + player->rect.h/2;

    // clear screen
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    // draw background
    world.draw(camera.pos);

    // draw player
    player->draw(camera.pos);

    // process physics (done here because of debug output)
    // Physics::world.DebugDraw();
    {
        const Uint32 newTime = SDL_GetTicks();
        const float timeStep = static_cast<float>(newTime - _lastTime) / 1000.0;
        _lastTime = newTime;
        int32 velocityIterations = 6;
        int32 positionIterations = 2;

        world.advance(timeStep);
        player->advance();

        Physics::world.Step(timeStep, velocityIterations, positionIterations);
    }

    // synchronize player to physics body
    {
        const b2Vec2 position = player->body->GetPosition();
        player->rect.setCenterX(position.x * Physics::PIXELS_PER_METER);
        player->rect.setCenterY(position.y * Physics::PIXELS_PER_METER);
    }

    // show changes
    SDL_RenderPresent(renderer);

    // yield CPU
    SDL_Delay(1);

    return true;
}

#ifdef __EMSCRIPTEN__
static void main_loop_wrapper(void *)
{
    main_loop();
}
#endif

int main(int, char **)
{
    /*{
        // const Line l1(Point(15, 10), Point(49, 25));
        // const Line l2(Point(29,  5), Point(32, 32));
        // const Line l2(Point(29,  5), Point(29, 32));
        const Line l1(Point(120, 720), Point(120, 750));
        const Line l2(Point(  0, 800), Point(900, 400));
        bool a, b;
        const Point pt = l1.intersect(l2, &a, &b);
        std::cout << pt << ' ' << a << ' ' << b << std::endl; // should say (30, 17)
        // return 0;
    }*/

    {
        const std::string TEST_DIR = "assets";
        const std::filesystem::path current_directory = std::filesystem::current_path();
        const std::filesystem::path current_directory_parent = current_directory.parent_path();
        const std::filesystem::path location1 = current_directory / TEST_DIR;
        const std::filesystem::path location2 = current_directory_parent / TEST_DIR;

        if (!std::filesystem::is_directory(location1))
        {
            std::filesystem::current_path(current_directory_parent);
            if (!std::filesystem::is_directory(location2))
            {
                fprintf(stderr, "couldn't find the \"%s\" directory at either of the following locations:\n\"%s\"\n\"%s\"\n", TEST_DIR.c_str(), location1.c_str(), location2.c_str());
                return -1;
            }
        }
    }

    // initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        fprintf(stderr, "error: SDL_Init failed: %s\n", SDL_GetError());
        return -1;
    }
    // initialize image loading library
    const int imgFlags = IMG_INIT_PNG;
    if (!(IMG_Init(imgFlags) & imgFlags))
    {
        printf("error: IMG_Init failed: %s\n", IMG_GetError());
    }
    // create window
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "0");
    window = SDL_CreateWindow(
        "Ludum Dare 47",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        SCREEN_WIDTH_RAW,
        SCREEN_HEIGHT_RAW,
        0
    );
    if (!window)
    {
        fprintf(stderr, "error: SDL_CreateWindow failed: %s\n", SDL_GetError());
        return -1;
    }
    // create renderer
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED|SDL_RENDERER_PRESENTVSYNC);
    if (!renderer)
    {
        fprintf(stderr, "error: SDL_CreateRenderer failed: %s\n", SDL_GetError());
        return -1;
    }
    SDL_RenderSetScale(renderer, ZOOM_FACTOR, ZOOM_FACTOR);

    if (!world.init())
    {
        fprintf(stderr, "error: world init failed!\n");
        return -1;
    }
    world.addChild(std::static_pointer_cast<GameObject>(std::shared_ptr<Player>(player = new Player())));
    if (!player->init())
    {
        fprintf(stderr, "error: player init failed!\n");
        return -1;
    }

    // run main loop
    _lastTime = SDL_GetTicks();
#ifdef __EMSCRIPTEN__
    emscripten_set_main_loop_arg(main_loop_wrapper, NULL, -1, 1);
#else
    while (main_loop())
    {
    }
#endif // __EMSCRIPTEN__

    // clean up
    world.quit();
    if (window)
        SDL_DestroyWindow(window);
    IMG_Quit();
    SDL_Quit();
    return 0;
}
