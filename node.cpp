#include "node.hpp"
#include "heuristics.hpp"

namespace checkers {

Node::Node() {
    this->gameState = GameState();
    this->alpha = -Heuristics::INFINITY;
    this->beta = Heuristics::INFINITY;
}

Node::Node(GameState gs) {
    this->gameState = gs;
    this->alpha = -Heuristics::INFINITY;
    this->beta = Heuristics::INFINITY;
}

void Node::mkChildren() {
    std::vector<GameState> lNextStates;
    this->gameState.findPossibleMoves(lNextStates);

    for(GameState gs : lNextStates) {
        Node n(gs);
        children.push_back(n);
    }
}

void Node::mkTree(int depth, bool mainPlayer) {
    if(depth > 0) {
        std::vector<GameState> lNextStates;

        if(mainPlayer) {
            this->gameState.findPossibleMoves(lNextStates);
        } else {
            this->gameState.reversed().findPossibleMoves(lNextStates);
        }

        for(GameState gs : lNextStates) {
            if(mainPlayer) {
                Node n(gs);
                n.mkTree(depth - 1, !mainPlayer);
                children.push_back(n);
            } else {
                Node n(gs.reversed());
                n.mkTree(depth - 1, !mainPlayer);
                children.push_back(n);
            }
        }
    }
}

// For debug purposes
void Node::toString() {
    this->toString(0);
}

void Node::toString(int d) {
    std::cerr << this->gameState.toMessage() << " - Eval = " 
              << Heuristics::evaluate(this->gameState) << std::endl;

    for(Node n : this->children) {
        for(int i = 0 ; i < d + 1 ; ++i)
            std::cerr << "\t";
        n.toString(d + 1);
    }
}

}
