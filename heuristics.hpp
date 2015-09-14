#ifndef _CHECKERS_HEURISTICS_HPP_
#define _CHECKERS_HEURISTICS_HPP_

#include "gamestate.hpp"
#include "node.hpp"

namespace checkers {

class Heuristics {
    private:
        const int PIECE    = 10;
        const int KING     = 50;
        const int WIN      = 10000;
        const int OPPPIECE = -10;
        const int OPPKING  = -50;

    public:
        static const int INFINITY = 100000;
        static const int DEPTH    = 6;

        static int evaluate(GameState);
        static GameState minmax(Node, int, bool);
        static int negamax(Node, int, bool, int, int);
        static GameState bestGameState(Node);
};

}

#endif
