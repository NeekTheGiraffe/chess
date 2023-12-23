#pragma once

#include "Board.h"
#include "utils.h"
#include "Direction.h"

#include <unordered_set>

class Analysis;

class Game
{
public:
    Game();
    Game(const Board& board, Color toMove, int lastMove);
    void movePiece(int pieceId, int dest);
    int lastMove() const { return m_lastMove; }
    int getPieceId(int position) const;
    const Piece& getPiece(int pieceId) const;
    Color toMove() const { return m_toMove; };

    const std::unordered_set<int>& legalMoves(int pieceId) const;
private:
    void calculateAllLegalMoves();
    void calculateLegalMoves(int pieceId, const Analysis& a);
    void pawnLegalMoves(const Piece& p, std::unordered_set<int>& moves, const Analysis& a);
    void kingLegalMoves(const Piece& p, std::unordered_set<int>& moves, const Analysis& a);
    void directionalLegalMoves(
        const Piece& p,
        const std::vector<Direction>& directions,
        std::unordered_set<int>& moves,
        const Analysis& a
    );
    void absoluteLegalMoves(
        const Piece& p,
        const std::vector<Direction>& relativePositions,
        std::unordered_set<int>& moves,
        const Analysis& a
    );

    std::unordered_set<int> m_legalMoves[NUM_PIECES];
    Board m_board;
    Color m_toMove;
    int m_lastMove;
};
