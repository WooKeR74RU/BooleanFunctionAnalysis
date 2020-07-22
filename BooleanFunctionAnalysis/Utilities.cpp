#include "Utilities.h"

#include <algorithm>

vector<char> toBinary(size_t val)
{
	vector<char> res;
	if (val == 0)
	{
		res.push_back('0');
		return res;
	}
	while (val != 0)
	{
		res.push_back(val % 2 + '0');
		val /= 2;
	}
	reverse(res.begin(), res.end());
	return res;
}

template<typename T>
void remove(vector<T>& sortedVector, const T& value)
{
	auto it = lower_bound(sortedVector.begin(), sortedVector.end(), value);
	if (it == sortedVector.end() || *it != value)
		throw;
	sortedVector.erase(it);
}
template void remove(vector<wstring>&, const wstring&);

template<typename T>
bool exists(const vector<T>& sortedVector, const T& value)
{
	auto it = lower_bound(sortedVector.begin(), sortedVector.end(), value);
	return it != sortedVector.end() && *it == value;
}
template bool exists(const vector<wstring>&, const wstring&);

template<typename T>
size_t getPos(const vector<T>& sortedVector, const T& value)
{
	auto it = lower_bound(sortedVector.begin(), sortedVector.end(), value);
	if (it == sortedVector.end() || *it != value)
		throw;
	return it - sortedVector.begin();
}
template size_t getPos(const vector<wstring>&, const wstring&);

template<typename T>
vector<vector<T>> getTranspose(const vector<vector<T>>& matrix)
{
	size_t n = matrix.size();
	if (n == 0)
		return vector<vector<T>>();
	size_t m = matrix[0].size();
	if (m == 0)
		return vector<vector<T>>();
	vector<vector<T>> res(m, vector<T>(n));
	for (size_t i = 0; i < n; i++)
	{
		for (size_t j = 0; j < m; j++)
			res[j][i] = matrix[i][j];
	}
	return res;
}
template vector<vector<char>> getTranspose(const vector<vector<char>>&);
