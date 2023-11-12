#pragma once

#include <SDL3/SDL.h>

class Application
{
public:
    Application();
    ~Application();
    void loop();
private:
    SDL_Window* m_window;
    SDL_Renderer* m_renderer;

    // Example
    SDL_FRect m_clip;
    SDL_Texture* m_texture;
};