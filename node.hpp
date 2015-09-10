#ifndef _CHECKERS_NODE_HPP
#define _CHECKERS_NODE_HPP

#include "gamestate.hpp"
#include <vector>

namespace checkers {

class Node {
    public:
        GameState gs;
        std::vector<Node> children;

        Node(const GameState);
        void mkChildren();
        void mkTree(int);
};

void Node::mkChildren() {
    std::vector<GameState> lNextStates;
    gs.findPossibleMoves(lNextStates);

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

Node::Node(const GameState gameState) {
    this->gs = gameState;
}

}

#endif
