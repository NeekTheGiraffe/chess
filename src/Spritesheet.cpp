#include "Spritesheet.h"
#include "utils.h"

#include <SDL3_image/SDL_image.h>

Spritesheet::Spritesheet(SDL_Renderer* renderer, const char* path, int rows, int columns)
    : m_rows(rows), m_cols(columns)
{
    SDL_Surface* surface = IMG_Load(path);
    ASSERT_IMG(surface != nullptr, "Load image from " + std::string(path));

    m_spriteWidth = (float)surface->clip_rect.w / columns;
    m_spriteHeight = (float)surface->clip_rect.h / rows;

    m_texture = SDL_CreateTextureFromSurface(renderer, surface);
    ASSERT_SDL(m_texture != nullptr, "Texture creation");
    SDL_DestroySurface(surface);
}

Spritesheet::~Spritesheet()
{
    SDL_DestroyTexture(m_texture);
}

int Spritesheet::render(SDL_Renderer* renderer, int index, const SDL_FRect& destRect)
{
    int row = index / m_cols, col = index % m_cols;
    SDL_FRect srcRect = { col * m_spriteWidth, row * m_spriteHeight, m_spriteWidth, m_spriteHeight };
    return SDL_RenderTexture(renderer, m_texture, &srcRect, &destRect);
}
