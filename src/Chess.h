#pragma once

#include "constants.h"

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
    Color color;
};

class Chess
{
public:
    Chess();
    ~Chess();
    int getPieceId(int space) const;
    const Piece& getPiece(int pieceId) const;
    void movePiece(int pieceId, int space);
private:
    // m_board[i] == -1 to represent no piece
    // 0 <= m_board[i] < NUM_PIECES to represent a piece 
    int m_board[NUM_SPACES];
    Piece m_pieces[NUM_PIECES];
};
