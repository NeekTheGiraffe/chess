#include "Application.h"

#include <SDL3_image/SDL_image.h>

#include <cstdlib>
#include <iostream>

#define ASSERT(cond, name, error_msg) if (!(cond)) \
    { \
        std::cerr << "Error during " << name << ": " << error_msg() << std::endl; \
        std::exit(1); \
    }
#define ASSERT_SDL(cond, name) ASSERT(cond, name, SDL_GetError)
#define ASSERT_IMG(cond, name) ASSERT(cond, name, IMG_GetError)

Application::Application()
{
    ASSERT_SDL(SDL_Init(SDL_INIT_EVERYTHING) == 0, "SDL initialization");
    ASSERT_IMG(IMG_Init(IMG_INIT_JPG) == IMG_INIT_JPG, "IMG initialization");

    m_window = SDL_CreateWindow("My program", 800, 600, SDL_WINDOW_RESIZABLE);
    ASSERT_SDL(m_window != nullptr, "Window creation");
    
    m_renderer = SDL_CreateRenderer(m_window, nullptr, SDL_RENDERER_ACCELERATED);
    ASSERT_SDL(m_renderer != nullptr, "Renderer creation");

    // For example drawings
    SDL_Surface* surface = IMG_Load("assets/giraffe.jpg");
    ASSERT_IMG(surface != nullptr, "Load surface");
    m_clip = { 0., 0., surface->clip_rect.w / 2.f, surface->clip_rect.h / 2.f };

    m_texture = SDL_CreateTextureFromSurface(m_renderer, surface);
    ASSERT_SDL(m_texture != nullptr, "Texture creation");
    SDL_DestroySurface(surface);
}

Application::~Application()
{
    SDL_DestroyTexture(m_texture);

    SDL_DestroyRenderer(m_renderer);
    SDL_DestroyWindow(m_window);
    IMG_Quit();
    SDL_Quit();
}

void Application::loop()
{
    SDL_Event e;
    bool mouseDown = false;
    bool rectSelected = false;
    SDL_FPoint mousePos;
    SDL_FPoint mouseOffset;
    SDL_FRect rect = { 0, 0, 40, 60 };

    for (;;)
    {
        if (SDL_PollEvent(&e))
        {
            if (e.type == SDL_EVENT_QUIT)
                return;
            if (e.type == SDL_EVENT_MOUSE_BUTTON_DOWN && e.button.button == SDL_BUTTON_LEFT)
            {
                mouseDown = true;
                if (SDL_PointInRectFloat(&mousePos, &rect))
                {
                    rectSelected = true;
                    mouseOffset = { mousePos.x - rect.x, mousePos.y - rect.y };
                }
            }
            if (e.type == SDL_EVENT_MOUSE_BUTTON_UP && e.button.button == SDL_BUTTON_LEFT)
            {
                mouseDown = false;
                rectSelected = false;
            }
            if (e.type == SDL_EVENT_MOUSE_MOTION)
                mousePos = { e.motion.x, e.motion.y };
        }

        if (rectSelected)
        {
            rect.x = mousePos.x - mouseOffset.x;
            rect.y = mousePos.y - mouseOffset.y;
        }

        SDL_SetRenderDrawColor(m_renderer, 255, 255, 255, 255);
        SDL_RenderClear(m_renderer);

        SDL_RenderTexture(m_renderer, m_texture, nullptr, &m_clip);

        SDL_SetRenderDrawColor(m_renderer, 0, 0, 255, 255);
        SDL_RenderFillRect(m_renderer, &rect);

        SDL_RenderPresent(m_renderer);
    }
}
