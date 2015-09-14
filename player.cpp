#include "player.hpp"
#include <cstdlib>
#include <queue>
#include "node.hpp"
#include "heuristics.hpp"

namespace checkers {

int nodesSeen = 0;

GameState Player::play(const GameState &pState,const Deadline &pDue) {
    nodesSeen = 0;

    std::vector<GameState> lNextStates;
    pState.findPossibleMoves(lNextStates);

    if (lNextStates.size() == 0) return GameState(pState, Move());

    // Building tree
    Node root(pState);
    root.mkTree(Heuristics::DEPTH, true);

    // Computing minmax algorithm
    Node bestNode;
    int alpha = -Heuristics::INFINITY, beta = Heuristics::INFINITY;
    Heuristics::minmax(root, true, bestNode, true, alpha, beta);

    std::cerr << nodesSeen << " nodes seen" << std::endl;

    return bestNode.gameState;
}

/*namespace checkers*/ }
