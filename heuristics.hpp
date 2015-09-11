#ifndef _CHECKERS_HEURISTICS_HPP_
#define _CHECKERS_HEURISTICS_HPP_

#include "gamestate.hpp"

namespace checkers {

class Heuristics {
    private:
        const int PIECE = 10;
        const int KING  = 50;
        const int WIN   = 10000;
        const int OPPPIECE = -10;
        const int OPPKING = -50;
        const int 
    public:
        static int evaluate(GameState);
};

}

#endif
