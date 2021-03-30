#pragma once
#include <string>
using namespace std;

class Product {
private:
	string name;
	double price;
	string hex;
	string barcode; //key
public:
	Product() { name = ""; price = 0.0; hex = ""; barcode = ""; }
	Product(string n, double p, string h, string b){ name = n; price = p; hex = h; barcode = b; }
	string getName() const { return name; }
	double getPrice() const { return price; }
	string getHex() const { return hex; }
	string getBarcode() const { return barcode; }
};

