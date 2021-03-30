#pragma once
#include "Product.h"
#include "Files.h"
#include "HashTable.h"
#include "HashNode.h"
#include <string>
#include <iostream>
#include <vector>
#include <memory>
#include <algorithm>
#include <iomanip>
using namespace std;

class Decrypt {
private:

	string xmlFileName;
	string cartFileName;
	vector<string> bs; //barcode
	vector<double> price;
	vector<pair<string, vector<string>>> carts;
	HashTable ht;
	vector<pair<string, char>> charDic;
	vector<pair<string, char>> hexDic;
public:
	Decrypt(string, string);
	//string getDecryption(Node*);
	string createHex(string& bs);
	void decToHexDictionary();
	string createName(string& bs);
	void charDictionary();
	void createHashTable(int size);
	int findNextPrime(int n);
	int isPrime(int nn);
};

