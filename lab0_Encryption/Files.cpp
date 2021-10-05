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

void Files::writeFile(string sfile, vector<string> sdata) // or slist[]
{
	ofstream outfile(sfile);
	for (auto s : sdata)
		outfile << s << endl;
	cout << "\n" << sfile << " file has been created." << endl;
	outfile.close();
}
