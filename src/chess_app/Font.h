#pragma once

#include <SDL3_ttf/SDL_ttf.h>

#include <string>
#include <unordered_map>

struct Label
{
    SDL_Texture* texture;
    int w, h;
};

class Font
{
public:
    Font(const std::string& path, int ptsize);
    Font(const Font&) = delete;
    ~Font();
    bool createLabel(SDL_Renderer* renderer, const std::string& key, const std::string& text, SDL_Color color);
    bool render(SDL_Renderer* renderer, const std::string& key, SDL_FPoint topLeft);
    SDL_Point dimensions(const std::string& key) const;
private:
    TTF_Font* m_font;
    std::unordered_map<std::string, Label> m_labels;
};
