#pragma once
#include "Node.h"

class Leaf: public Node {
private:
	float freq;
	string symbol;
public:
	//Leaf() {}
	Leaf(string s, float num) {
		freq = num;
		symbol = s;
	}
	string getSymbol() { return symbol; }
	float getFreq() { return freq; }
};

