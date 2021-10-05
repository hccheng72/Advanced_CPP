#include "QTree.h"

void QTree::seeQueue(priority_queue<Node*, vector<Node*>, myLess> pq) {
	for (int i = 0; i < 15; i++) {
		cout << pq.top()->getSymbol() << "_";
		pq.pop();
	}
	cout << endl;
}

Node* QTree::makeQTree(priority_queue<Node*, vector<Node*>, myLess> &pq) {
	while (pq.size() > 1) {
		//debugging
		//if (pq.size()>15)
			//seeQueue(pq);

		Node* n1 = pq.top();
		//cout << n1->getSymbol() << endl;
		pq.pop();
		Node* n2 = pq.top();
		//cout << n2->getSymbol() << endl;
		pq.pop();
		Node* newItem = new Branch(n1, n2);
		pq.push(newItem);
	}
	root = pq.top();
	return root;
}

void QTree::destroyTree(Node* nodePtr) {
	if (nodePtr) // != NULL
	{
		if (dynamic_cast<Branch*>(nodePtr) != nullptr) {
			destroyTree(dynamic_cast<Branch*>(nodePtr)->getLeft());
			destroyTree(dynamic_cast<Branch*>(nodePtr)->getRight());
			delete nodePtr;
		}
		else
			delete nodePtr;
	}
}