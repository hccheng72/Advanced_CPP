#pragma once
#include <vector>
using std::vector;
using std::string;


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

