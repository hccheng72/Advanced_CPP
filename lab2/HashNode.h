#pragma once
#include "Product.h"
//template<class ItemType>
class HashNode {
private:
	Product item;
	int occupied;
	int noCollisions;

public:
	// constructors
	HashNode() { occupied = 0; noCollisions = 0; }
	HashNode(Product anItem) { item = anItem; occupied = 1; noCollisions = 0; }
	HashNode(Product anItem, int ocp, int nCol)
	{
		item = anItem; occupied = ocp; noCollisions = nCol;
	}
	// setters
	void setItem(const Product & anItem) { item = anItem; }
	void setOccupied(int ocp) { occupied = ocp; }
	void setNoCollisions(int nCol) { noCollisions = nCol; }

	// getters
	Product getItem() const { return item; }
	int getOccupied() const { return occupied; }
	int getNoCollisions() const { return noCollisions; }
};