#include "StringParsing.h"

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
