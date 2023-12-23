#pragma once

#include "constants.h"

#include <unordered_set>
#include <vector>
#include <string>

namespace Chess
{
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

    inline Color opposite(Color color)
    {
        return color == Color::WHITE ? Color::BLACK : Color::WHITE;
    }

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
        int getPieceId(int space) const;
        const Piece& getPiece(int pieceId) const;
        void movePiece(int pieceId, int space);
        const Piece& whiteKing() const;
        const Piece& blackKing() const;
    private:
        // m_board[i] == -1 to represent no piece
        // 0 <= m_board[i] < NUM_PIECES to represent a piece 
        int m_spaces[NUM_SPACES];
        Piece m_pieces[NUM_PIECES];
        int m_whiteKingId;
        int m_blackKingId;
    };
}
