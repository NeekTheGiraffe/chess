#pragma once

#include "utils.h"
#include "Direction.h"

namespace Chess
{
    class Attacks
    {
    public:
        Attacks();
        void addNonDirectional() { m_attacks++; }
        void add(Direction d);
        int count() const { return m_attacks; }
        bool isDirectionAttacked(Direction d) const;
    private:
        int m_attacks;
        bool m_directions[8];
    };
}
