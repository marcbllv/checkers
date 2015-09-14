#include "heuristics.hpp"

namespace checkers {

constexpr int Heuristics::COEFFROWS[8];
constexpr int Heuristics::COEFFROWS_OPP[8];
constexpr int Heuristics::COEFFCOLS[8];

int Heuristics::minmax(Node root, bool color, 
        Node &bestNode, bool updateBestNode, int alpha, int beta) {

    if(root.children.empty()) {
        ++nodesSeen;
        return Heuristics::evaluate(root.gameState);
    } else {

        if(color) { // Main player : maximizing
            int maxVal = -Heuristics::INFINITY;
            int currVal;

            for(Node n : root.children) {
                currVal = Heuristics::minmax(n, !color, bestNode, false, alpha, beta);
                if(currVal > maxVal) {
                    maxVal = currVal;

                    // To get the best node at the top of the tree
                    if(updateBestNode) {
                        bestNode.gameState = n.gameState;
                    }

                    // Alpha cut ?
                    if(alpha < maxVal) {
                        alpha = maxVal;
                        if(alpha >= beta) {
                            return maxVal;
                        }
                    }
                }
            }

            return maxVal;
        } else {
            int minVal = Heuristics::INFINITY;
            int currVal;

            for(Node n : root.children) {
                currVal = Heuristics::minmax(n, !color, bestNode, false, alpha, beta);
                if(currVal < minVal) {
                    minVal = currVal;

                    // To get the best node at the top of the tree
                    if(updateBestNode) {
                        bestNode.gameState = n.gameState;
                    }

                    // Beta cut ?
                    if(beta > minVal) {
                        beta = minVal;
                        if(alpha >= beta) {
                            return minVal;
                        }
                    }
                }
            }

            return minVal;
        }
    }
}

int Heuristics::evaluate(GameState gs) {

    /*
     * Counting pieces
     * Counting kings
     */
    int evaluation;
    int pieces, kings;
    int oppPieces, oppKings;

    int pRow[8]        = {0, 0, 0, 0, 0, 0, 0, 0};
    int pRowOpp[8]     = {0, 0, 0, 0, 0, 0, 0, 0};

    int pCol[8]        = {0, 0, 0, 0, 0, 0, 0, 0};
    int pColOpp[8]     = {0, 0, 0, 0, 0, 0, 0, 0};

    uint8_t currP;

    evaluation = 0;
    pieces = 0;
    kings = 0;
    oppPieces = 0;
    oppKings = 0;

    // Looping through board and counting
    for(int i = 0 ; i < 32 ; i++) {
        currP = gs.at(GameState::cellToRow(i), GameState::cellToCol(i));
        if(currP & CELL_WHITE) {
            pieces++;
            pCol[GameState::cellToCol(currP)] += 1;

            if(currP & CELL_KING) {
                kings++;
            } else { // Normal piece
                pRow[GameState::cellToRow(currP)] += 1;
            }
        } else if(currP & CELL_RED) {
            oppPieces++;

            if(currP & CELL_KING) {
                oppKings++;
            } else { // Normal piece
                pRowOpp[GameState::cellToRow(currP)] += 1;
            }
        }
    }

    // Combining row coeffs
    for(int i = 0 ; i < 8 ; i++) {
        evaluation += pRow[i] * Heuristics::COEFFROWS[i];
        evaluation -= pRowOpp[i] * Heuristics::COEFFROWS_OPP[i];
    }

    // Columns coeffs
    for(int i = 0 ; i < 8 ; i++) {
        evaluation += pCol[i] * Heuristics::COEFFCOLS[i];
        evaluation -= pColOpp[i] * Heuristics::COEFFCOLS[i];
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
