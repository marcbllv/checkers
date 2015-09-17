#include "player.hpp"
#include <cstdlib>
#include <queue>
#include "node.hpp"
#include "heuristics.hpp"

namespace checkers {

uint8_t Player::color = 0;

GameState Player::play(const GameState &pState, const Deadline &pDue) {
    if(pState.getNextPlayer() == CELL_RED) {
        Player::color = CELL_RED;
    } else {
        Player::color = CELL_WHITE;
    }

    nodesSeen = 0;

    std::vector<GameState> lNextStates;
    pState.findPossibleMoves(lNextStates);

    if(lNextStates.size() == 0) {
        return GameState(pState, Move());
    } else if(lNextStates.size() == 1) {
        return lNextStates.front();
    }

    // Building tree
    //Node root(pState);
    //root.mkTree(Heuristics::DEPTH, true);

    // Computing minmax algorithm
    GameState bestGS = Heuristics::topMinmax(pState, pDue);
    std::cerr << "Nodes seen : " << nodesSeen << std::endl;
    return bestGS;
}

/*namespace checkers*/ }
