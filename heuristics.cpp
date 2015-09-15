#include "heuristics.hpp"
#include "player.hpp"

namespace checkers {

int nodesSeen = 0;

constexpr int Heuristics::COEFFROWS[8];
constexpr int Heuristics::COEFFROWS_OPP[8];
constexpr int Heuristics::COEFFCOLS[8];

int Heuristics::minmax(Node root, bool color, const Deadline &pDue,
        Node &bestNode, bool updateBestNode, int alpha, int beta) {

    if(root.children.empty()) {
        return Heuristics::evaluate(root.gameState, Player::color); 
    } else {

        if(color) { // Main player : maximizing
            int maxVal = -Heuristics::INFINITY, bestNodeIdx = 0;
            int currVal;

            for(unsigned int i = 0 ; i < root.children.size() ; ++i) {
                // If it's too late : return best value seen so far
                //if(pDue.getSeconds() - Deadline::now().getSeconds() < 0.8) {
                //    ++nodesSeen;
                //    break;
                //}

                currVal = Heuristics::minmax(root.children[i], !color, pDue, bestNode, false, alpha, beta);
                if(currVal > maxVal) {
                    maxVal = currVal;
                    bestNodeIdx = i;

                    // Alpha cut ?
                    if(alpha < maxVal) {
                        alpha = maxVal;
                        if(alpha >= beta) {
                            return maxVal;
                        }
                    }
                }
            }

            // To get the best node at the top of the tree
            if(updateBestNode) {
                bestNode.gameState = root.children[bestNodeIdx].gameState;
            }

            return maxVal;
        } else {
            int minVal = Heuristics::INFINITY, worstNodeIdx = 0;
            int currVal;

            for(unsigned int i = 0 ; i < root.children.size() ; ++i) {
                // If it's too late : return best value seen so far
                //if(pDue.getSeconds() - Deadline::now().getSeconds() < 0.8) {
                //    ++nodesSeen;
                //    break;
                //}

                currVal = Heuristics::minmax(root.children[i], !color, pDue, bestNode, false, alpha, beta);
                if(currVal < minVal) {
                    minVal = currVal;
                    worstNodeIdx = i;

                    // Beta cut ?
                    if(beta > minVal) {
                        beta = minVal;
                        if(alpha >= beta) {
                            return minVal;
                        }
                    }
                }
            }

            // To get the best node at the top of the tree
            if(updateBestNode) {
                bestNode.gameState = root.children[worstNodeIdx].gameState;
            }

            return minVal;
        }
    }
}

int Heuristics::evaluate(GameState gs, uint8_t color) {

    uint8_t CELL_PLAYER, CELL_OPP;

    if(color == CELL_WHITE) {
        CELL_PLAYER = CELL_WHITE;
        CELL_OPP    = CELL_RED;
    } else {
        CELL_PLAYER = CELL_RED;
        CELL_OPP    = CELL_WHITE;
    }

    /*
     * Counting pieces
     * Counting kings
     */
    int evaluation;
    int pieces, kings;
    int oppPieces, oppKings;

    int pRow[8]    = {0, 0, 0, 0, 0, 0, 0, 0};
    int pRowOpp[8] = {0, 0, 0, 0, 0, 0, 0, 0};

    int pCol[8]    = {0, 0, 0, 0, 0, 0, 0, 0};
    int pColOpp[8] = {0, 0, 0, 0, 0, 0, 0, 0};

    uint8_t currP;

    evaluation     = 0;
    pieces         = 0;
    kings          = 0;
    oppPieces      = 0;
    oppKings       = 0;

    // Looping through board and counting
    for(int i = 0 ; i < 32 ; i++) {
        currP = gs.at(GameState::cellToRow(i), GameState::cellToCol(i));
        if(currP & CELL_PLAYER) {
            pieces++;
            pCol[GameState::cellToCol(currP)] += 1;

            if(currP & CELL_KING) {
                kings++;
            } else { // Normal piece
                pRow[GameState::cellToRow(currP)] += 1;
            }
        } else if(currP & CELL_OPP) {
            oppPieces++;

            if(currP & CELL_KING) {
                oppKings++;
            } else { // Normal piece
                pRowOpp[GameState::cellToRow(currP)] += 1;
            }
        }
    }

    // Early / mid game
    if((pieces - oppPieces) < 3 || oppPieces > 4) {
        // Combining row coeffs
        for(int i = 0 ; i < 8 ; i++) {
            evaluation += pRow[i] * Heuristics::COEFFROWS[i];
        }

        // Columns coeffs
        for(int i = 0 ; i < 8 ; i++) {
            evaluation += pCol[i] * Heuristics::COEFFCOLS[i];
            evaluation -= pColOpp[i] * Heuristics::COEFFCOLS[i];
        }
    }

    evaluation = 
        pieces * Heuristics::PIECE - oppPieces * Heuristics::OPPPIECE +
        kings * Heuristics::KING - oppKings * Heuristics::OPPKING;

    // Infinite score (or -Infinity) if it's a winning or losing game state
    evaluation += (oppPieces == 0 ? Heuristics::INFINITY : 0);
    evaluation -= (pieces == 0 ? Heuristics::INFINITY : 0);

    return evaluation;
}

}
