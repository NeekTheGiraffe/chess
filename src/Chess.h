#pragma once

#include "constants.h"

#include <unordered_set>
#include <vector>

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
struct Direction;

class Chess
{
public:
    Chess();
    int getPieceId(int space) const;
    const Piece& getPiece(int pieceId) const;
    void movePiece(int pieceId, int space);
    std::unordered_set<int> legalMoves(int pieceId) const;
private:
    std::unordered_set<int> pawnLegalMoves(const Piece& p) const;
    std::unordered_set<int> rookLegalMoves(const Piece& p) const;
    std::unordered_set<int> bishopLegalMoves(const Piece& p) const;
    std::unordered_set<int> knightLegalMoves(const Piece& p) const;
    std::unordered_set<int> queenLegalMoves(const Piece& p) const;
    std::unordered_set<int> kingLegalMoves(const Piece& p) const;
    std::unordered_set<int> directionalLegalMoves(const Piece& p, const std::vector<Direction>& directions) const;
    std::unordered_set<int> absoluteLegalMoves(const Piece& p, const std::vector<Direction>& relativePositions) const;

    // m_board[i] == -1 to represent no piece
    // 0 <= m_board[i] < NUM_PIECES to represent a piece 
    int m_board[NUM_SPACES];
    Piece m_pieces[NUM_PIECES];
};
