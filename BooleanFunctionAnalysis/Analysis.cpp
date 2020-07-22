#include "Analysis.h"

#include <iostream>
using namespace std;

void truthTable(BooleanFunction& func)
{
	wcout << L"Таблица истинности:" << endl;
	vector<wstring> varNames = func.getVarNames();
	for (size_t i = 0; i < varNames.size(); i++)
		wcout << varNames[i] << ' ';
	wcout << 'F' << endl;
	vector<bool> vecFunc = func.getVecFunc();
	size_t setsCount = func.getSetsCount();
	func.setVariablesSet();
	for (size_t i = 0; i < setsCount; i++)
	{
		vector<bool> vars = func.getVarValues();
		for (size_t j = 0; j < vars.size(); j++)
			wcout << vars[j] << wstring(varNames[j].size(), ' ');
		wcout << vecFunc[i] << endl;
		func.nextVariablesSet();
	}
}

void vecFunc(BooleanFunction& func)
{
	wcout << L"Вектор функции:" << endl << "F = { ";
	vector<bool> vecFunc = func.getVecFunc();
	size_t setsCount = func.getSetsCount();
	for (size_t i = 0; i < setsCount; i++)
	{
		wcout << vecFunc[i];
		if (i + 1 != setsCount)
			wcout << ", ";
	}
	wcout << " }" << endl;
}

void minterms(BooleanFunction& func)
{
	wcout << L"Вектор минтермов:" << endl << "F = { ";
	vector<bool> vecFunc = func.getVecFunc();
	size_t setsCount = func.getSetsCount();
	bool isFirst = true;
	for (size_t i = 0; i < setsCount; i++)
	{
		if (vecFunc[i])
		{
			if (!isFirst)
				wcout << ", ";
			wcout << "Min" << i;
			isFirst = false;
		}
	}
	wcout << " }" << endl;
}

void maxterms(BooleanFunction& func)
{
	wcout << L"Вектор макстермов:" << endl << "F = { ";
	vector<bool> vecFunc = func.getVecFunc();
	size_t setsCount = func.getSetsCount();
	bool isFirst = true;
	for (size_t i = 0; i < setsCount; i++)
	{
		if (!vecFunc[setsCount - 1 - i])
		{
			if (!isFirst)
				wcout << ", ";
			wcout << "Max" << i;
			isFirst = false;
		}
	}
	wcout << " }" << endl;
}

void fullDNF(BooleanFunction& func)
{
	wcout << L"СДНФ:" << endl << L"F = ";
	vector<wstring> varNames = func.getVarNames();
	vector<bool> vecFunc = func.getVecFunc();
	size_t setsCount = func.getSetsCount();
	bool isFirst = true;
	func.setVariablesSet();
	for (size_t i = 0; i < setsCount; i++)
	{
		if (vecFunc[i])
		{
			if (!isFirst)
				wcout << '+';
			vector<bool> varValues = func.getVarValues();
			for (size_t j = 0; j < varNames.size(); j++)
			{
				if (!varValues[j])
					wcout << '!';
				wcout << varNames[j];
				if (j + 1 != varNames.size())
					wcout << '*';
			}
			isFirst = false;
		}
		func.nextVariablesSet();
	}
	wcout << endl;
}

void fullCNF(BooleanFunction& func)
{
	wcout << L"СКНФ:" << endl << L"F = ";
	vector<wstring> varNames = func.getVarNames();
	vector<bool> vecFunc = func.getVecFunc();
	size_t setsCount = func.getSetsCount();
	bool isFirst = true;
	func.setVariablesSet();
	for (size_t i = 0; i < setsCount; i++)
	{
		if (!vecFunc[setsCount - 1 - i])
		{
			if (!isFirst)
				wcout << '*';
			wcout << '(';
			vector<bool> varValues = func.getVarValues();
			for (size_t j = 0; j < varNames.size(); j++)
			{
				if (!varValues[j])
					wcout << '!';
				wcout << varNames[j];
				if (j + 1 != varNames.size())
					wcout << '+';
			}
			wcout << ')';
			isFirst = false;
		}
		func.nextVariablesSet();
	}
	wcout << endl;
}

void zhegalkinPolynomial(BooleanFunction& func)
{
	wcout << L"Полином Жегалкина:" << endl << L"F = ";
	vector<bool> zhegPol = func.getVecFunc();
	size_t varsCount = func.getVarsCount();
	for (size_t k = 0; k < varsCount; k++)
	{
		vector<bool> newPol(zhegPol.size());
		size_t blockLen = 1ull << k;
		for (size_t i = 0; i < zhegPol.size(); i++)
		{
			if (i % (2 * blockLen) < blockLen)
				newPol[i] = zhegPol[i];
			else
				newPol[i] = zhegPol[i] ^ zhegPol[i - blockLen];
		}
		zhegPol = newPol;
	}
	func.setVariablesSet();
	size_t setsCount = func.getSetsCount();
	vector<wstring> varNames = func.getVarNames();
	bool isFirstItem = true;
	for (size_t i = 0; i < setsCount; i++)
	{
		if (zhegPol[i])
		{
			if (!isFirstItem)
				wcout << '^';
			isFirstItem = false;
			vector<bool> varValues = func.getVarValues();
			bool isFirstVar = true;
			for (size_t j = 0; j < varNames.size(); j++)
			{
				if (varValues[j])
				{
					if (!isFirstVar)
						wcout << '*';
					isFirstVar = false;
					wcout << varNames[j];
				}
			}
			if (isFirstVar)
				wcout << '1';
		}
		func.nextVariablesSet();
	}
	wcout << endl;
}
