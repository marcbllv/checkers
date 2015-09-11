#include "player.hpp"
#include <cstdlib>
#include <queue>
#include "node.hpp"

namespace checkers {

static int globalCount = 0;

GameState Player::play(const GameState &pState,const Deadline &pDue) {
    //std::cerr << "Processing " << pState.toMessage() << std::endl;

    std::vector<GameState> lNextStates;
    pState.findPossibleMoves(lNextStates);

    if (lNextStates.size() == 0) return GameState(pState, Move());

    // Building tree
    int depth = 4;
    Node root(pState);
    root.mkTree(depth, true);

    ++globalCount;
    std::cerr << "Tour n° : " << globalCount << std::endl;

    return Player::minmax(root, depth, true);
}


GameState Player::minmax(Node root, int depth, bool max) {

    if(depth == 0 || root.children.empty()) {
        return root.gameState;
    } else if(max) { // Main player plays
        GameState bestGS = root.children.front().gameState;
        int maxVal = Player::heuristics(bestGS), currVal;

        for(std::vector<Node>::iterator node = root.children.begin() ; 
                node != root.children.end() ; ++node) {
            currVal = Player::heuristics(Player::minmax(*node, depth - 1, false));
            if(currVal > maxVal) {
                maxVal = currVal;
                bestGS = (*node).gameState;
            }
        }
        return bestGS;

    } else { // Opponent plays -> minimizing
        GameState worstGS = root.children.front().gameState;
        int minVal = Player::heuristics(worstGS), currVal;

        for(std::vector<Node>::iterator node = root.children.begin() ; 
                node != root.children.end() ; ++node) {
            currVal = Player::heuristics(Player::minmax(*node, depth - 1, false));
            if(currVal < minVal) {
                minVal = currVal;
                worstGS = (*node).gameState;
            }
        }
        return worstGS;
    }
}

int Player::heuristics(GameState gs) {

    /*
     * Counting pieces
     * Counting kings
     */
    int evalutation;
    int pieces, kings;
    int oppPieces, oppKings;
    uint8_t currP;

    pieces = 0;
    kings = 0;
    oppPieces = 0;
    oppKings = 0;

    // Looping through board and counting
    for(int i = 0 ; i < 32 ; i++) {
        currP = gs.at(GameState::cellToRow(i), GameState::cellToCol(i));
        if(currP & CELL_WHITE) {
            ++pieces;

            if(currP & CELL_KING) {
                kings++;
            }
        } else if(currP & CELL_RED) {
            ++oppPieces;

            if(currP & CELL_KING) {
                oppKings++;
            }
        }
    }

    evalutation = 
        pieces - oppPieces +
        kings * 2 +
        oppKings * 2
        + 5000 * (oppPieces == 0);

    // Combining linearly the results
    return  evalutation;
}

/*namespace checkers*/ }
