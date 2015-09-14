#ifndef _CHECKERS_HEURISTICS_HPP_
#define _CHECKERS_HEURISTICS_HPP_

#include "gamestate.hpp"
#include "node.hpp"


namespace checkers {
extern int nodesSeen;

class Heuristics {
    public:
        static const int     PIECE            = 100;
        static const int     KING             = 200;
        static const int     OPPPIECE         = 40;
        static const int     OPPKING          = 100;
        static constexpr int COEFFROWS[8]     = {15, 8, 8, 2, 7, 2, 4, 10};
        static constexpr int COEFFROWS_OPP[8] = {10, 4, 2, 7, 2, 8, 8, 15};
        static constexpr int COEFFCOLS[8]     = {10, 0, 0, 0, 0, 0, 0, 10};

        static const int INFINITY = 100000;
        static const int DEPTH    = 6;

        static int minmax(Node, bool, Node&, bool, int, int);
        static int evaluate(GameState);
};

}

#endif
