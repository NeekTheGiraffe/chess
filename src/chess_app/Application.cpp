#include "Application.h"
#include "utils.h"

#include <chess/constants.h>
#include <SDL3_image/SDL_image.h>

#include <iostream>

const int BOARD_DISPLAY_MARGIN = 20;
const float LEGAL_MOVE_SQUARE_RATIO = 0.4;
const Chess::Type PROMOTION_BUTTON_ORDER[] =
{
    Chess::Type::QUEEN, Chess::Type::KNIGHT, Chess::Type::ROOK, Chess::Type::BISHOP,
};
const int N_PROMOTION_BUTTONS = 4;

int pieceToSprite(Chess::Type t, Chess::Color c);

Application::Application()
    : m_selector(m_game),
      m_pieceSpritesheet(m_sdl.renderer, "assets/chess_pieces.png", 2, 6),
      m_cancelButtonSprite(m_sdl.renderer, "assets/cancel_button.png", 1, 1),
      m_mousePos({ 0, 0 })
{
}

void Application::loop()
{
    SDL_Event e;

    for (;;)
    {
        if (SDL_PollEvent(&e))
        {
            if (e.type == SDL_EVENT_QUIT)
                return;
            if (e.type == SDL_EVENT_MOUSE_BUTTON_DOWN && e.button.button == SDL_BUTTON_LEFT)
            {
                if (m_selector.inPromotion())
                {
                    Chess::Type promoteTo;
                    if (pointInCancelButton(m_mousePos))
                        m_selector.cancelPromotion();
                    else if (pointInPromotionButton(m_mousePos, promoteTo))
                        m_selector.promote(promoteTo);
                }
                else
                    m_selector.selectPiece(getBoardIndex(m_mousePos));
            }
            if (e.type == SDL_EVENT_MOUSE_BUTTON_UP && e.button.button == SDL_BUTTON_LEFT)
                m_selector.releasePiece(getBoardIndex(m_mousePos));
            if (e.type == SDL_EVENT_MOUSE_MOTION)
                m_mousePos = { e.motion.x, e.motion.y };
        }

        renderWindow();
    }
}

void Application::renderWindow()
{
    SDL_SetRenderDrawColor(m_sdl.renderer, 255, 255, 255, 255);
    SDL_RenderClear(m_sdl.renderer);

    SDL_FRect chessBoard = boardDimensions();
    float displayWidthPerGrid = chessBoard.w / Chess::BOARD_WIDTH;
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
        const Chess::Piece& p = m_game.getPiece(i);
        if (!p.alive || m_selector.selectedPieceId() == i)
            continue;
        m_pieceSpritesheet.render(m_sdl.renderer, pieceToSprite(p.type, p.color), boundingRect(m_game.getPiece(i).position));
    }

    // Legal moves and selected piece
    if (m_selector.isPieceSelected() && !m_selector.inPromotion())
    {
        SDL_SetRenderDrawBlendMode(m_sdl.renderer, SDL_BLENDMODE_BLEND);
        SDL_SetRenderDrawColor(m_sdl.renderer, 0, 0, 255, 192);
        for (const int i : m_game.legalMoves(m_selector.selectedPieceId()))
        {
            SDL_FRect r = boundingRect(i);
            r.x += r.w * (1. - LEGAL_MOVE_SQUARE_RATIO) / 2;
            r.y += r.h * (1. - LEGAL_MOVE_SQUARE_RATIO) / 2;
            r.w *= LEGAL_MOVE_SQUARE_RATIO;
            r.h *= LEGAL_MOVE_SQUARE_RATIO;
            SDL_RenderFillRect(m_sdl.renderer, &r);
        }
        m_pieceSpritesheet.render(
            m_sdl.renderer,
            pieceToSprite(m_selector.selectedPiece().type, m_selector.selectedPiece().color),
            { m_mousePos.x - displayWidthPerGrid / 2.f, m_mousePos.y - displayWidthPerGrid / 2.f, displayWidthPerGrid, displayWidthPerGrid }
        );
    }

    // Promotion UI
    if (m_selector.inPromotion())
    {
        SDL_SetRenderDrawColor(m_sdl.renderer, 255, 255, 255, 255);
        int promotionSquare = m_selector.promotionSquare();
        int r = Chess::rank(promotionSquare), f = Chess::file(promotionSquare);
        int direction = r == Chess::RANK_8 ? -1 : 1;
        for (int i = 0; i < N_PROMOTION_BUTTONS; i++)
        {
            SDL_FRect rect = boundingRect(Chess::space(r + direction * i, f));
            SDL_RenderFillRect(m_sdl.renderer, &rect);
            m_pieceSpritesheet.render(
                m_sdl.renderer,
                pieceToSprite(PROMOTION_BUTTON_ORDER[i], m_selector.selectedPiece().color),
                rect
            );
        }
        SDL_FRect cancelButton = boundingRect(Chess::space(r + direction * N_PROMOTION_BUTTONS, f));
        
        SDL_RenderFillRect(m_sdl.renderer, &cancelButton);
        m_cancelButtonSprite.render(m_sdl.renderer, 0, cancelButton);
    }

    SDL_RenderPresent(m_sdl.renderer);
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
    float gridWidth = boardRect.w / Chess::BOARD_WIDTH;
    
    if (!SDL_PointInRectFloat(&screenCoords, &boardRect))
        return -1;

    int rank = Chess::BOARD_WIDTH - 1 - (int)((screenCoords.y - boardRect.y) / gridWidth);
    int file = (screenCoords.x - boardRect.x) / gridWidth;

    return rank * Chess::BOARD_WIDTH + file;
}

