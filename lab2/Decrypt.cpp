#include "Decrypt.h"

Decrypt::Decrypt(string x, string c) {
	xmlFileName = x;
	cartFileName = c;
	//build dictionaries
	decToHexDictionary();//
	charDictionary();//
	
	//read xml
	Files xmlFile(xmlFileName);
	xmlFile.readFile(bs, price);
	//cout << "bs.size: " << bs.size() << ",bs.size: " << price.size() << endl;
	//cout << "1st item: " <<  bs[0] << " " << price[0] << endl;
	//read carts
	Files cartFile(cartFileName);
	cartFile.readFile(carts);
	//cout << "carts.size: " << carts.size() << endl;
	//cout << "num of products in 1st cart: " << carts[0].second.size() << endl;
	//cout << carts[0].first << endl;
	//for_each(carts[0].second.begin(), carts[0].second.end(), [](string s) {cout << s << endl; });
	//make notes and hash table
	
	createHashTable(findNextPrime(bs.size()));
	//cout << "ht size: " << ht.getSize() << endl;
	//cout << "ht load: " << ht.getLoadFactor() << endl;
	
	
	for (int i = 0; i < carts.size(); i++) { 
		cout << carts[i].first << ":" << endl;
		Product find;
		double sum = 0.0;
		for_each(carts[i].second.begin(), carts[i].second.end(), 
			[&](string s) {
			if (ht.search(find, s)) {
				cout << "\t" << left << setw(5) << find.getName() << " $" << setprecision(2) << fixed << find.getPrice() << endl;
				sum += find.getPrice();
			}
			else {
				cout << "product can't be found!" << endl;
			}
		});
		cout << "\t" << "Total:$" << sum << endl;
	}
	
}

void Decrypt::decToHexDictionary() {
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

string Decrypt::createHex(string& bs) {
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

void Decrypt::charDictionary() {
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

string Decrypt::createName(string& bs) {
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

void Decrypt::createHashTable(int size) {
	ht.setSize(size);
	for (unsigned int i = 0; i < bs.size(); i++) {
		string name = createName(bs[i]);
		string hex = createHex(bs[i]);
		Product aProduct(name, price[i], hex, bs[i]);
		ht.insert(aProduct);
	}
}

int Decrypt::findNextPrime(int n) {
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

int Decrypt::isPrime(int nn) {
	if (nn < 2)
		return false;
	for (int i = 2; i <= ceil(sqrt(nn)); i++) {
		if (nn % i == 0 && nn != i)
			return false;
	}
	return true;
}