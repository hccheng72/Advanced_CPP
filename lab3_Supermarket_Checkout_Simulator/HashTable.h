#pragma once
#include "Product.h"
#include "HashNode.h"
#include <memory>

//template<class ItemType>
class HashTable {
private:
	unique_ptr<HashNode[]> hashAry;
	int hashSize;
	int count;
public:
	HashTable() {};
	HashTable(int n) { count = 0; hashSize = n;	hashAry = unique_ptr<HashNode[]>(new HashNode[hashSize]); }
	void setSize(int n) { count = 0; hashSize = n;	hashAry = unique_ptr<HashNode[]>(new HashNode[hashSize]); }
	int getCount() const { return count; }
	int getSize() const { return hashSize; }
	double getLoadFactor() const { return 100.0 * count / hashSize; }
	bool isEmpty() const { return count == 0; }
	bool isFull()  const { return count == hashSize; }
	bool insert(const Product& itemIn);
	bool search(Product& itemOut, const string& key);
	int key_to_index(const string& k);
};