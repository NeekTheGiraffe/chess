#include "SDLContext.h"
#include "utils.h"

SDLContext::SDLContext()
{
    ASSERT_SDL(SDL_Init(SDL_INIT_EVERYTHING) == 0, "SDL initialization");
    ASSERT_IMG(IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG) == (IMG_INIT_JPG | IMG_INIT_PNG), "IMG initialization");

    window = SDL_CreateWindow("My program", 800, 600, SDL_WINDOW_RESIZABLE);
    ASSERT_SDL(window != nullptr, "Window creation");

    renderer = SDL_CreateRenderer(window, nullptr, SDL_RENDERER_ACCELERATED);
    ASSERT_SDL(renderer != nullptr, "Renderer creation");
}

SDLContext::~SDLContext()
{
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    IMG_Quit();
    SDL_Quit();
}
