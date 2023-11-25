#pragma once

#include "Chess.h"

#include <unordered_set>

struct Direction;

class Game
{
public:
    Game();
    void selectPieceAt(int position);
    void releasePieceAt(int position);
    bool isPieceSelected() const;
    const Piece& selectedPiece() const;
    int lastMove() const { return m_lastMove; }
    // TODO: Make pieceId invisible to the interface?
    int selectedPieceId() const { return m_selectedPiece; }
    const Piece& getPiece(int pieceId) const;
    const std::unordered_set<int>& legalMoves() const;
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

    int m_selectedPiece;
    std::unordered_set<int> m_legalMoves[NUM_PIECES];
    Chess m_board;
    Color m_toMove;
    int m_lastMove;
};
