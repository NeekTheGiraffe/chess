#pragma once

#include "Chess.h"

#include <unordered_set>

class Game
{
public:
    Game();
    void selectPieceAt(int position);
    void releasePieceAt(int position);
    bool isPieceSelected() const;
    const Piece& selectedPiece() const;
    // TODO: Make pieceId invisible to the interface?
    int selectedPieceId() const { return m_selectedPiece; }
    const Piece& getPiece(int pieceId) const;
    const std::unordered_set<int>& legalMoves() const;
private:
    int m_selectedPiece;
    std::unordered_set<int> m_legalMoves;
    Chess m_board;
};
