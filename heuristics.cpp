#include "heuristics.hpp"

namespace checkers {

int nodesSeen = 0;

constexpr int Heuristics::COEFFROWS[8];
constexpr int Heuristics::COEFFROWS_OPP[8];
constexpr int Heuristics::COEFFCOLS[8];

// Top level minmax
// Special treatment for not returning a value but a gamestate
GameState Heuristics::topMinmax(Node root, const Deadline &pDue) {
   if(root.children.empty()) {
      return root.gameState;
   } else {
       int maxVal = -Heuristics::INFINITY;
       int currVal;
       GameState bestGS;
       int alpha = -Heuristics::INFINITY, beta = Heuristics::INFINITY;

       for(Node n : root.children) {
           currVal = Heuristics::minmax(n, false, pDue, alpha, beta);

           if(currVal > maxVal) {
               maxVal = currVal;
               bestGS = n.gameState;
           }
       }

       std::vector<pid_t> pids(root.children.size());
       std::vector<int> vals(root.children.size());
       int val;

       for(unsigned int i = 0 ; i < root.children.size() ; ++i) {
           pids[i] = vfork();

           if(pids[i] == 0) {
               val = Heuristics::minmax(root.children[i], false, pDue, alpha, beta);
               exit(val);
           }
       }

       for(unsigned int i = 0 ; i < root.children.size() ; ++i) {
           wait(&val);
           vals[i] = val;
       }

       for(unsigned int i = 0 ; i < root.children.size() ; ++i) {
           currVal = vals[i];
           if(currVal > maxVal) {
               maxVal = currVal;
               bestGS = root.children[i].gameState;
           }
       }

       return bestGS;
   } 
}

int Heuristics::minmax(Node root, bool color, const Deadline &pDue,
        int alpha, int beta) {

    if(root.children.empty()) {
        ++nodesSeen;
        return Heuristics::evaluate(root.gameState, Player::color); 
    } else {

        if(color) { // Main player : maximizing
            int maxVal = -Heuristics::INFINITY;
            int currVal;

            for(Node n : root.children) {
                currVal = Heuristics::minmax(n, !color, pDue, alpha, beta);
                if(currVal > maxVal) {
                    maxVal = currVal;

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
                currVal = Heuristics::minmax(n, !color, pDue, alpha, beta);
                if(currVal < minVal) {
                    minVal = currVal;

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
    if((pieces - oppPieces) < 4 || oppPieces > 4) {
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
