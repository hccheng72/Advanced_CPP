#include <string>
#include <iostream>
#include <vector>
#include <memory>
#include <algorithm>
#include <iomanip>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <regex>
#include <queue>
#include <fstream>
using namespace std;

mutex laneMu;
condition_variable laneCV;
mutex cartMu;
mutex dataMu;
mutex coutMu;

struct Cart {
	string cartNum;
	vector<string> products;
};

class Product {
private:
	string name;
	double price;
	string hex;
	string barcode; //key
public:
	Product() { name = ""; price = 0.0; hex = ""; barcode = ""; }
	Product(string n, double p, string h, string b) { name = n; price = p; hex = h; barcode = b; }
	string getName() const { return name; }
	double getPrice() const { return price; }
	string getHex() const { return hex; }
	string getBarcode() const { return barcode; }
};

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
	bool insert(const Product &itemIn);
	bool search(Product &itemOut, const string &key);
	int key_to_index(const string &k);
};

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
		sum += k[i] * (int)pow(2, i);
	return sum % hashSize;
};


template <class T>
class LaneThreaded {
public:
	void add(T item);
	T remove();
	int getLaneSize() { return _lane.size(); }
	LaneThreaded() {}
private:
	queue<T> _lane;
	const unsigned int _size = 10;
	mutex laneMu;
	condition_variable laneCV;
};

template <class T>
void LaneThreaded<T>::add(T item) {
	while (true) {
		//add only when carts less than 10 in a lane
		unique_lock<mutex> locker(laneMu);
		laneCV.wait(locker, [this]() {return _lane.size() < _size; });
		_lane.push(item);
		locker.unlock();
		laneCV.notify_all();
		return;
	}
}

template <class T>
T LaneThreaded<T>::remove()
{
	while (true) {
		//checkout only when there're carts in a lane
		unique_lock<mutex> locker(laneMu);
		laneCV.wait(locker, [this]() {return _lane.size() > 0; });
		T back = _lane.front();
		_lane.pop();
		locker.unlock();
		laneCV.notify_all();
		return back;
	}
}


class Files {
private:
	string filename;
	//ifstream file;
public:
	Files(string s) { filename = s; }
	void readFile(vector<string>& bs, vector<double>& pc);
	void readFile(queue<Cart>& cart);
};

void Files::readFile(vector<string>& bs, vector<double>& pc) {
	ifstream file(filename);
	regex barcode("<Barcode>(\\d*)</Barcode>");
	regex price("<Price>(\\d*.\\d{2})</Price>");

	string buffer = "";
	if (file) {
		while (!file.eof()) {
			getline(file, buffer);
			if (buffer.length() > 0) {
				smatch match;
				if (regex_search(buffer, match, barcode)) {
					bs.push_back(move(match[1]));
				}
				else if (regex_search(buffer, match, price)) {
					pc.push_back(move(stod(match[1])));
				}
				buffer = "";
			}
		}
	}
	else {
		cout << "Unable to open file:  " << filename << endl;
	}
	file.close();
}

void Files::readFile(queue<Cart>& cart) {
	ifstream file(filename);
	int count = 1;
	Cart aCart;
	string buffer = "";
	if (file) {
		while (!file.eof()) {
			getline(file, buffer);
			if (buffer.length() > 0) {
				if (count % 2 == 1) {
					aCart.cartNum = buffer;
				}
				else if (count % 2 == 0) {
					vector<string> products;
					sregex_token_iterator end;  // default value is npos
					regex pattern("[0-9A-F]{12}");
					int count = 0;
					for (sregex_token_iterator pos(buffer.begin(), buffer.end(), pattern); pos != end; ++pos)
					{
						if ((*pos).length() > 0)
						{
							if ((static_cast<string>(*pos))[0] != 0x20)  //0x20=whitespace
							{
								products.push_back(*pos);
							}
						}
					}
					aCart.products = products;
					cart.push(move(aCart));
				}

				count++;
				buffer = "";

			}
		}
	}
	else {
		cout << "Unable to open file:  " << filename << endl;
	}
	file.close();
}


class Supermarket {
private:
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
	void checkout(shared_ptr<LaneThreaded<Cart>> &);
	string createHex(string& bs);
	void decToHexDictionary();
	string createName(string& bs);
	void charDictionary();
	void createHashTable(int size);
	int findNextPrime(int n);
	int isPrime(int nn);
};

Supermarket::Supermarket(string x, string c) {
	xmlFileName = x;
	cartFileName = c;
	//build dictionaries
	decToHexDictionary();
	charDictionary();

	//read xml
	Files xmlFile(xmlFileName);
	xmlFile.readFile(bs, price);

	//read carts
	Files cartFile(cartFileName);
	cartFile.readFile(carts);
	//cout << carts.size(); //997
	//make notes and hash table
	createHashTable(findNextPrime(bs.size()));
}

void Supermarket::addCartToLane(shared_ptr<LaneThreaded<Cart>> &lanePtr) {
	cartMu.lock();
	while (carts.size() > 0) { //with mutex
		lanePtr->add(carts.front());
		carts.pop();
		//cout << carts.size() << endl;
	}
	cartMu.unlock();
}

void Supermarket::checkout(shared_ptr<LaneThreaded<Cart>> &lanePtr) {
	while (carts.size() > 0 && lanePtr->getLaneSize() > 0) {
		Cart aCart;
		aCart = lanePtr->remove();
		double sum = 0.0;
		Product find;

		for_each(aCart.products.begin(), aCart.products.end(),
			[&](string s) {
			dataMu.lock();
			if (ht.search(find, s)) {
				sum += find.getPrice();
			}
			else
				cout << "product can't be found!" << endl;
			dataMu.unlock();
		});

		coutMu.lock();
		cout << aCart.cartNum << " has been checked out with total expenses $" << sum << endl;
		coutMu.unlock();
	}
}

