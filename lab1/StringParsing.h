#pragma once
#include <vector>
#include <regex>
#include <queue>
#include "Node.h"
#include "Leaf.h"
#include "myLess.h"

using std::vector;
using std::string;

class StringParsing {
private:
	string text;
	string split;
public:
	//StringParsing(string h) { delimiter = h; };
	StringParsing(string &s, string p) { text = s; split = p; }
	//void setSlit(string &s) { slit = s; }
	void Strtok(priority_queue<Node*, vector<Node*>, myLess> &);
};

