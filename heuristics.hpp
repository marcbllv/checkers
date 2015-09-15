#ifndef _CHECKERS_HEURISTICS_HPP_
#define _CHECKERS_HEURISTICS_HPP_

#include "gamestate.hpp"
#include "node.hpp"
#include "deadline.hpp"


namespace checkers {
extern int nodesSeen;

class Heuristics {
    public:
        static const int     PIECE            = 60;
        static const int     KING             = 200;
        static const int     OPPPIECE         = 50;
        static const int     OPPKING          = 200;
        static constexpr int COEFFROWS[8]     = {8, 1, 1, -5, 0, -5, -3, 3};
        static constexpr int COEFFROWS_OPP[8] = {3, -3, -5, 0, -5, 1, 1, 8};
        static constexpr int COEFFCOLS[8]     = {10, 0, 0, 0, 0, 0, 0, 10};

        static const int INFINITY = 100000;
        static const int DEPTH    = 6;

        static int minmax(Node, bool, const Deadline&, Node&, bool, int, int);
        static int evaluate(GameState, uint8_t);
};

}

#endif
