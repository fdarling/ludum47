#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "globals.h"

#include <iostream>

SDL_Texture * LoadTexture(const char *filename)
{
    SDL_Surface * const surf = IMG_Load(filename);
    if (!surf)
    {
        std::cerr << "error: failed to load image \"" << filename << "\"" << std::endl;
        return NULL;
    }
    if (!renderer)
    {
        std::cerr << "error: no SDL_Renderer to create texture for \"" << filename << "\"" << std::endl;
    }
    SDL_Texture * const texture = SDL_CreateTextureFromSurface(renderer, surf);
    SDL_FreeSurface(surf);
    if (!texture)
    {
        std::cerr << "error: couldn't create texture for \"" << filename << "\"" << std::endl;
    }
    return texture;
}
