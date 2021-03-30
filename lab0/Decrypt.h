#pragma once
#include "Dictionary.h"
#include "KeyValue.h"
#include "Files.h"
#include "StringParsing.h"

class Decrypt {
private:
	string filename;
	string delimiter;
	vector<string> slit;
	vector<string> encodeNums;
	vector<string> result;
public:
	Decrypt(string nm, string dl);
	int Atoi(string astring);
	string getDecryption(vector<string>::iterator begin, vector<string>::iterator end);
};

