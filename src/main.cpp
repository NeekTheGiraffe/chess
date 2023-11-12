#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>

#include <iostream>

int main()
{

    if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
    {
        std::cerr << "Error SDL initialization: " << SDL_GetError() << std::endl;
        return 1;
    }

    if (IMG_Init(IMG_INIT_JPG) == 0)
    {
        std::cerr << "Error IMG initialization: " << IMG_GetError() << std::endl;
        return 1;
    }

    SDL_Window* window = SDL_CreateWindow("My program", 800, 600, 0);
    if (window == nullptr)
    {
        std::cerr << "Error window creation: " << SDL_GetError() << std::endl;
        return 1;
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(window, nullptr, SDL_RENDERER_ACCELERATED);
    if (renderer == nullptr)
    {
        std::cerr << "Error renderer creation: " << SDL_GetError() << std::endl;
        return 1;
    }

    SDL_Surface* surface = IMG_Load("assets/giraffe.jpg");
    
    // std::cout << surface->clip_rect.x << " " << surface->clip_rect.y << " " << surface->clip_rect.w << " " << surface->clip_rect.h << std::endl;

    if (surface == nullptr)
    {
        std::cerr << "Error loading image: " << IMG_GetError() << std::endl;
        return 1;
    }

    SDL_FRect clip = { surface->clip_rect.x, surface->clip_rect.y, surface->clip_rect.w / 2.f, surface->clip_rect.h / 2.f };

    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_DestroySurface(surface);
    if (texture == nullptr)
    {
        std::cerr << "Error creating texture: " << SDL_GetError() << std::endl;
        return 1;
    }

    std::cout << "Hello world !" << std::endl;
    for (;;)
    {
        SDL_Event e;
        if (SDL_PollEvent(&e))
        {
            if (e.type == SDL_EVENT_QUIT)
                break;
        }

        SDL_RenderClear(renderer);
        SDL_RenderTexture(renderer, texture, nullptr, &clip);
        SDL_RenderPresent(renderer);
    }

    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    IMG_Quit();
    SDL_Quit();
}