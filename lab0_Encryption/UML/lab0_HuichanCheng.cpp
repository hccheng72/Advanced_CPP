/*
Encryption

This program will dncrypt a file which contains numbers representing 
encoded characters back to their decoded characters. The decrypted 
text is plain English.

Written By: Huichan Cheng
Instructor: Grant Larkin
IDE: MS VS 2017
*/

#include <string>
#include <iostream>
#include <fstream>
#include <vector>
using namespace std;


/*~*~*
  struct KeyValue
*~**/
template <class T1, class T2>
struct KeyValue {
	T1 key;
	T2 value;
	KeyValue() {}
	KeyValue(T1 i, T2 s) { key = i; value = s; }
};


/*~*~*
  Class Dictionary
*~**/
template <class T>
class Dictionary {
private:
	vector<T> data;
	int dataSize;
public:
	Dictionary(int n);
	int getDatasize() const { return dataSize; }
	T &operator[](const int &sub);
	void push_back(T &newItem);
};

template <class T>
Dictionary<T>::Dictionary(int n) {
	dataSize = n;
	data.resize(n);
}

template <class T>
T &Dictionary<T>::operator[](const int &sub) {
	if (sub >= 0 || sub < dataSize) {
		return data[sub];
	}
}

template <class T>
void Dictionary<T>::push_back(T &newItem) {
	data.push_back(newItem);
	dataSize++;
}


/*~*~*
  Files
*~**/
class Files {
private:
	string filename;
	ifstream file;
public:
	Files(string s) { filename = s; }
	void readFile(vector<string>& slist);
	void writeFile(string sfile, vector<string> sdata);
};

//#include "Files.h"
void Files::readFile(vector<string>& slist) {
	ifstream file(filename);
	string buffer = "";
	if (file) {
		while (!file.eof()) {
			getline(file, buffer);
			if (buffer.length() > 0) {
				slist.push_back(buffer);  // array notation
				buffer = "";
			}
		}
	}
	else {
		cout << "Unable to open file:  " << filename << endl;
	}
	file.close();
}

void Files::writeFile(string sfile, vector<string> sdata) {
	ofstream outfile(sfile);
	for (auto s : sdata)
		outfile << s << endl;
	cout << "\n" << sfile << " file has been created." << endl;
	outfile.close();
}


/*~*~*
  Class StringParsing
*~**/
class StringParsing {
private:
	string slit;
	string delimiter;
public:
	StringParsing(string h) { delimiter = h; };
	StringParsing(string &s, string h) { slit = s; delimiter = h; }
	void setSlit(string &s) { slit = s; }
	void Strtok(vector<string> &v);
};

//#include "StringParsing.h"
void StringParsing::Strtok(vector<string> &v) {
	string stemp;
	int start = 0;
	int index = slit.find(delimiter);
	
	while (index != string::npos) {
		stemp.assign(slit, start, index - start);
		v.push_back(stemp);
		start = index + delimiter.size();
		index = slit.find(delimiter, start);
	}
	
	if (start >= string::npos) {
		stemp.assign(slit, start, slit.length() - start);
		v.push_back(stemp);
	}
}


/*~*~*
  Class Decrypt
*~**/
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

//#include "Decrypt.h"
Decrypt::Decrypt(string nm, string dl) {
	filename = nm;
	delimiter = dl;
	Files file(filename);
	file.readFile(slit);
	StringParsing aslit(delimiter);
	for (auto i : slit) {
		aslit.setSlit(i);
		aslit.Strtok(encodeNums);

		string astring = getDecryption(encodeNums.begin(), encodeNums.end());
		result.push_back(astring);
	}
	file.writeFile("decrypted.txt", result);
}

int Decrypt::Atoi(string astring) {
	if (isdigit(astring[0]))
		return stoi(astring);
}

string Decrypt::getDecryption(vector<string>::iterator begin, vector<string>::iterator end) {
	Dictionary<KeyValue<int, string>> aDict(256);
	for (int i = 0; i < 256; i++) {
		aDict[i].key = i;
		aDict[i].value = string(1, i);
	}

	string word(1, Atoi(*begin));
	begin++;
	string result = word;
	string entry;
	int key;
	while (begin != end) {
		key = Atoi(*begin);
		if (key >= 0 || key < aDict.getDatasize()) {
			entry = aDict[key].value;
		}
		else if (key == aDict.getDatasize()) {
			entry = word + word[0];
		}
		else {
			return "Invalid key";
		}
		result += entry;
		KeyValue<int, string> newPair(aDict.getDatasize(), word + entry[0]);
		aDict.push_back(newPair);
		word = entry;
		begin++;
	}
	return result;
}


/*~*~*
  Main()
*~**/
//#include "Decrypt.h"
int main() {
	/*
	string filename, delimiter;

	cout << "which file should be decrypted? ex. 'Encrypted.txt' ";
	getline(cin, filename);
	cout << "what is the delimiter? ex.', ' ";
	cin.ignore();
	getline(cin, delimiter);
	*/
	Decrypt aDecrypt("Encrypted.txt", ", ");
	return 0;
}

