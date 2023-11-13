#include "Chess.h"

#include <algorithm>

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
}

int Chess::getPieceId(int space) const
{
    return m_board[space];
}


const Piece& Chess::getPiece(int pieceId) const
{
    return m_pieces[pieceId];
}

void Chess::movePiece(int pieceId, int space)
{
    if (m_board[space] != -1)
        m_pieces[m_board[space]].alive = false;
    m_board[m_pieces[pieceId].position] = -1;

    m_board[space] = pieceId;
    m_pieces[pieceId].position = space;
}



/*

for piece in pieces:
    moves[square(piece)] = piece.generate_moves()
return moves

*/
