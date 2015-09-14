#ifndef _CHECKERS_NODE_HPP_
#define _CHECKERS_NODE_HPP_

#include "gamestate.hpp"
#include <vector>

namespace checkers {

class Node {
    public:
        GameState gameState;
        std::vector<Node> children;
        int alpha;
        int beta;

        Node();
        Node(const GameState);
        void mkChildren();
        void mkTree(int, bool);

        // For debug purposes
        void toString();
        void toString(int);
};

}

#endif
