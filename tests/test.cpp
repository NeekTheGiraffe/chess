#include <chess/Board.h>
#include <chess/Game.h>
#include <yaml-cpp/yaml.h>

#include <iostream>
#include <fstream>
#include <sstream>
#include <unordered_set>
#include <algorithm>

#define TEST_ASSERT(cond, error_msg) if (!(cond)) {\
        std::cout << error_msg << std::endl;\
        std::exit(1);\
    }

struct Testcase
{
    Chess::Board board;
    Chess::Color toMove;
    int lastMoveSrc;
    int lastMoveDest;
    std::unordered_set<int> expectedMoves[Chess::NUM_SPACES];

    Testcase(const YAML::Node& document);
};

std::string display(int space);
std::vector<int> setDifference(const std::unordered_set<int>& lhs, const std::unordered_set<int>& rhs);

int main(int argc, char** argv) {
    if (argc < 2)
    {
        std::cout << "Must enter a filename" << std::endl;
        std::exit(1);
    }
    YAML::Node document = YAML::LoadFile(argv[1]);
    std::cout << "Found testcase file" << std::endl;
    Testcase testcase(document);
    Chess::Game game(testcase.board, testcase.toMove, testcase.lastMoveDest, testcase.lastMoveSrc);

    for (int pos = 0; pos < Chess::BOARD_WIDTH * Chess::BOARD_WIDTH; pos++) {
        int pieceId = game.getPieceId(pos);
        if (pieceId == -1)
            continue;
        std::vector<int> expectedNotFound =
            setDifference(testcase.expectedMoves[pos], game.legalMoves(pieceId));
        std::vector<int> foundNotExpected =
            setDifference(game.legalMoves(pieceId), testcase.expectedMoves[pos]);
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

int parseSpace(const std::string& space)
{
    TEST_ASSERT(space.size() == 2, "Space \"" + space + "\" must be length 2");
    TEST_ASSERT(space[0] >= 'a' && space[0] <= 'h',
        "File '" + std::string(1, space[0]) + "' in \"" + space + "\" must be between a-h");
    TEST_ASSERT(space[1] >= '1' && space[1] <= '8',
        "Rank '" + std::string(1, space[1]) + "' in \"" + space + "\" must be between 1-8");
    int file = space[0] - 'a';
    int rank = space[1] - '1';
    //std::cout << "Success " << (space[1] * Chess::BOARD_WIDTH + space[0]) << std::endl;
    return rank * Chess::BOARD_WIDTH + file;
}
std::vector<int> getMoved(const YAML::Node& document)
{
    std::vector<int> result;
    YAML::Node node = document["moved"];
    if (!node.IsDefined())
        return result;
    auto moved = node.as<std::vector<std::string>>();
    for (const std::string& space : moved)
        result.push_back(parseSpace(space));
    return result;
}
int parseSpaceFromYaml(const YAML::Node& document, const std::string& key)
{
    YAML::Node field = document[key];
    return field.IsDefined() ? parseSpace(field.as<std::string>()) : -1;
}

Testcase::Testcase(const YAML::Node& document)
    : board(document["board"].as<std::string>(), getMoved(document))
{
    std::string _toMove = document["to-move"].as<std::string>();
    TEST_ASSERT(_toMove == "white" || _toMove == "black", "to-move must be \"white\" or \"black\"");
    toMove = _toMove == "white" ? Chess::Color::WHITE : Chess::Color::BLACK;
    std::cout << "toMove: " << _toMove << std::endl;

    const YAML::Node& lastMove = document["last-move"];
    if (lastMove.IsDefined())
    {
        lastMoveDest = parseSpaceFromYaml(lastMove, "dest");
        lastMoveSrc = parseSpaceFromYaml(lastMove, "src");
        std::cout << "lastMove: " << lastMoveSrc << " -> " << lastMoveDest << std::endl;
    }
    else
    {
        lastMoveDest = -1;
        lastMoveSrc = -1;
    }

    YAML::Node legalMoves = document["legal-moves"];
    for (const auto& moveset : legalMoves)
    {
        int src = parseSpace(moveset.first.as<std::string>());
        auto dests = moveset.second.as<std::vector<std::string>>();
        for (const std::string& destStr : dests)
            expectedMoves[src].insert(parseSpace(destStr));
    }
}
