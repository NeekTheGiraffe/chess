#include "Board.h"
#include "Game.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <unordered_map>
#include <unordered_set>

#define TEST_ASSERT(cond, error_msg) if (!(cond)) {\
        std::cout << error_msg << std::endl;\
        std::exit(1);\
    }

Board parseBoard(std::ifstream& infile);
Color parseToMove(std::ifstream& infile);
void parseExpectedLegalMoves(std::ifstream& infile, std::unordered_set<int>* expectedLegalMoves);

int main(int argc, char** argv) {
    if (argc < 2)
    {
        std::cout << "Must enter a filename" << std::endl;
        std::exit(1);
    }
    std::ifstream infile(argv[1]);
    TEST_ASSERT(infile.good(), "Error reading file");
    Board board = parseBoard(infile);
    Color toMove = parseToMove(infile);
    std::unordered_set<int> expectedLegalMoves[NUM_SPACES];
    parseExpectedLegalMoves(infile, expectedLegalMoves);
    int lastMove = -1;
    Game game(board, toMove, lastMove);
    for (int pos = 0; pos < BOARD_WIDTH * BOARD_WIDTH; pos++) {
        int pieceId = game.getPieceId(pos);
        if (pieceId != -1)
        {
            std::cout << (char)(pos % BOARD_WIDTH + 'a') << (char)(pos / BOARD_WIDTH + '1') << std::endl;
            std::cout << "pieceId: " << pieceId << std::endl;
            std::cout << "Legal: ";
            for (int legalMove : game.legalMoves(pieceId))
                std::cout << legalMove << ", ";
            std::cout << "\nExpected: ";
            for (int legalMove : expectedLegalMoves[pos])
                std::cout << legalMove << ", ";
            std::cout << "\n";
            TEST_ASSERT(game.legalMoves(pieceId) == expectedLegalMoves[pos],
                "Legal moves are not the same");
        }
    }
}

Board parseBoard(std::ifstream& infile) {
    std::stringstream boardStream;
    char line[10];
    for (int i = 0; i < 8; i++)
    {
        infile.getline(line, 10);
        boardStream << line;
    }
    Board board(boardStream.str());
    return board;
}

Color parseToMove(std::ifstream& infile) {
    char ch = infile.get();
    infile.get();
    if (ch == 'w')
        return Color::WHITE;
    else if (ch == 'b')
        return Color::BLACK;
    std::cerr << "Unexpected character when parsing color to move" << std::endl;
    std::exit(1);
}

void parseExpectedLegalMoves(std::ifstream& infile, std::unordered_set<int>* expectedLegalMoves) {
    char buf[1024];
    while (infile.getline(buf, 1024))
    {
        TEST_ASSERT(buf[0] >= 'a' && buf[0] <= 'h', "Expected a-h but found " + buf[0]);
        TEST_ASSERT(buf[1] >= '1' && buf[1] <= '8', "Expected 1-8 but found " + buf[1]);
        int srcRank = buf[1] - '1';
        int srcFile = buf[0] - 'a';
        
        int destFile = -1;
        for (int i = 6; i < 1024 && buf[i] != '\0'; i++)
        {
            if (buf[i] == ' ' || buf[i] == ',')
                continue;
            if (destFile == -1)
            {
                TEST_ASSERT(buf[i] >= 'a' && buf[i] <= 'h', "Expected a-h but found " + buf[i]);
                destFile = buf[i] - 'a';
            }
            else
            {
                TEST_ASSERT(buf[i] >= '1' && buf[i] <= '8', "Expected 1-8 but found " + buf[i]);
                int destRank = buf[i] - '1';
                expectedLegalMoves[srcRank * BOARD_WIDTH + srcFile].insert(destRank * BOARD_WIDTH + destFile);
                destFile = -1;
            }
        }
    }
}
