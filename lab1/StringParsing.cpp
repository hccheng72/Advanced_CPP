#include "StringParsing.h"
#include <iostream>

void StringParsing::Strtok(priority_queue<Node*, vector<Node*>, myLess> &pq) {
	//sregex_token_iterator end;  // default value is npos
	regex pattern(split);
	smatch match;
	if (regex_search(text, match, pattern)) {
		//cout << match[1].str() << " " << match[2] << endl;
		Node* newNode = new Leaf(match[1].str(), stof(match[2]));
		pq.push(newNode);
		//cout << newNode->getSymbol() << " " << newNode->getFreq() << endl;
	}
}