SDL_FRect Application::boundingRect(int index) const
{
    int rank = index / Chess::BOARD_WIDTH;
    int file = index % Chess::BOARD_WIDTH;
    SDL_FRect board = boardDimensions();
    float gridWidth = board.w / Chess::BOARD_WIDTH;
    return
    {
        file * gridWidth + board.x,
        (Chess::BOARD_WIDTH - rank - 1) * gridWidth + board.y,
        gridWidth,
        gridWidth
    };
}

bool Application::pointInCancelButton(const SDL_FPoint& pt) const
{
    int promotionSquare = m_selector.promotionSquare();
    int r = Chess::rank(promotionSquare), f = Chess::file(promotionSquare);
    int direction = r == Chess::RANK_8 ? -1 : 1;
    return SDL_PointInRectFloat(&pt, &boundingRect(Chess::space(r + direction * N_PROMOTION_BUTTONS, f)));
}

bool Application::pointInPromotionButton(const SDL_FPoint& pt, Chess::Type& promoteTo) const
{
    int promotionSquare = m_selector.promotionSquare();
    int r = Chess::rank(promotionSquare), f = Chess::file(promotionSquare);
    int direction = r == Chess::RANK_8 ? -1 : 1;
    for (int i = 0; i < N_PROMOTION_BUTTONS; i++)
    {
        if (SDL_PointInRectFloat(&pt, &boundingRect(Chess::space(r + direction * i, f))))
        {
            promoteTo = PROMOTION_BUTTON_ORDER[i];
            return true;
        }
    }
    return false;
}

int pieceToSprite(Chess::Type t, Chess::Color c)
{
    int baseIndex = 0;
    switch (t)
    {
    case Chess::Type::PAWN: baseIndex = 5; break;
    case Chess::Type::ROOK: baseIndex = 4; break;
    case Chess::Type::KNIGHT: baseIndex = 3; break;
    case Chess::Type::BISHOP: baseIndex = 2; break;
    case Chess::Type::QUEEN: baseIndex = 1; break;
    case Chess::Type::KING: baseIndex = 0; break;
    }
    return c == Chess::Color::BLACK ? baseIndex + 6 : baseIndex;
}
