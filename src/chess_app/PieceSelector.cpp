#include "PieceSelector.h"

#include <cassert>

// position may be a valid square or -1
void PieceSelector::selectPiece(int position)
{
    assert(!inPromotion());
    if (position != -1)
    {
        int pieceId = m_game.getPieceId(position);
        if (pieceId != -1 && m_game.getPiece(pieceId).color == m_game.toMove())
        {
            m_selectedPiece = pieceId;
            //std::cerr << m_legalMoves.size() << " legal moves" << std::endl;
        }
    }
}

// position may be a valid square or -1
void PieceSelector::releasePiece(int position)
{
    assert(!inPromotion());
    if (m_selectedPiece == -1 || position == -1)
        return;

    if (m_game.isPromotion(selectedPiece().position, position))
        m_promotionSquare = position;
    else
    {
        m_game.movePiece(m_selectedPiece, position);
        m_selectedPiece = -1;
    }
}

int PieceSelector::promotionSquare() const
{
    assert(inPromotion());
    return m_promotionSquare;
}

void PieceSelector::cancelPromotion()
{
    assert(inPromotion());
    m_selectedPiece = -1;
    m_promotionSquare = -1;
}

void PieceSelector::promote(Chess::Type promoteTo)
{
    assert(inPromotion());
    m_game.movePieceWithPromotion(m_selectedPiece, m_promotionSquare, promoteTo);
    m_selectedPiece = -1;
    m_promotionSquare = -1;
}
