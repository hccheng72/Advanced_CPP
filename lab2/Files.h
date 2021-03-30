#pragma once
#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <regex>
using namespace std;

class Files {
private:
	string filename;
	//ifstream file;
public:
	Files(string s) { filename = s; }
	void readFile(vector<string>& bs, vector<double>& pc);
	void readFile(vector<pair<string, vector<string>>>& cart);
	void writeFile(string sfile, vector<string> sdata);
};

