#include <string>
#include <iostream>
#include "Decrypt.h"
using namespace std;

int main() {
	string filename, delimiter;
	cout << "which file should be decrypted? ";
	getline(cin, filename);
	cout << "what is the delimiter? ";
	cin.ignore();
	getline(cin, delimiter);
	Decrypt aDecrypt(filename, delimiter);
	return 0;
}
