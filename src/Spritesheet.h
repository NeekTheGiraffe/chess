#pragma once

#include <SDL3/SDL.h>

class Spritesheet
{
public:
    Spritesheet(SDL_Renderer* renderer, const char* path, int rows, int columns);
    ~Spritesheet();
    int render(SDL_Renderer* renderer, int i, const SDL_FRect& destRect);
    //float spriteWidth() const { return m_spriteWidth; }
    //float spriteHeight() const { return m_spriteHeight; }
private:
    SDL_Texture* m_texture;
    float m_spriteWidth, m_spriteHeight;
    int m_rows, m_cols;
};
