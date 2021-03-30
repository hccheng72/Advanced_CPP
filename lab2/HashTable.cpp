#include "HashTable.h"

bool HashTable::insert(const Product &itemIn)
{
	if (count == hashSize)
		return false;

	HashNode newItem(itemIn);
	int bucket = key_to_index(itemIn.getHex());
	int nc = 0;
	while (hashAry[bucket].getOccupied()) {
		bucket = (bucket + 1) % hashSize;
		nc++;
	}

	hashAry[bucket] = newItem;
	hashAry[bucket].setNoCollisions(nc);
	count++;

	return true;
}

bool HashTable::search(Product &itemOut, const string &key) {
	int bucket = key_to_index(key), bucketProbed = 0;

	while (bucketProbed < hashSize) {
		if (hashAry[bucket].getItem().getHex() == key) {
			itemOut = hashAry[bucket].getItem();
			return true;
		}
		bucket = (bucket + 1) % hashSize;
		bucketProbed++;
	}

	return false;
}

int HashTable::key_to_index(const string &k) {
	int sum = 0;
	for (int i = 0; k[i]; i++)
		sum += k[i]*pow(2,i);
	return sum % hashSize;
};