#pragma once

#include <SDL3/SDL.h>

#include <string>

struct SDLContext
{
    SDLContext(const std::string& windowName);
    ~SDLContext();

    SDL_Window* window;
    SDL_Renderer* renderer;
};
