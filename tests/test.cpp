#include <chess/Board.h>
#include <chess/Game.h>

#include <iostream>
#include <fstream>
#include <sstream>
#include <unordered_set>
#include <algorithm>

#define TEST_ASSERT(cond, error_msg) if (!(cond)) {\
        std::cout << error_msg << std::endl;\
        std::exit(1);\
    }

Chess::Board parseBoard(std::ifstream& infile);
Chess::Color parseToMove(std::ifstream& infile);
void parseExpectedLegalMoves(std::ifstream& infile, std::unordered_set<int>* expectedLegalMoves);
std::string display(int space);
std::string err(int line, int col, const std::string& expected, char found)
{
    std::stringstream ss;
    ss << "Line ";
    ss << line;
    ss << ", Column ";
    ss << col;
    ss << ": expected ";
    ss << expected;
    ss << " but found ";
    ss << found;
    return ss.str();
}
std::vector<int> setDifference
(
    const std::unordered_set<int>& lhs,
    const std::unordered_set<int>& rhs
)
{
    std::vector<int> result;
    std::copy_if(lhs.begin(), lhs.end(), std::back_inserter(result),
        [&rhs](int needle) { return rhs.find(needle) == rhs.end(); });
    return result;
}

int main(int argc, char** argv) {
    if (argc < 2)
    {
        std::cout << "Must enter a filename" << std::endl;
        std::exit(1);
    }

    std::ifstream infile(argv[1]);
    TEST_ASSERT(infile.good(), "Error reading file");
    Chess::Board board = parseBoard(infile);
    Chess::Color toMove = parseToMove(infile);
    std::unordered_set<int> expectedLegalMoves[Chess::NUM_SPACES];
    parseExpectedLegalMoves(infile, expectedLegalMoves);
    int lastMove = -1;
    Chess::Game game(board, toMove, lastMove);

    for (int pos = 0; pos < Chess::BOARD_WIDTH * Chess::BOARD_WIDTH; pos++) {
        int pieceId = game.getPieceId(pos);
        if (pieceId == -1)
            continue;
        std::vector<int> expectedNotFound =
            setDifference(expectedLegalMoves[pos], game.legalMoves(pieceId));
        std::vector<int> foundNotExpected =
            setDifference(game.legalMoves(pieceId), expectedLegalMoves[pos]);
        if (expectedNotFound.size() > 0 || foundNotExpected.size() > 0)
            std::cout << display(pos) << std::endl;
        if (expectedNotFound.size() > 0)
        {
            std::cout << "Expected, not found: " << std::endl;
            for (int move : expectedNotFound)
                std::cout << display(move) << ", ";
            std::cout << std::endl;
        }
        if (foundNotExpected.size() > 0)
        {
            std::cout << "Found, not expected: " << std::endl;
            for (int move : foundNotExpected)
                std::cout << display(move) << ", ";
            std::cout << std::endl;
        }
        TEST_ASSERT(expectedNotFound.size() == 0 && foundNotExpected.size() == 0,
            "Legal moves are not the same");
    }
    std::cout << std::endl;
}

std::string display(int space)
{
    std::string displayed;
    displayed.push_back(space % Chess::BOARD_WIDTH + 'a');
    displayed.push_back(space / Chess::BOARD_WIDTH + '1');
    return displayed;
}

Chess::Board parseBoard(std::ifstream& infile) {
    std::stringstream boardStream;
    char line[10];
    for (int i = 0; i < 8; i++)
    {
        infile.getline(line, 10);
        boardStream << line;
    }
    Chess::Board board(boardStream.str());
    return board;
}

Chess::Color parseToMove(std::ifstream& infile) {
    char ch = infile.get();
    infile.get();
    if (ch == 'w')
        return Chess::Color::WHITE;
    else if (ch == 'b')
        return Chess::Color::BLACK;
    std::cout << "Unexpected character when parsing color to move" << std::endl;
    std::exit(1);
}

void parseExpectedLegalMoves(std::ifstream& infile, std::unordered_set<int>* expectedLegalMoves) {
    char buf[1024];
    int line = 9;
    while (infile.getline(buf, 1024))
    {
        line++;
        if (buf[0] == '\0')
            break;
        TEST_ASSERT(buf[0] >= 'a' && buf[0] <= 'h', err(line, 0, "a-h", buf[0]));
        TEST_ASSERT(buf[1] >= '1' && buf[1] <= '8', err(line, 1, "1-8", buf[1]));
        int srcRank = buf[1] - '1';
        int srcFile = buf[0] - 'a';
        
        int destFile = -1;
        for (int i = 6; i < 1024 && buf[i] != '\0'; i++)
        {
            if (buf[i] == ' ' || buf[i] == ',')
                continue;
            if (destFile == -1)
            {
                TEST_ASSERT(buf[i] >= 'a' && buf[i] <= 'h', err(line, i, "a-h", buf[i]));
                destFile = buf[i] - 'a';
            }
            else
            {
                TEST_ASSERT(buf[i] >= '1' && buf[i] <= '8', err(line, i, "1-8", buf[i]));
                int destRank = buf[i] - '1';
                expectedLegalMoves[srcRank * Chess::BOARD_WIDTH + srcFile].insert(destRank * Chess::BOARD_WIDTH + destFile);
                destFile = -1;
            }
        }
    }
}
