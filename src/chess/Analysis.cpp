#include "Analysis.h"

#include <algorithm>
#include <cassert>

namespace Chess
{
    Analysis::Analysis(const Board& b, Color toMove)
        : m_board(b), m_king(toMove == Color::WHITE ? b.whiteKing() : b.blackKing())
    {
        for (int i = 0; i < NUM_PIECES; i++) {
            const Piece& p = b.getPiece(i);
            if (p.color == opposite(toMove) && p.alive)
                projectAttacks(p, m_attacks);
        }

        int startRank = rank(m_king.position), startFile = file(m_king.position);
        std::fill(m_pins, m_pins + NUM_SPACES, Direction{ 0, 0 });
        m_kingAttackerSpace = -1;
        for (const Direction& d : queenDirections)
        {
            std::vector<int> intervening;
            for (int r = startRank + d.r, f = startFile + d.f; isInBounds(r, f); r += d.r, f += d.f)
            {
                int i = space(r, f);
                if (b.getPieceId(i) != -1)
                {
                    const Piece& p = b.getPiece(b.getPieceId(i));
                    if (p.color == m_king.color && m_attacks[i].isDirectionAttacked(d.inverse()))
                    {
                        //std::cerr << "Space " << i << " is pinned in direction " << d.r << ", " << d.f << std::endl;
                        m_pins[i] = d;
                    }
                    if (p.color != m_king.color && m_attacks[m_king.position].isDirectionAttacked(d.inverse()))
                    {
                        m_kingAttackerSpace = i;
                        m_intervening = intervening;
                    }
                    break;
                }
                intervening.push_back(space(r, f));
            }
        }
        m_kingAttacks = m_attacks[m_king.position].count();
    }

    int Analysis::attackCount(int space) const
    {
        return m_attacks[space].count();
    }

    int Analysis::kingAttackCount() const
    {
        return m_kingAttacks;
    }

    bool Analysis::isPinnedInDifferentDirection(int space, Direction d) const
    {
        if (m_pins[space].r == 0 && m_pins[space].f == 0)
            return false;
        return !m_pins[space].isParallel(d);
    }

    int Analysis::kingAttackerSpace() const
    {
        assert(m_kingAttacks == 1);
        return m_kingAttackerSpace;
    }


    const Attacks& Analysis::kingAttacks() const
    {
        return m_attacks[m_king.position];
    }

    bool Analysis::isInterveningSquare(int space) const
    {
        return std::find(m_intervening.begin(), m_intervening.end(), space) != m_intervening.end();
    }

    void Analysis::projectAttacks(const Piece& p, Attacks* attacks)
    {
        switch (p.type)
        {
        case Type::BISHOP:
            projectDirectionalAttacks(p, bishopDirections, attacks);
            break;
        case Type::KING:
            projectAbsoluteAttacks(p, kingRelativePositions, attacks);
            break;
        case Type::KNIGHT:
            projectAbsoluteAttacks(p, knightRelativePositions, attacks);
            break;
        case Type::PAWN:
            projectPawnAttacks(p, attacks);
            break;
        case Type::QUEEN:
            projectDirectionalAttacks(p, queenDirections, attacks);
            break;
        case Type::ROOK:
            projectDirectionalAttacks(p, rookDirections, attacks);
            break;
        }
    }
    void Analysis::projectPawnAttacks(const Piece& p, Attacks* attacks)
    {
        int r = rank(p.position), f = file(p.position);
        int rankDirection = p.color == Color::WHITE ? 1 : -1;
        int left = space(r + rankDirection, f - 1);
        if (isInBounds(r + rankDirection, f - 1))
            attacks[left].addNonDirectional();
        int right = space(r + rankDirection, f + 1);
        if (isInBounds(r + rankDirection, f + 1))
            attacks[right].addNonDirectional();
    }
    void Analysis::projectDirectionalAttacks(const Piece& p, const std::vector<Direction>& directions, Attacks* attacks)
    {
        int startRank = rank(p.position), startFile = file(p.position);
        for (const Direction& d : directions)
        {
            for (int r = startRank + d.r, f = startFile + d.f; isInBounds(r, f); r += d.r, f += d.f)
            {
                int i = space(r, f);
                attacks[i].add(d);
                if (m_board.getPieceId(i) != -1)
                    break;
            }
        }
    }
    void Analysis::projectAbsoluteAttacks(const Piece& p, const std::vector<Direction>& relativePositions, Attacks* attacks)
    {
        int r = rank(p.position), f = file(p.position);
        for (const Direction& d : relativePositions)
        {
            int r2 = r + d.r, f2 = f + d.f;
            int i = space(r2, f2);
            if (isInBounds(r2, f2))
                attacks[i].addNonDirectional();
        }
    }
}
