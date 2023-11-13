#include "Chess.h"
#include "utils.h"

#include <algorithm>
#include <iostream>

struct Direction {
    int r, f;
};

const Piece STARTING_PIECES[NUM_PIECES] = {
    {0, Type::ROOK}, {1, Type::KNIGHT}, {2, Type::BISHOP}, {3, Type::QUEEN}, {4, Type::KING}, {5, Type::BISHOP}, {6, Type::KNIGHT}, {7, Type::ROOK},
    {8, Type::PAWN}, {9, Type::PAWN}, {10, Type::PAWN}, {11, Type::PAWN}, {12, Type::PAWN}, {13, Type::PAWN}, {14, Type::PAWN}, {15, Type::PAWN},
    {48, Type::PAWN}, {49, Type::PAWN}, {50, Type::PAWN}, {51, Type::PAWN}, {52, Type::PAWN}, {53, Type::PAWN}, {54, Type::PAWN}, {55, Type::PAWN},
    {56, Type::ROOK}, {57, Type::KNIGHT}, {58, Type::BISHOP}, {59, Type::QUEEN}, {60, Type::KING}, {61, Type::BISHOP}, {62, Type::KNIGHT}, {63, Type::ROOK},
};

Chess::Chess()
{
    std::copy(STARTING_PIECES, STARTING_PIECES + 32, m_pieces);
    std::fill(m_board, m_board + NUM_SPACES, -1);
    for (int i = 0; i < NUM_PIECES; i++)
    {
        m_pieces[i].alive = true;
        m_pieces[i].color = i < 16 ? Color::WHITE : Color::BLACK;
        m_board[m_pieces[i].position] = i;
    }

    for (int i = 0; i < NUM_SPACES; i++)
    {
        std::cout << m_board[i] << " ";
        if (i % BOARD_WIDTH == BOARD_WIDTH - 1)
            std::cout << std::endl;
    }
}
int Chess::getPieceId(int space) const { return m_board[space]; }
const Piece& Chess::getPiece(int pieceId) const { return m_pieces[pieceId]; }
void Chess::movePiece(int pieceId, int space)
{
    if (m_board[space] != -1)
        m_pieces[m_board[space]].alive = false;
    m_board[m_pieces[pieceId].position] = -1;

    m_board[space] = pieceId;
    m_pieces[pieceId].position = space;
}

std::unordered_set<int> Chess::legalMoves(int pieceId) const
{
    const Piece& p = m_pieces[pieceId];
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

std::unordered_set<int> Chess::pawnLegalMoves(const Piece& p) const
{
    std::unordered_set<int> result;
    int r = rank(p.position), f = file(p.position);
    int rankDirection = p.color == Color::WHITE ? 1 : -1;
    int forward = space(r + rankDirection, f);
    if (isInBounds(forward) && m_board[forward] == -1)
        result.insert(forward);
    int left = space(r + rankDirection, f - 1);
    if (isInBounds(left) && m_board[left] != -1 && m_pieces[m_board[left]].color != p.color)
        result.insert(left);
    int right = space(r + rankDirection, f + 1);
    if (isInBounds(right) && m_board[right] != -1 && m_pieces[m_board[right]].color != p.color)
        result.insert(right);
    return result;
}
std::unordered_set<int> Chess::rookLegalMoves(const Piece& p) const
{
    static std::vector<Direction> directions = { {0,1},{1,0},{0,-1},{-1,0} };
    return directionalLegalMoves(p, directions);
}
std::unordered_set<int> Chess::bishopLegalMoves(const Piece& p) const
{
    static std::vector<Direction> directions = { {1,1},{1,-1},{-1,-1},{-1,1} };
    return directionalLegalMoves(p, directions);
}
std::unordered_set<int> Chess::knightLegalMoves(const Piece& p) const
{
    static std::vector<Direction> relativePositions = { {1,2},{2,1},{1,-2},{2,-1},{-1,-2},{-2,-1},{-1,2},{-2,1} };
    return absoluteLegalMoves(p, relativePositions);
}
std::unordered_set<int> Chess::queenLegalMoves(const Piece& p) const
{
    static std::vector<Direction> directions = { {1,0},{1,1},{0,1},{-1,1},{-1,0},{-1,-1},{0,-1},{1,-1} };
    return directionalLegalMoves(p, directions);
}
std::unordered_set<int> Chess::kingLegalMoves(const Piece& p) const
{
    static std::vector<Direction> relativePositions = { {1,0},{1,1},{0,1},{-1,1},{-1,0},{-1,-1},{0,-1},{1,-1} };
    return absoluteLegalMoves(p, relativePositions);
}

std::unordered_set<int> Chess::directionalLegalMoves(const Piece& p, const std::vector<Direction>& directions) const
{
    int startRank = rank(p.position), startFile = file(p.position);
    std::unordered_set<int> result;
    for (const Direction& d : directions)
    {
        for (int r = startRank + d.r, f = startFile + d.f; isInBounds(r, f); r += d.r, f += d.f)
        {
            int i = space(r, f);
            if (m_board[i] != -1 && m_pieces[m_board[i]].color == p.color)
                break;
            result.insert(i);
            if (m_board[i] != -1)
                break;
        }
    }
    return result;
}

std::unordered_set<int> Chess::absoluteLegalMoves(const Piece& p, const std::vector<Direction>& relativePositions) const
{
    int r = rank(p.position), f = file(p.position);
    std::unordered_set<int> result;
    for (const Direction& d : relativePositions)
    {
        int r2 = r + d.r, f2 = f + d.f;
        int i = space(r2, f2);
        if (isInBounds(r2, f2) && (m_board[i] == -1 || m_pieces[m_board[i]].color != p.color))
            result.insert(i);
    }
    return result;
}
