#include "Application.h"
#include "utils.h"
#include "constants.h"

#include <SDL3_image/SDL_image.h>

#include <iostream>

const int BOARD_DISPLAY_MARGIN = 20;
const float LEGAL_MOVE_SQUARE_RATIO = 0.4;

int pieceToSprite(const Piece& p);

Application::Application()
    : m_spritesheet(m_sdl.renderer, "assets/chess_pieces.png", 2, 6)
{
}

void Application::loop()
{
    SDL_Event e;
    SDL_FPoint mousePos = { 0, 0 };

    for (;;)
    {
        if (SDL_PollEvent(&e))
        {
            if (e.type == SDL_EVENT_QUIT)
                return;
            if (e.type == SDL_EVENT_MOUSE_BUTTON_DOWN && e.button.button == SDL_BUTTON_LEFT)
                m_game.selectPieceAt(getBoardIndex(mousePos));
            if (e.type == SDL_EVENT_MOUSE_BUTTON_UP && e.button.button == SDL_BUTTON_LEFT)
                m_game.releasePieceAt(getBoardIndex(mousePos));
            if (e.type == SDL_EVENT_MOUSE_MOTION)
                mousePos = { e.motion.x, e.motion.y };
        }

        SDL_SetRenderDrawColor(m_sdl.renderer, 255, 255, 255, 255);
        SDL_RenderClear(m_sdl.renderer);

        SDL_FRect chessBoard = boardDimensions();
        float displayWidthPerGrid = chessBoard.w / BOARD_WIDTH;
        SDL_SetRenderDrawColor(m_sdl.renderer, 219, 168, 92, 255);
        SDL_RenderFillRect(m_sdl.renderer, &chessBoard);

        // Chess board
        SDL_SetRenderDrawColor(m_sdl.renderer, 87, 58, 14, 255);
        for (int i = 0; i < 64; i++)
        {
            int rank = i / 8;
            int file = i % 8;
            if ((rank + file) % 2 == 1)
                continue;
            SDL_RenderFillRect(m_sdl.renderer, &boundingRect(i));
        }

        if (m_game.lastMove() != -1)
        {
            SDL_SetRenderDrawColor(m_sdl.renderer, 0, 168, 157, 255);
            SDL_RenderFillRect(m_sdl.renderer, &boundingRect(m_game.lastMove()));
        }

        // Pieces
        for (int i = 0; i < 32; i++)
        {
            const Piece& p = m_game.getPiece(i);
            if (!p.alive || m_game.selectedPieceId() == i)
                continue;
            m_spritesheet.render(m_sdl.renderer, pieceToSprite(p), boundingRect(m_game.getPiece(i).position));
        }

        // Legal moves and selected piece
        if (m_game.isPieceSelected())
        {
            SDL_SetRenderDrawBlendMode(m_sdl.renderer, SDL_BLENDMODE_BLEND);
            SDL_SetRenderDrawColor(m_sdl.renderer, 0, 0, 255, 192);
            for (const int i : m_game.legalMoves())
            {
                SDL_FRect r = boundingRect(i);
                r.x += r.w * (1. - LEGAL_MOVE_SQUARE_RATIO) / 2;
                r.y += r.h * (1. - LEGAL_MOVE_SQUARE_RATIO) / 2;
                r.w *= LEGAL_MOVE_SQUARE_RATIO;
                r.h *= LEGAL_MOVE_SQUARE_RATIO;
                SDL_RenderFillRect(m_sdl.renderer, &r);
            }
            m_spritesheet.render(m_sdl.renderer, pieceToSprite(m_game.selectedPiece()), {mousePos.x - displayWidthPerGrid / 2.f, mousePos.y - displayWidthPerGrid / 2.f, displayWidthPerGrid, displayWidthPerGrid});
        }

        SDL_RenderPresent(m_sdl.renderer);
    }
}

SDL_FRect Application::boardDimensions() const
{
    int w, h;
    SDL_GetWindowSize(m_sdl.window, &w, &h);
    float min = w < h ? w : h;
    float boardDisplayWidth = min - 2 * BOARD_DISPLAY_MARGIN;
    float boardX = (w - boardDisplayWidth) / 2, boardY = (h - boardDisplayWidth) / 2;
    return { boardX, boardY, boardDisplayWidth, boardDisplayWidth };
}

int Application::getBoardIndex(const SDL_FPoint& screenCoords) const
{
    SDL_FRect boardRect = boardDimensions();
    float gridWidth = boardRect.w / BOARD_WIDTH;
    
    if (!SDL_PointInRectFloat(&screenCoords, &boardRect))
        return -1;

    int rank = BOARD_WIDTH - 1 - (int)((screenCoords.y - boardRect.y) / gridWidth);
    int file = (screenCoords.x - boardRect.x) / gridWidth;

    return rank * BOARD_WIDTH + file;
}

SDL_FRect Application::boundingRect(int index) const
{
    int rank = index / BOARD_WIDTH;
    int file = index % BOARD_WIDTH;
    SDL_FRect board = boardDimensions();
    float gridWidth = board.w / BOARD_WIDTH;
    return
    {
        file * gridWidth + board.x,
        (BOARD_WIDTH - rank - 1) * gridWidth + board.y,
        gridWidth,
        gridWidth
    };
}

int pieceToSprite(const Piece& p)
{
    int baseIndex = 0;
    switch (p.type)
    {
    case Type::PAWN: baseIndex = 5; break;
    case Type::ROOK: baseIndex = 4; break;
    case Type::KNIGHT: baseIndex = 3; break;
    case Type::BISHOP: baseIndex = 2; break;
    case Type::QUEEN: baseIndex = 1; break;
    case Type::KING: baseIndex = 0; break;
    }
    return p.color == Color::BLACK ? baseIndex + 6 : baseIndex;
}
