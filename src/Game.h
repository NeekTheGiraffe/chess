#pragma once

#include "Board.h"

#include <unordered_set>

struct Direction;

class Game
{
public:
    Game();
    void movePiece(int pieceId, int dest);
    int lastMove() const { return m_lastMove; }
    int getPieceId(int position) const;
    const Piece& getPiece(int pieceId) const;
    Color toMove() const { return m_toMove; };

    const std::unordered_set<int>& legalMoves(int pieceId) const;
private:
    void calculateAllLegalMoves();
    void calculateLegalMoves(int pieceId);
    void pawnLegalMoves(const Piece& p, std::unordered_set<int>& moves);
    void rookLegalMoves(const Piece& p, std::unordered_set<int>& moves);
    void bishopLegalMoves(const Piece& p, std::unordered_set<int>& moves);
    void knightLegalMoves(const Piece& p, std::unordered_set<int>& moves);
    void queenLegalMoves(const Piece& p, std::unordered_set<int>& moves);
    void kingLegalMoves(const Piece& p, std::unordered_set<int>& moves);
    void directionalLegalMoves(const Piece& p, const std::vector<Direction>& directions, std::unordered_set<int>& moves);
    void absoluteLegalMoves(const Piece& p, const std::vector<Direction>& relativePositions, std::unordered_set<int>& moves);

    std::unordered_set<int> m_legalMoves[NUM_PIECES];
    Board m_board;
    Color m_toMove;
    int m_lastMove;
};
