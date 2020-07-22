#pragma once

#include <vector>
#include <algorithm>
using namespace std;

vector<char> toBinary(size_t val);

template<typename T>
void remove(vector<T>& sortedVector, const T& value);

template<typename T>
bool exists(const vector<T>& sortedVector, const T& value);

template<typename T>
size_t getPos(const vector<T>& sortedVector, const T& value);

template<typename T>
vector<vector<T>> getTranspose(const vector<vector<T>>& matrix);

template<typename T>
void removeDuplicates(vector<T>& sortedVector)
{
	sortedVector.resize(unique(sortedVector.begin(), sortedVector.end()) - sortedVector.begin());
}