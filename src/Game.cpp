#include "Game.h"
#include "utils.h"

#include <cassert>

struct Direction {
    int r, f;
};

Game::Game()
    : m_toMove(Color::WHITE), m_lastMove(-1)
{
    calculateAllLegalMoves();
}

// Assume pieceId is valid and dest is a valid square
void Game::movePiece(int pieceId, int dest)
{
    if (m_legalMoves[pieceId].count(dest) > 0)
    {
        m_board.movePiece(pieceId, dest);
        m_toMove = m_toMove == Color::WHITE ? Color::BLACK : Color::WHITE;
        m_lastMove = dest;
        calculateAllLegalMoves();
    }
}

int Game::getPieceId(int position) const
{
    return m_board.getPieceId(position);
}

const Piece& Game::getPiece(int pieceId) const
{
    return m_board.getPiece(pieceId);
}

const std::unordered_set<int>& Game::legalMoves(int pieceId) const
{
    return m_legalMoves[pieceId];
}

void Game::calculateAllLegalMoves()
{
    int total = 0;
    for (int i = 0; i < NUM_PIECES; i++)
    {
        calculateLegalMoves(i);
        total += m_legalMoves[i].size();
    }
    std::cerr << total << " legal moves" << std::endl;
}

void Game::calculateLegalMoves(int pieceId)
{
    m_legalMoves[pieceId].clear();
    const Piece& p = m_board.getPiece(pieceId);
    if (!p.alive || p.color != m_toMove)
        return;
    switch (p.type)
    {
    case Type::PAWN: pawnLegalMoves(p, m_legalMoves[pieceId]); return;
    case Type::ROOK: rookLegalMoves(p, m_legalMoves[pieceId]); return;
    case Type::KNIGHT: knightLegalMoves(p, m_legalMoves[pieceId]); return;
    case Type::BISHOP: bishopLegalMoves(p, m_legalMoves[pieceId]); return;
    case Type::QUEEN: queenLegalMoves(p, m_legalMoves[pieceId]); return;
    case Type::KING: kingLegalMoves(p, m_legalMoves[pieceId]); return;
    }
}

void Game::pawnLegalMoves(const Piece& p, std::unordered_set<int>& moves)
{
    int r = rank(p.position), f = file(p.position);
    int rankDirection = p.color == Color::WHITE ? 1 : -1;
    int forward = space(r + rankDirection, f);
    if (isInBounds(forward) && m_board.getPieceId(forward) == -1)
    {
        moves.insert(forward);
        if (!p.hasMoved)
        {
            int twoForward = space(r + 2 * rankDirection, f);
            if (isInBounds(twoForward) && m_board.getPieceId(twoForward) == -1)
                moves.insert(twoForward);
        }
    }
    int left = space(r + rankDirection, f - 1);
    if (isInBounds(left) && m_board.getPieceId(left) != -1 && m_board.getPiece(m_board.getPieceId(left)).color != p.color)
        moves.insert(left);
    int right = space(r + rankDirection, f + 1);
    if (isInBounds(right) && m_board.getPieceId(right) != -1 && m_board.getPiece(m_board.getPieceId(right)).color != p.color)
        moves.insert(right);
}
void Game::rookLegalMoves(const Piece& p, std::unordered_set<int>& moves)
{
    static std::vector<Direction> directions = { {0,1},{1,0},{0,-1},{-1,0} };
    directionalLegalMoves(p, directions, moves);
}
void Game::bishopLegalMoves(const Piece& p, std::unordered_set<int>& moves)
{
    static std::vector<Direction> directions = { {1,1},{1,-1},{-1,-1},{-1,1} };
    directionalLegalMoves(p, directions, moves);
}
void Game::knightLegalMoves(const Piece& p, std::unordered_set<int>& moves)
{
    static std::vector<Direction> relativePositions = { {1,2},{2,1},{1,-2},{2,-1},{-1,-2},{-2,-1},{-1,2},{-2,1} };
    absoluteLegalMoves(p, relativePositions, moves);
}
void Game::queenLegalMoves(const Piece& p, std::unordered_set<int>& moves)
{
    static std::vector<Direction> directions = { {1,0},{1,1},{0,1},{-1,1},{-1,0},{-1,-1},{0,-1},{1,-1} };
    directionalLegalMoves(p, directions, moves);
}
void Game::kingLegalMoves(const Piece& p, std::unordered_set<int>& moves)
{
    static std::vector<Direction> relativePositions = { {1,0},{1,1},{0,1},{-1,1},{-1,0},{-1,-1},{0,-1},{1,-1} };
    absoluteLegalMoves(p, relativePositions, moves);
}

void Game::directionalLegalMoves(const Piece& p, const std::vector<Direction>& directions, std::unordered_set<int>& moves)
{
    int startRank = rank(p.position), startFile = file(p.position);
    for (const Direction& d : directions)
    {
        for (int r = startRank + d.r, f = startFile + d.f; isInBounds(r, f); r += d.r, f += d.f)
        {
            int i = space(r, f);
            if (m_board.getPieceId(i) != -1 && m_board.getPiece(m_board.getPieceId(i)).color == p.color)
                break;
            moves.insert(i);
            if (m_board.getPieceId(i) != -1)
                break;
        }
    }
}

void Game::absoluteLegalMoves(const Piece& p, const std::vector<Direction>& relativePositions, std::unordered_set<int>& moves)
{
    int r = rank(p.position), f = file(p.position);
    for (const Direction& d : relativePositions)
    {
        int r2 = r + d.r, f2 = f + d.f;
        int i = space(r2, f2);
        if (isInBounds(r2, f2) && (m_board.getPieceId(i) == -1 || m_board.getPiece(m_board.getPieceId(i)).color != p.color))
            moves.insert(i);
    }
}
