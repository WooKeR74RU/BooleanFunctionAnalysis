#include "KarnaughMap.h"

#include <iostream>
#include "Utilities.h"

void reverse34(vector<size_t>& arr, size_t l, size_t r)
{
	size_t len = r - l + 1;
	if (len == 2)
		return;
	size_t st3 = l + len / 4 * 2;
	size_t st4 = l + len / 4 * 3;
	size_t count = len / 4;
	for (size_t i = 0; i < count; i++)
		swap(arr[st3 + i], arr[st4 + i]);
	size_t mid = (l + r) / 2;
	reverse34(arr, l, mid);
	reverse34(arr, mid + 1, r);
}

void karnaughMap(BooleanFunction& func)
{
	if (func.getVarsCount() == 1)
		return;
	wcout << L"Карта Карно:" << endl;
	size_t n = func.getVarsCount() / 2;
	vector<size_t> valuesN(1ull << n);
	for (size_t i = 0; i < valuesN.size(); i++)
		valuesN[i] = i;
	reverse34(valuesN, 0, valuesN.size() - 1);
	size_t m = func.getVarsCount() / 2 + func.getVarsCount() % 2;
	vector<size_t> valuesM(1ull << m);
	for (size_t i = 0; i < valuesM.size(); i++)
		valuesM[i] = i;
	reverse34(valuesM, 0, valuesM.size() - 1);
	vector<vector<char>> titleM(valuesM.size());
	for (size_t i = 0; i < titleM.size(); i++)
	{
		titleM[i] = toBinary(valuesM[i]);
		titleM[i].insert(titleM[i].begin(), m - titleM[i].size(), '0');
		reverse(titleM[i].begin(), titleM[i].end());
	}
	titleM = getTranspose(titleM);
	for (size_t i = 0; i < titleM.size(); i++)
	{
		wcout << wstring(n + 2, ' ');
		for (size_t j = 0; j < titleM[i].size(); j++)
			wcout << titleM[i][j];
		wcout << endl;
	}
	wcout << wstring(n + 2, ' ') << wstring(valuesM.size(), '|') << endl;
	vector<vector<char>> titleN(valuesN.size());
	for (size_t i = 0; i < titleN.size(); i++)
	{
		titleN[i] = toBinary(valuesN[i]);
		titleN[i].insert(titleN[i].begin(), n - titleN[i].size(), '0');
	}
	for (size_t i = 0; i < valuesN.size(); i++)
	{
		for (size_t j = 0; j < titleN[i].size(); j++)
			wcout << titleN[i][j];
		wcout << "--";
		for (size_t j = 0; j < valuesM.size(); j++)
		{
			size_t varsSetIndex = valuesN[i] << m | valuesM[j];
			func.setVariablesSet(varsSetIndex);
			wcout << func.consider();
		}
		wcout << endl;
	}
	vector<wstring> varNames = func.getVarNames();
	wcout << L"Значения строк соответствуют (слева-направо) ";
	for (size_t i = 0; i < n; i++)
	{
		wcout << varNames[i];
		if (i + 1 != n)
			wcout << ", ";
	}
	wcout << "." << endl;
	wcout << L"Значения столбцов соответствуют (снизу-вверх) ";
	for (size_t i = 0; i < m; i++)
	{
		wcout << varNames[n + i];
		if (i + 1 != m)
			wcout << ", ";
	}
	wcout << "." << endl;
}
