#include "player.hpp"
#include <cstdlib>
#include <queue>
#include "node.hpp"
#include "heuristics.hpp"

namespace checkers {

GameState Player::play(const GameState &pState,const Deadline &pDue) {
    //std::cerr << "Processing " << pState.toMessage() << std::endl;

    std::vector<GameState> lNextStates;
    pState.findPossibleMoves(lNextStates);

    if (lNextStates.size() == 0) return GameState(pState, Move());

    // Building tree
    Node root(pState);
    root.mkTree(Heuristics::DEPTH, true);

    GameState g = Heuristics::bestGameState(root);
    return g;
}

/*namespace checkers*/ }
