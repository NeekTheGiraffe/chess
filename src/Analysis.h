#pragma once

#include "utils.h"
#include "Board.h"
#include "Attacks.h"
#include "Direction.h"

class Analysis
{
public:
    Analysis(const Board& b, Color toMove);
    int attackCount(int space) const;
    int kingAttackCount() const;
    bool isPinnedInDifferentDirection(int space, Direction d) const;
    int kingAttackerSpace() const;
    const Attacks& kingAttacks() const;
    bool isInterveningSquare(int space) const;
private:
    void projectAttacks(const Piece& p, Attacks* attacks);
    void projectPawnAttacks(const Piece& p, Attacks* attacks);
    void projectDirectionalAttacks(const Piece& p, const std::vector<Direction>& directions, Attacks* attacks);
    void projectAbsoluteAttacks(const Piece& p, const std::vector<Direction>& relativePositions, Attacks* attacks);

    const Board& m_board;
    int m_kingAttacks;
    const Piece& m_king;
    int m_kingAttackerSpace;
    Attacks m_attacks[NUM_SPACES];
    Direction m_pins[NUM_SPACES];
    std::vector<int> m_intervening;
};