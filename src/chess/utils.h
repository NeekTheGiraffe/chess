#pragma once

#include "constants.h"

namespace Chess
{
    inline int rank(int position) { return position / BOARD_WIDTH; }
    inline int file(int position) { return position % BOARD_WIDTH; }
    inline int space(int rank, int file) { return rank * BOARD_WIDTH + file; }
    inline bool isInBounds(int rank, int file) { return rank >= 0 && rank < BOARD_WIDTH && file >= 0 && file < BOARD_WIDTH; }
}
