cmake_minimum_required(VERSION 3.7)

project(game)

find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(box2d REQUIRED)

#include_directories(${SDL2_INCLUDE_DIRS}) # TODO is this required? Doesn't seem like...

include(FindPkgConfig)
pkg_search_module(SDL2_gfx REQUIRED SDL2_gfx)

add_executable(${PROJECT_NAME}
    src/globals.cpp
    src/MakeFixture.cpp
    src/LoadTexture.cpp
    src/ContactListener.cpp
    src/DrawPhysics.cpp
    src/GameObject.cpp
    src/Player.cpp
    src/Bullet.cpp
    src/Ladder.cpp
    src/World.cpp
    src/Point.cpp
    src/Line.cpp
    src/Camera.cpp
    src/main.cpp
)

target_link_libraries(${PROJECT_NAME}
    ${SDL2_LIBRARIES}
    SDL2_image::SDL2_image
    SDL2_gfx
    box2d::box2d
)
