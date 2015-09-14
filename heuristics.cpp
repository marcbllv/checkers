#include "heuristics.hpp"

namespace checkers {

static int nbreNode;

// This function is called by the player. It returns the best state,
// using the negamax function below
GameState Heuristics::bestGameState(Node root) {

    nbreNode = 0;

    if(root.children.empty()) {
        return root.gameState;
    } else { // Perform negamax and return the best gamestate
        int depth = Heuristics::DEPTH;
        int alpha = -Heuristics::INFINITY, beta = Heuristics::INFINITY;

        GameState bestGS = root.children.front().gameState;
        int maxVal = -Heuristics::negamax(bestGS, depth - 1, true, alpha, beta), currVal;

        for(Node n : root.children) {
            currVal = -negamax(n, depth - 1, true, alpha, beta);
            if(currVal > maxVal) {
                maxVal = currVal;
                bestGS = n.gameState;
            }
        }
        return bestGS;
    }
}

int Heuristics::negamax(Node root, int depth, bool color, int alpha, int beta) {

    if(root.children.empty()) {
        return Heuristics::evaluate(root.gameState) * (color ? -1 : 1);
    } else {
        int maxVal = -Heuristics::INFINITY, currVal;

        for(Node n : root.children) {
            currVal = -negamax(n, depth - 1, !color, -beta, -alpha);

            // No pruning
            if(currVal > maxVal) {
                maxVal = currVal;
            }

            // With alpha-beta pruning
            if(currVal > maxVal) {
                maxVal = currVal;
                if(maxVal > alpha) {
                    alpha = maxVal;
                    if(maxVal > beta) {
                        return maxVal;
                    }
                }
            }
        }

        return maxVal;
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
    int coeffRow[8]    = {15, 8, 8, 2, 7, 2, 4, 10};
    int coeffRowOpp[8] = {10, 4, 2, 7, 2, 8, 8, 15};

    int pCol[8]        = {0, 0, 0, 0, 0, 0, 0, 0};
    int pColOpp[8]     = {0, 0, 0, 0, 0, 0, 0, 0};
    int coeffCol[8]    = {10, 0, 0, 0, 0, 0, 0, 10};

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
        evaluation += pRow[i] * coeffRow[i];
        evaluation -= pRowOpp[i] * coeffRowOpp[i];
    }

    // Columns coeffs
    for(int i = 0 ; i < 8 ; i++) {
        evaluation += pCol[i] * coeffCol[i];
        evaluation -= pColOpp[i] * coeffCol[i];
    }

    evaluation = 
        pieces * 100 - oppPieces * 40 +
        kings * 200 - oppKings * 100;

    // Infinite score (or -Infinite) if it's a winning or losing game state
    evaluation += (oppPieces == 0 ? Heuristics::INFINITY : 0);
    evaluation -= (pieces == 0 ? Heuristics::INFINITY : 0);

    return evaluation;
}

}
