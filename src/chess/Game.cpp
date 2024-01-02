#include "Game.h"
#include "utils.h"
#include "constants.h"
#include "Analysis.h"

#include <algorithm>
#include <cassert>
#include <iostream>

namespace Chess
{
    Game::Game()
        : m_toMove(Color::WHITE), m_lastMoveDest(-1)
    {
        calculateAllLegalMoves();
    }
    Game::Game(const Board& board, Color toMove, int lastMove, int lastMoveSrc)
        : m_board(board), m_toMove(toMove), m_lastMoveDest(lastMove), m_lastMoveSrc(lastMoveSrc)
    {
        calculateAllLegalMoves();
    }

    // Assume pieceId is valid and dest is a valid square
    void Game::movePiece(int pieceId, int dest)
    {
        if (m_legalMoves[pieceId].count(dest) > 0)
        {
            int src = m_board.getPiece(pieceId).position;
            m_board.movePiece(pieceId, dest);
            m_toMove = opposite(m_toMove);
            m_lastMoveDest = dest;
            m_lastMoveSrc = src;
            
            if (m_specialMoves.count({ src, dest }) > 0)
            {
                Move move = m_specialMoves[{ src, dest }];
                if (move == Move::CASTLE)
                {
                    if (file(dest) == C_FILE)
                        m_board.movePiece(m_board.getPieceId(space(rank(src), A_FILE)),
                            space(rank(src), D_FILE));
                    else
                        m_board.movePiece(m_board.getPieceId(space(rank(src), H_FILE)),
                            space(rank(src), F_FILE));
                }
                else if (move == Move::EN_PASSANT)
                {
                    m_board.destroyPieceAt(space(rank(src), file(dest)));
                }
                assert(move != Move::PROMOTION);
            }
            calculateAllLegalMoves();
        }
    }
    void Game::movePieceWithPromotion(int pieceId, int dest, Type promoteTo)
    {
        if (m_legalMoves[pieceId].count(dest) == 0)
            return;
        int src = m_board.getPiece(pieceId).position;
        assert(isPromotion(src, dest));
        assert(promoteTo != Type::PAWN && promoteTo != Type::KING);
        m_board.movePiece(pieceId, dest);
        m_board.promote(pieceId, promoteTo);
        m_toMove = opposite(m_toMove);
        m_lastMoveDest = dest;
        m_lastMoveSrc = src;
        calculateAllLegalMoves();
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
    bool Game::isPromotion(int src, int dest) const
    {
        return m_specialMoves.count({ src, dest }) > 0 &&
            m_specialMoves.at({ src, dest }) == Move::PROMOTION;
    }
    bool Game::hasLegalMoves() const { return m_legalMovesCount > 0; }
    bool Game::inCheckmate() const
    {
        return m_legalMovesCount == 0 && m_kingAttacked;
    }
    bool Game::inStalemate() const
    {
        return m_legalMovesCount == 0 && !m_kingAttacked;
    }

    void Game::calculateAllLegalMoves()
    {
        m_specialMoves.clear();
        Analysis a(m_board, m_toMove);
        m_kingAttacked = a.kingAttackCount() > 0;
        m_legalMovesCount = 0;
        for (int i = 0; i < NUM_PIECES; i++)
        {
            calculateLegalMoves(i, a);
            m_legalMovesCount += m_legalMoves[i].size();
        }
        std::cerr << m_legalMovesCount << " legal moves" << std::endl;
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

    bool isPromotionRank(Color c, int rank)
    {
        return c == Color::WHITE && rank == RANK_8 ||
            c == Color::BLACK && rank == RANK_1;
    }
    void Game::pawnLegalMoves
    (
        const Piece& p,
        std::unordered_set<int>& moves,
        const Analysis& analysis
    )
    {
        if (analysis.kingAttackCount() > 1)
            return;
        int r = rank(p.position), f = file(p.position);
        int rankDirection = p.color == Color::WHITE ? 1 : -1;
        int forward = space(r + rankDirection, f);
        if (isInBounds(r + rankDirection, f) && m_board.getPieceId(forward) == -1 &&
            !analysis.isPinnedInDifferentDirection(p.position, Direction{ rankDirection, 0 }))
        {
            if (analysis.kingAttackCount() == 0 || analysis.isInterveningSquare(forward))
            {
                moves.insert(forward);
                if (isPromotionRank(p.color, r + rankDirection))
                    m_specialMoves[{ p.position, forward }] = Move::PROMOTION;
            }
            if (!p.hasMoved)
            {
                int twoForward = space(r + 2 * rankDirection, f);
                if (isInBounds(r + 2 * rankDirection, f) && m_board.getPieceId(twoForward) == -1 &&
                    (analysis.kingAttackCount() == 0 || analysis.isInterveningSquare(twoForward)))
                    moves.insert(twoForward);
            }
        }
        
        int left = space(r + rankDirection, f - 1);
        if (isInBounds(r + rankDirection, f - 1) &&
            !analysis.isPinnedInDifferentDirection(p.position, Direction{ rankDirection, -1 }) &&
            (analysis.kingAttackCount() == 0 || analysis.kingAttackerSpace() == left))
        {
            if (m_board.hasPiece(left) && m_board.getPieceAt(left).color != p.color)
            {
                moves.insert(left);
                if (isPromotionRank(p.color, r + rankDirection))
                    m_specialMoves[{ p.position, left }] = Move::PROMOTION;
            }
            if (m_lastMoveDest == space(r, f - 1) &&
                m_board.getPieceAt(space(r, f - 1)).type == Type::PAWN &&
                m_lastMoveSrc == space(r + 2 * rankDirection, f - 1))
            {
                moves.insert(left);
                m_specialMoves[{ p.position, left }] = Move::EN_PASSANT;
            }
        }

        int right = space(r + rankDirection, f + 1);
        if (isInBounds(r + rankDirection, f + 1) &&
            !analysis.isPinnedInDifferentDirection(p.position, Direction{ rankDirection, 1 }) &&
            (analysis.kingAttackCount() == 0 || analysis.kingAttackerSpace() == left))
        {
            if (m_board.hasPiece(right) && m_board.getPieceAt(right).color != p.color)
            {
                moves.insert(right);
                if (isPromotionRank(p.color, r + rankDirection))
                    m_specialMoves[{ p.position, right }] = Move::PROMOTION;
            }
            if (m_lastMoveDest == space(r, f + 1) &&
                m_board.getPieceAt(space(r, f + 1)).type == Type::PAWN &&
                m_lastMoveSrc == space(r + 2 * rankDirection, f + 1))
            {
                moves.insert(right);
                m_specialMoves[{ p.position, right }] = Move::EN_PASSANT;
            }
        }
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
        // Castling
        if (!p.hasMoved && analysis.kingAttackCount() == 0)
        {
            const Piece& hFilePiece = m_board.getPieceAt(space(r, H_FILE));
            if (hFilePiece.type == Type::ROOK && hFilePiece.color == p.color &&
                !m_board.hasPiece(space(r, F_FILE)) && analysis.attackCount(space(r, F_FILE)) == 0 &&
                !m_board.hasPiece(space(r, G_FILE)) && analysis.attackCount(space(r, G_FILE)) == 0)
            {
                int castleDest = space(r, G_FILE);
                moves.insert(castleDest);
                m_specialMoves[{ p.position, castleDest }] = Move::CASTLE;
            }
            const Piece& aFilePiece = m_board.getPieceAt(space(r, A_FILE));
            if (aFilePiece.type == Type::ROOK && aFilePiece.color == p.color &&
                !m_board.hasPiece(space(r, B_FILE)) &&
                !m_board.hasPiece(space(r, C_FILE)) && analysis.attackCount(space(r, C_FILE)) == 0 &&
                !m_board.hasPiece(space(r, D_FILE)) && analysis.attackCount(space(r, D_FILE)) == 0)
            {
                int castleDest = space(r, C_FILE);
                moves.insert(castleDest);
                m_specialMoves[{ p.position, castleDest }] = Move::CASTLE;
            }
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
