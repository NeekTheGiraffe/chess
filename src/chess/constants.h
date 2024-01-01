#pragma once

#include "Direction.h"

#include <vector>

namespace Chess
{
    inline const int BOARD_WIDTH = 8;
    inline const int NUM_SPACES = 64;
    inline const int NUM_PIECES = 32;

    inline const int A_FILE = 0;
    inline const int B_FILE = 1;
    inline const int C_FILE = 2;
    inline const int D_FILE = 3;
    inline const int E_FILE = 4;
    inline const int F_FILE = 5;
    inline const int G_FILE = 6;
    inline const int H_FILE = 7;

    inline const int RANK_1 = 0;
    inline const int RANK_2 = 1;
    inline const int RANK_3 = 2;
    inline const int RANK_4 = 3;
    inline const int RANK_5 = 4;
    inline const int RANK_6 = 5;
    inline const int RANK_7 = 6;
    inline const int RANK_8 = 7;

    inline const std::vector<Direction> rookDirections = { {0,1},{1,0},{0,-1},{-1,0} };
    inline const std::vector<Direction> bishopDirections = { {1,1},{1,-1},{-1,-1},{-1,1} };
    inline const std::vector<Direction> knightRelativePositions = { {1,2},{2,1},{1,-2},{2,-1},{-1,-2},{-2,-1},{-1,2},{-2,1} };
    inline const std::vector<Direction> queenDirections = { {1,0},{1,1},{0,1},{-1,1},{-1,0},{-1,-1},{0,-1},{1,-1} };
    inline const std::vector<Direction> kingRelativePositions = { {1,0},{1,1},{0,1},{-1,1},{-1,0},{-1,-1},{0,-1},{1,-1} };
}
