#pragma once
#include<queue>
#include "Node.h"
#include "Branch.h"
#include "myLess.h"
#include <iostream>//
using namespace std;

class QTree {
private:
	Node* root;
public:
	QTree():root(NULL) {}
	void seeQueue(priority_queue<Node*, vector<Node*>, myLess> pq);
	Node* makeQTree(priority_queue<Node*, vector<Node*>, myLess> &pq);
	Node* getRoot() const { return root; }
	void destroyTree(Node* nodePtr);
	~QTree() { destroyTree(root); }
};

