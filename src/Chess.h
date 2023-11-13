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

struct Piece
{
    int position;
    Type type;
    bool alive;
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
    int m_board[NUM_SPACES];
    Piece m_pieces[32];
};
