#include "Game.h"
#include "utils.h"
#include "constants.h"
#include "Analysis.h"

#include <algorithm>
#include <cassert>

namespace Chess
{
    Game::Game()
        : m_toMove(Color::WHITE), m_lastMove(-1)
    {
        calculateAllLegalMoves();
    }
    Game::Game(const Board& board, Color toMove, int lastMove)
        : m_board(board), m_toMove(toMove), m_lastMove(lastMove)
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
    int Game::getPieceId(int position) const {
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
        Analysis a(m_board, m_toMove);
        int total = 0;
        for (int i = 0; i < NUM_PIECES; i++)
        {
            calculateLegalMoves(i, a);
            total += m_legalMoves[i].size();
        }
        std::cerr << total << " legal moves" << std::endl;
    }

    void Game::calculateLegalMoves(int pieceId, const Analysis& analysis)
    {
        m_legalMoves[pieceId].clear();
        const Piece& p = m_board.getPiece(pieceId);
        if (!p.alive || p.color != m_toMove)
            return;
        switch (p.type)
        {
        case Type::PAWN:
            pawnLegalMoves(p, m_legalMoves[pieceId], analysis);
            return;
        case Type::ROOK:
            directionalLegalMoves(p, rookDirections, m_legalMoves[pieceId], analysis);
            return;
        case Type::KNIGHT:
            absoluteLegalMoves(p, knightRelativePositions, m_legalMoves[pieceId], analysis);
            return;
        case Type::BISHOP:
            directionalLegalMoves(p, bishopDirections, m_legalMoves[pieceId], analysis);
            return;
        case Type::QUEEN:
            directionalLegalMoves(p, queenDirections, m_legalMoves[pieceId], analysis);
            return;
        case Type::KING:
            kingLegalMoves(p, m_legalMoves[pieceId], analysis);
            return;
        }
    }

    void Game::pawnLegalMoves
    (
        const Piece& p,
        std::unordered_set<int>& moves,
        const Analysis& analysis
    )
    {
        int r = rank(p.position), f = file(p.position);
        int rankDirection = p.color == Color::WHITE ? 1 : -1;
        int forward = space(r + rankDirection, f);
        if (isInBounds(forward) && m_board.getPieceId(forward) == -1 &&
            !analysis.isPinnedInDifferentDirection(p.position, Direction{ rankDirection, 0 }))
        {
            if (analysis.kingAttackCount() == 0 || analysis.isInterveningSquare(forward))
                moves.insert(forward);
            if (!p.hasMoved)
            {
                int twoForward = space(r + 2 * rankDirection, f);
                if (isInBounds(twoForward) && m_board.getPieceId(twoForward) == -1 &&
                    (analysis.kingAttackCount() == 0 || analysis.isInterveningSquare(twoForward)))
                    moves.insert(twoForward);
            }
        }
        int left = space(r + rankDirection, f - 1);
        if (isInBounds(left) && m_board.getPieceId(left) != -1 &&
            m_board.getPiece(m_board.getPieceId(left)).color != p.color &&
            !analysis.isPinnedInDifferentDirection(p.position, Direction{ rankDirection, -1 }) &&
            (analysis.kingAttackCount() == 0 || analysis.kingAttackerSpace() == left))
            moves.insert(left);
        int right = space(r + rankDirection, f + 1);
        if (isInBounds(right) && m_board.getPieceId(right) != -1 &&
            m_board.getPiece(m_board.getPieceId(right)).color != p.color &&
            !analysis.isPinnedInDifferentDirection(p.position, Direction{ rankDirection, 1 }) &&
            (analysis.kingAttackCount() == 0 || analysis.kingAttackerSpace() == right))
            moves.insert(right);
    }
    void Game::kingLegalMoves
    (
        const Piece& p,
        std::unordered_set<int>& moves,
        const Analysis& analysis
    )
    {
        int r = rank(p.position), f = file(p.position);
        for (const Direction& d : kingRelativePositions)
        {
            int r2 = r + d.r, f2 = f + d.f;
            int i = space(r2, f2);
            if (isInBounds(r2, f2) &&
                (m_board.getPieceId(i) == -1 || m_board.getPiece(m_board.getPieceId(i)).color != p.color) &&
                analysis.attackCount(i) == 0 &&
                !analysis.kingAttacks().isDirectionAttacked(d))
                moves.insert(i);
        }
    }
    void Game::directionalLegalMoves
    (
        const Piece& p,
        const std::vector<Direction>& directions,
        std::unordered_set<int>& moves,
        const Analysis& analysis
    )
    {
        if (analysis.kingAttackCount() > 1)
            return;
        int startRank = rank(p.position), startFile = file(p.position);
        for (const Direction& d : directions)
        {
            if (analysis.isPinnedInDifferentDirection(p.position, d))
                continue;
            for (int r = startRank + d.r, f = startFile + d.f; isInBounds(r, f); r += d.r, f += d.f)
            {
                int i = space(r, f);
                if (m_board.getPieceId(i) != -1 && m_board.getPiece(m_board.getPieceId(i)).color == p.color)
                    break;
                if (analysis.kingAttackCount() == 0 ||
                    analysis.kingAttackerSpace() == i ||
                    analysis.isInterveningSquare(i))
                    moves.insert(i);
                if (m_board.getPieceId(i) != -1)
                    break;
            }
        }
    }
    void Game::absoluteLegalMoves
    (
        const Piece& p,
        const std::vector<Direction>& relativePositions,
        std::unordered_set<int>& moves,
        const Analysis& analysis
    )
    {
        if (analysis.kingAttackCount() > 1)
            return;
        int r = rank(p.position), f = file(p.position);
        for (const Direction& d : relativePositions)
        {
            if (analysis.isPinnedInDifferentDirection(p.position, d))
                continue;
            int r2 = r + d.r, f2 = f + d.f;
            int i = space(r2, f2);
            if (isInBounds(r2, f2) && (m_board.getPieceId(i) == -1 || m_board.getPiece(m_board.getPieceId(i)).color != p.color) &&
                (analysis.kingAttackCount() == 0 ||
                    analysis.kingAttackerSpace() == i ||
                    analysis.isInterveningSquare(i)))
                moves.insert(i);
        }
    }
}
