#pragma once

#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <SDL3_ttf/SDL_ttf.h>

#include <iostream>
#include <cstdlib>

#define ASSERT(cond, name, error_msg) if (!(cond)) \
    { \
        std::cerr << "Error during " << (name) << ": " << error_msg() << std::endl; \
        std::exit(1); \
    }
#define ASSERT_SDL(cond, name) ASSERT(cond, name, SDL_GetError)
#define ASSERT_IMG(cond, name) ASSERT(cond, name, IMG_GetError)
#define ASSERT_TTF(cond, name) ASSERT(cond, name, TTF_GetError)
