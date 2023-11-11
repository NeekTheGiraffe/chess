#include <SDL3/SDL.h>

#include <iostream>

int main() {

    if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
        std::cout << "Error SDL initialization: " << SDL_GetError() << std::endl;
        return 1;
    }

    SDL_Window* window = SDL_CreateWindow("My program", 800, 600, 0);
    if (window == nullptr) {
        std::cout << "Error window creation" << std::endl;
        return 2;
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(window, nullptr, SDL_RENDERER_ACCELERATED);
    if (renderer == nullptr) {
        std::cout << "Error renderer creation" << std::endl;
        return 3;
    }

    std::cout << "Hello world !" << std::endl;
    for (;;) {
        SDL_Event e;
        if (SDL_WaitEvent(&e)) {
            if (e.type == SDL_EVENT_QUIT)
                break;
        }
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}