#pragma once

#include "Direction.h"

#include <vector>

namespace Chess
{
    inline const int BOARD_WIDTH = 8;
    inline const int NUM_SPACES = 64;
    inline const int NUM_PIECES = 32;

    inline const std::vector<Direction> rookDirections = { {0,1},{1,0},{0,-1},{-1,0} };
    inline const std::vector<Direction> bishopDirections = { {1,1},{1,-1},{-1,-1},{-1,1} };
    inline const std::vector<Direction> knightRelativePositions = { {1,2},{2,1},{1,-2},{2,-1},{-1,-2},{-2,-1},{-1,2},{-2,1} };
    inline const std::vector<Direction> queenDirections = { {1,0},{1,1},{0,1},{-1,1},{-1,0},{-1,-1},{0,-1},{1,-1} };
    inline const std::vector<Direction> kingRelativePositions = { {1,0},{1,1},{0,1},{-1,1},{-1,0},{-1,-1},{0,-1},{1,-1} };
}
