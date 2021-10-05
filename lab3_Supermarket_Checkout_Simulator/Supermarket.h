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
#include <thread>
#include <mutex>
#include <condition_variable>
#include "LaneThreaded.h"
#include "Cart.h"
#include "AddCartToLane.h"
using namespace std;

class Supermarket {
private:
	mutex cartMu;
	mutex dataMu;
	mutex coutMu;
	string xmlFileName;
	string cartFileName;
	vector<string> bs; //barcode
	vector<double> price;
	queue<Cart> carts;
	HashTable ht;
	vector<pair<string, char>> charDic;
	vector<pair<string, char>> hexDic;
public:
	Supermarket(string, string);
	void addCartToLane(shared_ptr<LaneThreaded<Cart>>& lanePtr);
	void checkout(shared_ptr<LaneThreaded<Cart>>&);
	string createHex(string& bs);
	void decToHexDictionary();
	string createName(string& bs);
	void charDictionary();
	void createHashTable(int size);
	int findNextPrime(int n);
	int isPrime(int nn);
};

