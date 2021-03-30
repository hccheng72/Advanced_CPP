#pragma once
#include <string>
#include <iostream>
#include <fstream>
#include <vector>
using namespace std;

class Files {
private:
	string filename;
	ifstream file;
public:
	Files(string s) { filename = s; }
	void readFile(vector<string>& slist);
	void writeFile(string sfile, vector<string> sdata);
};

