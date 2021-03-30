#pragma once
#include "Node.h"

class Branch: public Node {
private:
	Node* left;
	Node* right;
public:
	//Branch() {}
	Branch(Node* n1, Node* n2) : left(n1), right(n2) {}
	float getFreq() {
		return left->getFreq() + right->getFreq();
	}
	string getSymbol() {
		return left->getSymbol() + right->getSymbol();
	}
	Node* getLeft() const { return left; }
	Node* getRight() const { return right; }
};

