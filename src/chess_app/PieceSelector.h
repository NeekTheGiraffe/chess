#pragma once

#include <chess/Game.h>

class PieceSelector
{
public:
    PieceSelector(Chess::Game& game) : m_game(game), m_selectedPiece(-1), m_promotionSquare(-1) {}
    void selectPiece(int position);
    void releasePiece(int newPosition);
    bool isPieceSelected() const { return m_selectedPiece != -1; }
    int selectedPieceId() const { return m_selectedPiece; }
    const Chess::Piece& selectedPiece() const { return m_game.getPiece(m_selectedPiece); }
    bool inPromotion() const { return m_promotionSquare != -1; }
    int promotionSquare() const;
    void cancelPromotion();
    void promote(Chess::Type promoteTo);
private:
    Chess::Game& m_game;
    int m_selectedPiece;
    int m_promotionSquare;
};
