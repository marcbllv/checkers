#include "player.hpp"
#include "tree.hh"
#include <cstdlib>
#include <queue>

namespace checkers {

GameState Player::play(const GameState &pState,const Deadline &pDue) {
    //std::cerr << "Processing " << pState.toMessage() << std::endl;

    std::vector<GameState> lNextStates;
    pState.findPossibleMoves(lNextStates);

    if (lNextStates.size() == 0) return GameState(pState, Move());

    // Building decision tree
    tree<GameState> decisionTree = Player::buildTree(pState);

    /* 
     * TODO
     * BFS & choose best game state
     */


    return lNextStates[rand() % lNextStates.size()];
}

tree<GameState> Player::buildTree(GameState initState, int depth) {

    tree<GameState> decisionTree = tree<GameState>(initState); 
    tree<GameState>::iterator root, currNode;
    root = decisionTree.begin();

    if(depth == 0) {
        return decisionTree;
    }

    std::vector<GameState> lNextStates;
    initState.findPossibleMoves(lNextStates);

    for(GameState st: lNextStates) {
        currNode = decisionTree.append_child(root, st);
        // Recursively building deeper nodes
        tree<GameState> deeperTree = Player::buildTree(*currNode, depth - 1);
        decisionTree.replace(currNode, deeperTree.begin());
    }

    return decisionTree;
}

int Player::heuristics(GameState gs) {

    /*
     * Counting pieces
     * Counting kings
     */
    int deltaPieces, kings, oppKings;
    uint8_t currP;

    // Looping through board and counting
    for(int i = 0 ; i < 32 ; i++) {
        currP = gs.at(GameState::cellToRow(i), GameState::cellToCol(i));
        if(currP & CELL_WHITE) {
            deltaPieces++;

            if(currP & CELL_KING) {
                kings++;
            }
        } else if(currP & CELL_RED) {
            deltaPieces--;

            if(currP & CELL_KING) {
                oppKings++;
            }
        }
    }

    // Combining linearly the results
    return 
        deltaPieces +
        kings * 2 +
        oppKings * 2;
}

/*namespace checkers*/ }
