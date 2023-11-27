#include "PieceSelector.h"

// position may be a valid square or -1
void PieceSelector::selectPiece(int position)
{
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
    if (m_selectedPiece != -1 && position != -1)
        m_game.movePiece(m_selectedPiece, position);
    m_selectedPiece = -1;
}
