#include "Decrypt.h"

Decrypt::Decrypt(string nm, string dl) {
	filename = nm;
	delimiter = dl;
	Files file("Encrypted.txt");
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