void Supermarket::decToHexDictionary() {
	hexDic = {
		pair<string, char>("0000", '0'),
		pair<string, char>("0001", '1'),
		pair<string, char>("0010", '2'),
		pair<string, char>("0011", '3'),
		pair<string, char>("0100", '4'),
		pair<string, char>("0101", '5'),
		pair<string, char>("0110", '6'),
		pair<string, char>("0111", '7'),
		pair<string, char>("1000", '8'),
		pair<string, char>("1001", '9'),
		pair<string, char>("1010", 'A'),
		pair<string, char>("1011", 'B'),
		pair<string, char>("1100", 'C'),
		pair<string, char>("1101", 'D'),
		pair<string, char>("1110", 'E'),
		pair<string, char>("1111", 'F')
	};
}

string Supermarket::createHex(string& bs) {
	string hex = "";
	unsigned int pos = 0;
	while (pos < bs.size()) {
		string fourbits = bs.substr(pos, 4);
		auto itr = find_if(hexDic.begin(), hexDic.end(),
			[&](pair<string, char> apair) {return apair.first == fourbits; });
		if (itr != hexDic.end()) {
			hex += (*itr).second;
		}
		pos += 4;
	}
	return hex;
}

void Supermarket::charDictionary() {
	charDic =
	{
		pair<string, char>("011000100", ' '),
		pair<string, char>("011000100", ' '),
		pair<string, char>("010000101", '-'),
		pair<string, char>("010001010", '+'),
		pair<string, char>("010101000", '$'),
		pair<string, char>("000101010", '%'),
		pair<string, char>("010010100", '*'),
		pair<string, char>("110000100", '.'),
		pair<string, char>("010100010", '/'),
		pair<string, char>("000110100", '0'),
		pair<string, char>("100100001", '1'),
		pair<string, char>("001100001", '2'),
		pair<string, char>("101100000", '3'),
		pair<string, char>("000110001", '4'),
		pair<string, char>("100110000", '5'),
		pair<string, char>("001110000", '6'),
		pair<string, char>("000100101", '7'),
		pair<string, char>("100100100", '8'),
		pair<string, char>("001100100", '9'),
		pair<string, char>("100001001", 'A'),
		pair<string, char>("001001001", 'B'),
		pair<string, char>("101001000", 'C'),
		pair<string, char>("000011001", 'D'),
		pair<string, char>("100011000", 'E'),
		pair<string, char>("001011000", 'F'),
		pair<string, char>("000001101", 'G'),
		pair<string, char>("100001100", 'H'),
		pair<string, char>("001001100", 'I'),
		pair<string, char>("000011100", 'J'),
		pair<string, char>("100000011", 'K'),
		pair<string, char>("001000011", 'L'),
		pair<string, char>("101000010", 'M'),
		pair<string, char>("000010011", '0'),
		pair<string, char>("100010010", 'O'),
		pair<string, char>("001010010", 'P'),
		pair<string, char>("000000111", 'Q'),
		pair<string, char>("100000110", 'R'),
		pair<string, char>("001000110", 'S'),
		pair<string, char>("000010110", 'T'),
		pair<string, char>("110000001", 'U'),
		pair<string, char>("011000001", 'V'),
		pair<string, char>("111000000", '1'),
		pair<string, char>("010010001", 'X'),
		pair<string, char>("110010000", 'y'),
		pair<string, char>("011010000", 'Z')
	};
}

string Supermarket::createName(string& bs) {
	string name;
	unsigned int pos = 0;
	while (pos + 3 < bs.size()) {
		string ninebits = bs.substr(pos, 9);
		auto itr = find_if(charDic.begin(), charDic.end(),
			[&](pair<string, char> apair) {return apair.first == ninebits; });
		if (itr != charDic.end()) {
			name += (*itr).second;
		}
		pos += 9;
	}
	return name;
}

void Supermarket::createHashTable(int size) {
	ht.setSize(size);
	for (unsigned int i = 0; i < bs.size(); i++) {
		string name = createName(bs[i]);
		string hex = createHex(bs[i]);
		Product aProduct(name, price[i], hex, bs[i]);
		ht.insert(aProduct);
	}
}

int Supermarket::findNextPrime(int n) {
	if (n <= 1)
		return 2;

	bool found = false;
	while (!found) {
		n++;
		if (isPrime(n))
			found = true;
	}
	return n;
}

int Supermarket::isPrime(int nn) {
	if (nn < 2)
		return false;
	for (int i = 2; i <= ceil(sqrt(nn)); i++) {
		if (nn % i == 0 && nn != i)
			return false;
	}
	return true;
}


int main() {
	Supermarket afile("ProductPrice.xml", "Carts.csv");
	
	vector<shared_ptr<LaneThreaded<Cart>>> lanes(5, shared_ptr<LaneThreaded<Cart>>(new LaneThreaded<Cart>));
	thread addcart[5];
	thread check[5];
	for (int i = 0; i < 5; i++) {
		addcart[i] = thread(&Supermarket::addCartToLane, ref(afile), ref(lanes[i]));
	}
	for (int i = 0; i < 5; i++) {
		check[i] = thread(&Supermarket::checkout, ref(afile), ref(lanes[i]));
	}

	for (auto& t : addcart)
		t.join();
	for (auto& t : check)
		t.join();
	
	return 0;
}