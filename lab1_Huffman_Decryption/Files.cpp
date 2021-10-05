#include "Files.h"

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
