#include "Game.h"

#include <cassert>

Game::Game()
    : m_selectedPiece(-1), m_toMove(Color::WHITE), m_lastMove(-1)
{
}

void Game::selectPieceAt(int position)
{
    if (position != -1)
    {
        int pieceId = m_board.getPieceId(position);
        if (pieceId != -1 && m_board.getPiece(pieceId).color == m_toMove)
        {
            m_selectedPiece = pieceId;
            m_legalMoves = m_board.legalMoves(pieceId);
            //std::cerr << m_legalMoves.size() << " legal moves" << std::endl;
        }
    }
}

void Game::releasePieceAt(int position)
{
    if (m_selectedPiece != -1)
    {
        if (position != -1 && m_legalMoves.count(position) > 0)
        {
            m_board.movePiece(m_selectedPiece, position);
            m_toMove = m_toMove == Color::WHITE ? Color::BLACK : Color::WHITE;
            m_lastMove = position;
        }
    }
    m_selectedPiece = -1;
}

bool Game::isPieceSelected() const
{
    return m_selectedPiece != -1;
}

const Piece& Game::selectedPiece() const
{
    assert(isPieceSelected());
    return m_board.getPiece(m_selectedPiece);
}

const Piece& Game::getPiece(int pieceId) const
{
    return m_board.getPiece(pieceId);
}

const std::unordered_set<int>& Game::legalMoves() const
{
    return m_legalMoves;
}
