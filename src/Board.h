#pragma once

#include "constants.h"

#include <unordered_set>
#include <vector>
#include <string>

enum class Type
{
    PAWN,
    ROOK,
    KNIGHT,
    BISHOP,
    QUEEN,
    KING,
};
enum class Color
{
    WHITE,
    BLACK,
};

struct Piece
{
    int position;
    Type type;
    bool alive;
    bool hasMoved;
    Color color;
};

class Board
{
public:
    Board();
    Board(const std::string& s);
    Board(const Board& other);
    int getPieceId(int space) const;
    const Piece& getPiece(int pieceId) const;
    void movePiece(int pieceId, int space);
private:
    // m_board[i] == -1 to represent no piece
    // 0 <= m_board[i] < NUM_PIECES to represent a piece 
    int m_spaces[NUM_SPACES];
    Piece m_pieces[NUM_PIECES];
};
