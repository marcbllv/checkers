#ifndef _CHECKERS_NODE_HPP
#define _CHECKERS_NODE_HPP

#include "gamestate.hpp"
#include <vector>

namespace checkers {

class Node {
    public:
        GameState gameState;
        std::vector<Node> children;

        Node(const GameState);
        void mkChildren();
        void mkTree(int);
};

Node::Node(const GameState gs) {
    this->gameState = gs;
}

void Node::mkChildren() {
    std::vector<GameState> lNextStates;
    this->gameState.findPossibleMoves(lNextStates);

    for(GameState gs : lNextStates) {
        Node n(gs);
        children.push_back(n);
    }
}

void Node::mkTree(int depth) {
    if(depth > 0) {
        this->mkChildren();
        for(Node n : this->children) {
            n.mkTree(depth - 1);
        }
    }
}

}

#endif
