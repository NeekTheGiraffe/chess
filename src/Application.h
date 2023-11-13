#pragma once

#include "SDLContext.h"
#include "Spritesheet.h"
#include "Chess.h"

#include <SDL3/SDL.h>

class Application
{
public:
    Application();
    void loop();
private:
    int getBoardIndex(const SDL_FPoint& screenCoords) const;
    SDL_FRect boardDimensions() const;
    SDL_FRect boundingRect(int index) const;

    SDLContext m_sdl;
    Spritesheet m_spritesheet;
    Chess m_game;
};