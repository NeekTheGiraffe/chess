#pragma once

#include <SDL3/SDL.h>

struct SDLContext
{
    SDLContext();
    ~SDLContext();

    SDL_Window* window;
    SDL_Renderer* renderer;
};
