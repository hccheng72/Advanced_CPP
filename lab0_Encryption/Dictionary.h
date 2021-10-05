#pragma once
#include<vector>
#include<string>
using namespace std;

template <class T>
class Dictionary {
private:
	vector<T> data;
	int dataSize;
public:
	Dictionary(int n) { 
		dataSize = n;
		//data = new vector<KeyValue>(n);
		data.resize(n);
	}
	int getDatasize() const { return dataSize; }
	T &operator[](const int &sub) {
		if (sub >= 0 || sub < dataSize) {
			return data[sub];
		}
	}
	void push_back(T &newItem) {
		data.push_back(newItem);
		dataSize++;
	}
};

