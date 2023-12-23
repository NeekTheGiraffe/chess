#include "Board.h"
#include "utils.h"

#include <algorithm>
#include <iostream>
#include <sstream>
#include <cassert>

bool getTypeColor(char ch, Type& type, Color& color);

const Piece STARTING_PIECES[NUM_PIECES] = {
    {0, Type::ROOK}, {1, Type::KNIGHT}, {2, Type::BISHOP}, {3, Type::QUEEN}, {4, Type::KING}, {5, Type::BISHOP}, {6, Type::KNIGHT}, {7, Type::ROOK},
    {8, Type::PAWN}, {9, Type::PAWN}, {10, Type::PAWN}, {11, Type::PAWN}, {12, Type::PAWN}, {13, Type::PAWN}, {14, Type::PAWN}, {15, Type::PAWN},
    {48, Type::PAWN}, {49, Type::PAWN}, {50, Type::PAWN}, {51, Type::PAWN}, {52, Type::PAWN}, {53, Type::PAWN}, {54, Type::PAWN}, {55, Type::PAWN},
    {56, Type::ROOK}, {57, Type::KNIGHT}, {58, Type::BISHOP}, {59, Type::QUEEN}, {60, Type::KING}, {61, Type::BISHOP}, {62, Type::KNIGHT}, {63, Type::ROOK},
};

Board::Board()
    : m_whiteKingId(4), m_blackKingId(28)
{
    std::copy(STARTING_PIECES, STARTING_PIECES + NUM_PIECES, m_pieces);
    std::fill(m_spaces, m_spaces + NUM_SPACES, -1);
    for (int i = 0; i < NUM_PIECES; i++)
    {
        m_pieces[i].alive = true;
        m_pieces[i].hasMoved = false;
        m_pieces[i].color = i < 16 ? Color::WHITE : Color::BLACK;
        m_spaces[m_pieces[i].position] = i;
    }

    for (int i = 0; i < NUM_SPACES; i++)
    {
        std::cout << m_spaces[i] << " ";
        if (i % BOARD_WIDTH == BOARD_WIDTH - 1)
            std::cout << std::endl;
    }
}
Board::Board(const std::string& in)
    : m_whiteKingId(-1), m_blackKingId(-1)
{
    std::fill(m_pieces, m_pieces + NUM_PIECES, Piece());
    std::fill(m_spaces, m_spaces + NUM_SPACES, -1);
    std::istringstream instream(in);
    int nextPiece = 0;
    for (int rank = BOARD_WIDTH - 1; rank >= 0; rank--) {
        for (int file = 0; file < BOARD_WIDTH; file++) {
            char c = instream.get();
            Type type;
            Color color;
            if (!getTypeColor(c, type, color)) {
                continue;
            }
            assert(nextPiece < NUM_PIECES);
            m_pieces[nextPiece] = Piece{
                rank * BOARD_WIDTH + file,
                type,
                true,
                false,
                color,
            };
            m_spaces[rank * BOARD_WIDTH + file] = nextPiece;
            if (type == Type::KING) {
                if (color == Color::WHITE)
                    m_whiteKingId = nextPiece;
                else
                    m_blackKingId = nextPiece;
            }
            nextPiece++;
        }
    }
}
int Board::getPieceId(int space) const { return m_spaces[space]; }
const Piece& Board::getPiece(int pieceId) const { return m_pieces[pieceId]; }
void Board::movePiece(int pieceId, int space)
{
    if (m_spaces[space] != -1)
        m_pieces[m_spaces[space]].alive = false;
    m_spaces[m_pieces[pieceId].position] = -1;

    m_spaces[space] = pieceId;
    m_pieces[pieceId].position = space;
    m_pieces[pieceId].hasMoved = true;
}
const Piece& Board::whiteKing() const
{
    assert(m_whiteKingId >= 0);
    return m_pieces[m_whiteKingId];
}
const Piece& Board::blackKing() const
{
    assert(m_blackKingId >= 0);
    return m_pieces[m_blackKingId];
}

bool getTypeColor(char ch, Type& type, Color& color) {
    switch (ch) {
    case 'R':
        type = Type::ROOK;
        color = Color::BLACK;
        return true;
    case 'N':
        type = Type::KNIGHT;
        color = Color::BLACK;
        return true;
    case 'B':
        type = Type::BISHOP;
        color = Color::BLACK;
        return true;
    case 'Q':
        type = Type::QUEEN;
        color = Color::BLACK;
        return true;
    case 'K':
        type = Type::KING;
        color = Color::BLACK;
        return true;
    case 'P':
        type = Type::PAWN;
        color = Color::BLACK;
        return true;
    case 'r':
        type = Type::ROOK;
        color = Color::WHITE;
        return true;
    case 'n':
        type = Type::KNIGHT;
        color = Color::WHITE;
        return true;
    case 'b':
        type = Type::BISHOP;
        color = Color::WHITE;
        return true;
    case 'q':
        type = Type::QUEEN;
        color = Color::WHITE;
        return true;
    case 'k':
        type = Type::KING;
        color = Color::WHITE;
        return true;
    case 'p':
        type = Type::PAWN;
        color = Color::WHITE;
        return true;
    }
    return false;
}
