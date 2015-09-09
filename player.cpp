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
    int depth = 2;
    tree<GameState> decisionTree = Player::buildTree(pState, depth);

    return minmax(decisionTree, depth-1, false);
}

GameState Player::minmax(tree<GameState> decisionTree, int depth, bool opponentPlays) {

    std::cerr << "Minmax starting" << std::endl;
    // Do we stop looking for any nodes of does the tree still have children to explore ?
    //if(depth == 0 || decisionTree.is_valid(decisionTree.begin().begin())) {
    if(depth == 0) {
        return *(decisionTree.begin());
    } else if(!opponentPlays) { // Main player plays
        std::cerr << "Main player plays" << std::endl;
        tree<GameState>::sibling_iterator sib = decisionTree.begin().begin();
        GameState bestGS = *(decisionTree.begin().begin());
        int maxVal = Player::heuristics(bestGS), currVal;

        // Iterate through children
        while(sib != decisionTree.begin().end()) {
            currVal = Player::heuristics(Player::minmax(*sib, depth - 1, true));
            if(currVal > maxVal) {
                maxVal = currVal;
                bestGS = *sib;
            }
            ++sib;
        }

        return bestGS;
    } else { // Opponent plays
        tree<GameState>::sibling_iterator sib = decisionTree.begin().begin();
        GameState worstGS = *(decisionTree.begin().begin());
        int minVal = Player::heuristics(worstGS), currVal;

        // Iterate through children
        while(sib != decisionTree.begin().end()) {
            currVal = Player::heuristics(Player::minmax(*sib, depth - 1, false));
            if(currVal < minVal) {
                minVal = currVal;
                worstGS = *sib;
            }
            ++sib;
        }

        return worstGS;
    }
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
        tree<GameState> deeperTree = Player::buildTree((*currNode).reversed(), depth - 1);
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

    deltaPieces = 0;
    kings = 0;
    oppKings = 0;

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

    std::cerr << deltaPieces << std::endl;

    // Combining linearly the results
    return 
        deltaPieces +
        kings * 2 +
        oppKings * 2;
}

/*namespace checkers*/ }
