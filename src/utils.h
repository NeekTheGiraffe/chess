#pragma once

#include "constants.h"

#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>

#include <iostream>
#include <cstdlib>

#define ASSERT(cond, name, error_msg) if (!(cond)) \
    { \
        std::cerr << "Error during " << (name) << ": " << error_msg() << std::endl; \
        std::exit(1); \
    }
#define ASSERT_SDL(cond, name) ASSERT(cond, name, SDL_GetError)
#define ASSERT_IMG(cond, name) ASSERT(cond, name, IMG_GetError)

inline int rank(int position) { return position / BOARD_WIDTH; }
inline int file(int position) { return position % BOARD_WIDTH; }
inline int space(int rank, int file) { return rank * BOARD_WIDTH + file; }
inline bool isInBounds(int position) { return position >= 0 && position < NUM_SPACES; }
inline bool isInBounds(int rank, int file) { return rank >= 0 && rank < BOARD_WIDTH && file >= 0 && file < BOARD_WIDTH; }
