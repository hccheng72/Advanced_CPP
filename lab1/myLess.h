#pragma once
#include "Node.h"
#include "Leaf.h"
#include <iostream>//

struct myLess {
	bool operator() (Node* n1, Node*n2) {
		return (n1->getFreq() > n2->getFreq());
			//return true;
		
		/*if (fabs(n1->getFreq() - n2->getFreq()) < 0.0001) {
			if (((Leaf*)(n1))->getSymbol() > ((Leaf*)(n2))->getSymbol()) {
				//cout << ((Leaf*)(n1))->getSymbol() << " " << ((Leaf*)(n2))->getSymbol() << endl;
				return true;
			}*/
		//}
		//return false;
	}
};