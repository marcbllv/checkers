#ifndef _CHECKERS_HEURISTICS_HPP_
#define _CHECKERS_HEURISTICS_HPP_

#include "gamestate.hpp"
#include "node.hpp"
#include "deadline.hpp"


namespace checkers {
extern int nodesSeen;

class Heuristics {
    public:
        static const int     PIECE        = 50;
        static const int     KING         = 200;
        static const int     OPPPIECE     = 80;
        static const int     OPPKING      = 200;
        static const int     BACKPIECE    = 12;
        static constexpr int COEFFROWS[8] = {10, 5, 3, 0, 0, 3, 5, 10};
        static constexpr int COEFFCOLS[8] = {10, 5, 3, 0, 0, 3, 5, 10};

        static constexpr unsigned int CENTER[6] = {13, 14, 17, 18, 21, 22};
        static constexpr unsigned int MAINDIAG[8] = {3, 7, 10, 14, 17, 21, 24, 28};
        static const int CENTRALKING      = 20;

        static const int INFINITY = 100000;
        static const int DEPTH    = 6;

        static GameState topMinmax(Node, const Deadline&);
        static int minmax(Node, bool, const Deadline&, int, int);
        static int evaluate(GameState, uint8_t);
};

}

#endif
