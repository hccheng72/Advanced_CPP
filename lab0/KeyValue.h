#pragma once
//#include <utility>
//#include <string>
//using namespace std;

template <class T1, class T2>
struct KeyValue {
	T1 key;
	T2 value;
	KeyValue() {}
	KeyValue(T1 i, T2 s) { key = i; value = s; }
};
