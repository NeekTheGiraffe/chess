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
    std::unordered_set<int> calculateLegalMoves(int pieceId) const;
    std::unordered_set<int> pawnLegalMoves(const Piece& p) const;
    std::unordered_set<int> rookLegalMoves(const Piece& p) const;
    std::unordered_set<int> bishopLegalMoves(const Piece& p) const;
    std::unordered_set<int> knightLegalMoves(const Piece& p) const;
    std::unordered_set<int> queenLegalMoves(const Piece& p) const;
    std::unordered_set<int> kingLegalMoves(const Piece& p) const;
    std::unordered_set<int> directionalLegalMoves(const Piece& p, const std::vector<Direction>& directions) const;
    std::unordered_set<int> absoluteLegalMoves(const Piece& p, const std::vector<Direction>& relativePositions) const;

    int m_selectedPiece;
    std::unordered_set<int> m_legalMoves;
    Chess m_board;
    Color m_toMove;
    int m_lastMove;
};
