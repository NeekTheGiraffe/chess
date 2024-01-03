#pragma once

#include "SDLContext.h"
#include "Spritesheet.h"
#include "PieceSelector.h"
#include "Font.h"

#include <chess/Game.h>
#include <SDL3/SDL.h>

#include <unordered_set>

class Application
{
public:
    Application();
    ~Application();
    void loop();
private:
    int getBoardIndex(const SDL_FPoint& screenCoords) const;
    SDL_FRect boardDimensions() const;
    SDL_FRect boundingRect(int index) const;
    bool pointInCancelButton(const SDL_FPoint& pt) const;
    bool pointInPromotionButton(const SDL_FPoint& pt, Chess::Type& promoteTo) const;
    void renderWindow();

    SDLContext m_sdl;
    Spritesheet m_pieceSpritesheet;
    Spritesheet m_cancelButtonSprite;
    Font m_font;
    SDL_FPoint m_mousePos;
    SDL_Surface* m_windowIcon;

    Chess::Game m_game;
    PieceSelector m_selector;
};