#pragma once
#include "Node.h"
#include "Leaf.h"
#include "Branch.h"
#include "QTree.h"
#include "Files.h"
#include "StringParsing.h"
#include "myLess.h"
#include "frequency.h"
#include <string>
#include <iostream>
using namespace std;


class Decrypt {
private:

	string binaryFile;
	string freqFile;
	vector<string> slit;
	priority_queue<Node*, vector<Node*>, myLess> frqTable;
	queue<int> bits;
	vector<string> result;
public:
	Decrypt(string, string);
	string getDecryption(Node*);
};

