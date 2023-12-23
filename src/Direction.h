#pragma once

namespace Chess
{
    struct Direction {
        int r, f;
        Direction inverse() const { return Direction{ -r, -f }; }
        // Assumes both r and f are in { -1, 0, 1 }
        bool isParallel(const Direction& other) const
        {
            Direction inv = other.inverse();
            return r == other.r && f == other.f ||
                r == inv.r && f == inv.f;
        }
    };
}