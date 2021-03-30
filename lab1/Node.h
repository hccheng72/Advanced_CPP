#pragma once
#include <string>
using namespace std;

class Node {
public:
	virtual float getFreq() = 0;
	virtual string getSymbol() = 0; 
};

