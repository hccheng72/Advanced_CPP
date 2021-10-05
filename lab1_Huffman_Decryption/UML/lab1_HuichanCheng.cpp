/*
Lab1 : Bit Decryption
Written By: Huichan Cheng
Instructor: Grant Larkin
IDE: MS VS 2017
*/
#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <bitset>
#include <queue>
#include <regex>
using namespace std;

/*~*~*
  class Node
*~**/
class Node {
public:
	virtual float getFreq() = 0;
	virtual string getSymbol() = 0;
};

/*~*~*
  class Branch
*~**/
class Branch : public Node {
private:
	Node* left;
	Node* right;
public:
	Branch(Node* n1, Node* n2) : left(n1), right(n2) {}
	float getFreq() {
		return left->getFreq() + right->getFreq();
	}
	string getSymbol() {
		return left->getSymbol() + right->getSymbol();
	}
	Node* getLeft() const { return left; }
	Node* getRight() const { return right; }
};

/*~*~*
  class Leaf
*~**/
class Leaf : public Node {
private:
	float freq;
	string symbol;
public:
	Leaf(string s, float num) {
		freq = num;
		symbol = s;
	}
	string getSymbol() { return symbol; }
	float getFreq() { return freq; }
};

/*~*~*
  struct myLess
*~**/
struct myLess {
	bool operator() (Node* n1, Node*n2) {
		return (n1->getFreq() > n2->getFreq());
	}
};

/*~*~*
  class QTree
*~**/
class QTree {
private:
	Node* root;
public:
	QTree() :root(NULL) {}
	Node* makeQTree(priority_queue<Node*, vector<Node*>, myLess> &pq);
	Node* getRoot() const { return root; }
	void destroyTree(Node* nodePtr);
	~QTree() { destroyTree(root); }
};

Node* QTree::makeQTree(priority_queue<Node*, vector<Node*>, myLess> &pq) {
	while (pq.size() > 1) {
		Node* n1 = pq.top();
		pq.pop();
		Node* n2 = pq.top();
		pq.pop();
		Node* newItem = new Branch(n1, n2);
		pq.push(newItem);
	}
	root = pq.top();
	return root;
}

void QTree::destroyTree(Node* nodePtr) {
	if (nodePtr) // != NULL
	{
		if (dynamic_cast<Branch*>(nodePtr) != nullptr) {
			destroyTree(dynamic_cast<Branch*>(nodePtr)->getLeft());
			destroyTree(dynamic_cast<Branch*>(nodePtr)->getRight());
			delete nodePtr;
		}
		else
			delete nodePtr;
	}
}

/*~*~*
  class StringParsing
*~**/
class StringParsing {
private:
	string text;
	string split;
public:
	StringParsing(string &s, string p) { text = s; split = p; }
	void Strtok(priority_queue<Node*, vector<Node*>, myLess> &);
};

void StringParsing::Strtok(priority_queue<Node*, vector<Node*>, myLess> &pq) {
	regex pattern(split);
	smatch match;
	if (regex_search(text, match, pattern)) {
		Node* newNode = new Leaf(match[1].str(), stof(match[2]));
		pq.push(newNode);
	}
}

/*~*~*
  class Files
*~**/
class Files {
private:
	string filename;
	//ifstream file;
public:
	Files(string s) { filename = s; }
	void readFile(vector<string>& slist);
	void ReadBinaryFile(queue<int> &bitq);
	void writeFile(string sfile, vector<string> sdata);
};

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

void Files::ReadBinaryFile(queue<int> &bitq) {
	streampos size = -1;
	unsigned char* memblock = NULL;
	ifstream file(filename, ios::in | ios::binary);
	if (file.is_open())
	{
		file.seekg(0, ios::end);
		size = file.tellg();
		//cout << size;
		memblock = new unsigned char[size];
		file.seekg(0, ios::beg);
		file.read((char*)memblock, size);
	}
	else
	{
		cerr << "Unable to open file:  " << filename << endl;
		//return -1;
	}
	file.close();

	for (int i = 0; i < size; i++) {
		bitset<8> bs(memblock[i]);
		for (unsigned j = 0; j < bs.size(); j++) {
			bitq.push(bs[bs.size() - 1 - j]);
		}
	}

	delete[] memblock;
}

void Files::writeFile(string sfile, vector<string> sdata) // or slist[]
{
	ofstream outfile(sfile);
	for (auto s : sdata)
		outfile << s << endl;
	cout << "\n" << sfile << " file has been created." << endl;
	outfile.close();
}

/*~*~*
  struct frequency
*~**/
struct frequency {
	char _c;
	float _f;
	frequency(char c, float f) { _c = c; _f = f; }
};

/*~*~*
  class Decrypt
*~**/
class Decrypt {
private:

	string binaryFile;
	string freqFile;
	vector<string> slit;
	priority_queue<Node*, vector<Node*>, myLess> frqTable;
	queue<int> bits;
	vector<string> result;
public:
	Decrypt(string, string);
	string getDecryption(Node*);
};

