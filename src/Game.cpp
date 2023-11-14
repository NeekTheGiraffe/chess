#include "Game.h"
#include "utils.h"

#include <cassert>

struct Direction {
    int r, f;
};

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
            m_legalMoves = calculateLegalMoves(pieceId);
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

std::unordered_set<int> Game::calculateLegalMoves(int pieceId) const
{
    const Piece& p = m_board.getPiece(pieceId);
    switch (p.type)
    {
    case Type::PAWN: return pawnLegalMoves(p);
    case Type::ROOK: return rookLegalMoves(p);
    case Type::KNIGHT: return knightLegalMoves(p);
    case Type::BISHOP: return bishopLegalMoves(p);
    case Type::QUEEN: return queenLegalMoves(p);
    case Type::KING: return kingLegalMoves(p);
    }
}

std::unordered_set<int> Game::pawnLegalMoves(const Piece& p) const
{
    std::unordered_set<int> result;
    int r = rank(p.position), f = file(p.position);
    int rankDirection = p.color == Color::WHITE ? 1 : -1;
    int forward = space(r + rankDirection, f);
    if (isInBounds(forward) && m_board.getPieceId(forward) == -1)
        result.insert(forward);
    int left = space(r + rankDirection, f - 1);
    if (isInBounds(left) && m_board.getPieceId(left) != -1 && m_board.getPiece(m_board.getPieceId(left)).color != p.color)
        result.insert(left);
    int right = space(r + rankDirection, f + 1);
    if (isInBounds(right) && m_board.getPieceId(right) != -1 && m_board.getPiece(m_board.getPieceId(right)).color != p.color)
        result.insert(right);
    return result;
}
std::unordered_set<int> Game::rookLegalMoves(const Piece& p) const
{
    static std::vector<Direction> directions = { {0,1},{1,0},{0,-1},{-1,0} };
    return directionalLegalMoves(p, directions);
}
std::unordered_set<int> Game::bishopLegalMoves(const Piece& p) const
{
    static std::vector<Direction> directions = { {1,1},{1,-1},{-1,-1},{-1,1} };
    return directionalLegalMoves(p, directions);
}
std::unordered_set<int> Game::knightLegalMoves(const Piece& p) const
{
    static std::vector<Direction> relativePositions = { {1,2},{2,1},{1,-2},{2,-1},{-1,-2},{-2,-1},{-1,2},{-2,1} };
    return absoluteLegalMoves(p, relativePositions);
}
std::unordered_set<int> Game::queenLegalMoves(const Piece& p) const
{
    static std::vector<Direction> directions = { {1,0},{1,1},{0,1},{-1,1},{-1,0},{-1,-1},{0,-1},{1,-1} };
    return directionalLegalMoves(p, directions);
}
std::unordered_set<int> Game::kingLegalMoves(const Piece& p) const
{
    static std::vector<Direction> relativePositions = { {1,0},{1,1},{0,1},{-1,1},{-1,0},{-1,-1},{0,-1},{1,-1} };
    return absoluteLegalMoves(p, relativePositions);
}

std::unordered_set<int> Game::directionalLegalMoves(const Piece& p, const std::vector<Direction>& directions) const
{
    int startRank = rank(p.position), startFile = file(p.position);
    std::unordered_set<int> result;
    for (const Direction& d : directions)
    {
        for (int r = startRank + d.r, f = startFile + d.f; isInBounds(r, f); r += d.r, f += d.f)
        {
            int i = space(r, f);
            if (m_board.getPieceId(i) != -1 && m_board.getPiece(m_board.getPieceId(i)).color == p.color)
                break;
            result.insert(i);
            if (m_board.getPieceId(i) != -1)
                break;
        }
    }
    return result;
}

std::unordered_set<int> Game::absoluteLegalMoves(const Piece& p, const std::vector<Direction>& relativePositions) const
{
    int r = rank(p.position), f = file(p.position);
    std::unordered_set<int> result;
    for (const Direction& d : relativePositions)
    {
        int r2 = r + d.r, f2 = f + d.f;
        int i = space(r2, f2);
        if (isInBounds(r2, f2) && (m_board.getPieceId(i) == -1 || m_board.getPiece(m_board.getPieceId(i)).color != p.color))
            result.insert(i);
    }
    return result;
}
