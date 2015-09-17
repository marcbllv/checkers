#ifndef _CHECKERS_HEURISTICS_HPP_
#define _CHECKERS_HEURISTICS_HPP_

#include "gamestate.hpp"
#include "node.hpp"
#include "deadline.hpp"

namespace checkers {
extern int nodesSeen;

class Heuristics {
    public:
        static const int     PIECE        = 60;
        static const int     KING         = 80;
        static const int     OPPPIECE     = 60;
        static const int     OPPKING      = 80;
        static const int     BACKPIECE    = 12;
        static constexpr int COEFFROWS[8] = {0, 4, 6, 7, 7, 8, 8, 18};
        static constexpr int COEFFCOLS[8] = {10, 0, 0, 0, 0, 0, 0, 10};

        static constexpr unsigned int CENTER[6]   = {13, 14, 17, 18, 21, 22};
        static constexpr unsigned int MAINDIAG[8] = {3, 7, 10, 14, 17, 21, 24, 28};
        static const int CENTRALKING              = 20;
        static const int MAINDIAGKINGS            = 20;

        static const int INFINITY = 100000;
        static const int DEPTH    = 8;

        static GameState topMinmax(GameState, const Deadline&);
        static int minmax(GameState root, bool color, int depth,
                const Deadline &pDue, int alpha, int beta);
        static int evaluate(GameState, uint8_t);
};

}

#endif
