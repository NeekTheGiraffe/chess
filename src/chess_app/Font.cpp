#include "Font.h"

#include <cassert>

Font::Font(const std::string& path, int ptsize)
{
    m_font = TTF_OpenFont(path.c_str(), ptsize);
}

Font::~Font()
{
    TTF_CloseFont(m_font);
    for (auto& entry : m_labels)
        SDL_DestroyTexture(entry.second.texture);
}

bool Font::createLabel(SDL_Renderer* renderer, const std::string& key, const std::string& text, SDL_Color color)
{
    if (m_labels.count(key) > 0)
        return false;
    SDL_Surface* surface = TTF_RenderText_Solid(m_font, text.c_str(), color);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    m_labels[key] = Label{ texture, surface->w, surface->h };
    SDL_DestroySurface(surface);
    return true;
}

bool Font::render(SDL_Renderer* renderer, const std::string& key, SDL_FPoint topLeft)
{
    if (m_labels.count(key) == 0)
        return false;
    Label& label = m_labels[key];
    SDL_FRect dest = { topLeft.x, topLeft.y, label.w, label.h };
    SDL_RenderTexture(renderer, label.texture, nullptr, &dest);
    return true;
}

SDL_Point Font::dimensions(const std::string& key) const
{
    assert(m_labels.count(key) > 0);
    const Label& l = m_labels.at(key);
    return { l.w, l.h };
}
