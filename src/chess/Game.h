#pragma once

#include "Board.h"
#include "utils.h"
#include "Direction.h"
#include "Analysis.h"

#include <unordered_set>
#include <unordered_map>

namespace Chess {
    struct SourceDest
    {
        int src, dest;
        bool operator==(const SourceDest& other) const { return src == other.src && dest == other.dest; }
    };
}

template <>
struct std::hash<Chess::SourceDest>
{
    size_t operator()(const Chess::SourceDest& sd) const
    {
        using std::hash;
        return (hash<int>()(sd.src)) ^ (hash<int>()(sd.dest) << 1);
    }
};

namespace Chess
{
    enum class Move
    {
        CASTLE,
        EN_PASSANT,
    };

    class Game
    {
    public:
        Game();
        Game(const Board& board, Color toMove, int lastMove, int lastMoveSrc);
        void movePiece(int pieceId, int dest);
        int lastMove() const { return m_lastMoveDest; }
        int getPieceId(int position) const;
        const Piece& getPiece(int pieceId) const;
        Color toMove() const { return m_toMove; };

        const std::unordered_set<int>& legalMoves(int pieceId) const;
    private:
        void calculateAllLegalMoves();
        void calculateLegalMoves(int pieceId, const Analysis& a);
        void pawnLegalMoves(const Piece& p, std::unordered_set<int>& moves, const Analysis& a);
        void kingLegalMoves(const Piece& p, std::unordered_set<int>& moves, const Analysis& a);
        void directionalLegalMoves(
            const Piece& p,
            const std::vector<Direction>& directions,
            std::unordered_set<int>& moves,
            const Analysis& a
        );
        void absoluteLegalMoves(
            const Piece& p,
            const std::vector<Direction>& relativePositions,
            std::unordered_set<int>& moves,
            const Analysis& a
        );

        std::unordered_set<int> m_legalMoves[NUM_PIECES];
        std::unordered_map<SourceDest, Move> m_specialMoves;
        Board m_board;
        Color m_toMove;
        int m_lastMoveDest;
        int m_lastMoveSrc;
    };
}