Decrypt::Decrypt(string bin, string fre) {
	binaryFile = bin;
	freqFile = fre;

	Files binfile(binaryFile);
	binfile.ReadBinaryFile(bits);

	/*
	Files freqfile(freqFile);
	freqfile.readFile(slit);
	//cout << slit.size() << " " << slit[50]; //size:51
	for (auto i : slit) {
		StringParsing aslit(i, "'(.)',([\\d]*)");
		aslit.Strtok(frqTable);
	}
	*/
	///*
	vector<frequency> vfrequency =
	{
	frequency(' ',8.0),
	frequency('!',0.0306942),
	frequency('"',0.000183067),
	frequency('#',0.00854313),
	frequency('$',0.00970255),
	frequency('%',0.00170863),
	frequency('&',0.00134249),
	frequency('\'',0.000122045),
	frequency('(',0.000427156),
	frequency(')',0.00115942),
	frequency('*',0.0241648),
	frequency('+',0.00231885),
	frequency(',',0.00323418),
	frequency('-',0.0197712),
	frequency('.',0.0316706),
	frequency('/',0.00311214),
	frequency('0',2.74381),
	frequency('1',4.35053),
	frequency('2',3.12312),
	frequency('3',2.43339),
	frequency('4',1.94265),
	frequency('5',1.88577),
	frequency('6',1.75647),
	frequency('7',1.621),
	frequency('8',1.66225),
	frequency('9',1.79558),
	frequency(':',0.00189169),
	frequency(';',0.00207476),
	frequency('<',0.000427156),
	frequency('=',0.00140351),
	frequency('>',0.000183067),
	frequency('?',0.00207476),
	frequency('@',0.0238597),
	frequency('A',0.130466),
	frequency('B',0.0806715),
	frequency('C',0.0660872),
	frequency('D',0.0698096),
	frequency('E',0.0970865),
	frequency('F',0.0417393),
	frequency('G',0.0497332),
	frequency('H',0.0544319),
	frequency('I',0.070908),
	frequency('J',0.0363083),
	frequency('K',0.0460719),
	frequency('L',0.0775594),
	frequency('M',0.0782306),
	frequency('N',0.0748134),
	frequency('O',0.0729217),
	frequency('P',0.073715),
	frequency('Q',0.0147064),
	frequency('R',0.08476),
	frequency('S',0.108132),
	frequency('T',0.0801223),
	frequency('U',0.0350268),
	frequency('V',0.0235546),
	frequency('W',0.0320367),
	frequency('X',0.0142182),
	frequency('Y',0.0255073),
	frequency('Z',0.0170252),
	frequency('[',0.0010984),
	frequency('\\',0.00115942),
	frequency(']',0.0010984),
	frequency('^',0.00195272),
	frequency('_',0.0122655),
	frequency('\'',0.00115942),
	frequency('a',7.52766),
	frequency('b',2.29145),
	frequency('c',2.57276),
	frequency('d',2.76401),
	frequency('e',7.0925),
	frequency('f',1.2476),
	frequency('g',1.85331),
	frequency('h',2.41319),
	frequency('i',4.69732),
	frequency('j',0.836677),
	frequency('k',1.96828),
	frequency('l',3.77728),
	frequency('m',2.99913),
	frequency('n',4.56899),
	frequency('o',5.17),
	frequency('p',2.45578),
	frequency('q',0.346119),
	frequency('r',4.96032),
	frequency('s',4.61079),
	frequency('t',3.87388),
	frequency('u',2.10191),
	frequency('v',0.833626),
	frequency('w',1.24492),
	frequency('x',0.573305),
	frequency('y',1.52483),
	frequency('z',0.632558),
	frequency('{',0.000122045),
	frequency('|',0.000122045),
	frequency('}',0.000122045),
	frequency('~',0.00152556)
	};

	for (auto x : vfrequency) {
		Node* newItem = new Leaf(string(1, x._c), x._f);
		frqTable.push(newItem);
	}
	//*/

	QTree qt;
	Node* root = qt.makeQTree(frqTable);

	result.push_back(getDecryption(root));
	cout << result[0];
	//freqfile.writeFile("decrypted.txt", result);
}

string Decrypt::getDecryption(Node* root) {
	int temp;
	string res = "";
	Node* pW = root;
	while (!bits.empty()) {
		if (dynamic_cast<Branch*>(pW) != nullptr) {
			temp = bits.front();
			//cout << temp;
			bits.pop();
			if (temp == 0) 
				pW = dynamic_cast<Branch*>(pW)->getLeft();
			else if (temp == 1) 
				pW = dynamic_cast<Branch*>(pW)->getRight();
			else
				cout << "error" << endl;
		}
		else
		{
			res += pW->getSymbol();
			pW = root;
		}
	}
	return res;
}


int main() {
	Decrypt afile("Compress.bin", "Frequencies.txt");
	return 0;
}