#ifndef _CHECKERS_NODE_HPP
#define _CHECKERS_NODE_HPP

#include <vector>

class Node {
    GameState gs;
    std::vector<Node> children;

    void mkChildren();
}


void Node::mkChildren() {
    std::vector<GameState> lNextStates;
    gs.findPossibleMoves(lNextStates);

    for(GameState gs : lNextStates) {
        Node n(gs);
        children.push_back(n);
    }
}

#endif
