#include "Classes.h"

#include <iostream>
using namespace std;

void classes(BooleanFunction& func)
{
	vector<bool> vecFunc = func.getVecFunc();
	size_t setsCount = func.getSetsCount();
	size_t varsCount = func.getVarsCount();

	bool isT0 = vecFunc.front() == false;
	bool isT1 = vecFunc.back() == true;

	bool isS = true;
	for (size_t i = 0; i < setsCount / 2; i++)
	{
		if (vecFunc[i] == vecFunc[setsCount - 1 - i])
		{
			isS = false;
			break;
		}
	}

	bool isM = true;
	func.setVariablesSet();
	for (size_t i = 0; i < setsCount; i++)
	{
		if (vecFunc[i])
		{
			vector<bool> vars = func.getVarValues();
			for (size_t j = 0; j < varsCount; j++)
			{
				if (!vars[j])
				{
					size_t newIndex = i | 1ull << (varsCount - 1 - j);
					if (!vecFunc[newIndex])
						isM = false;
				}
			}
		}
		if (!isM)
			break;
		func.nextVariablesSet();
	}

	bool isL = true;
	vector<bool> zhegPol = func.getVecFunc();
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
	for (size_t i = 0; i < setsCount; i++)
	{
		if (zhegPol[i])
		{
			vector<bool> vars = func.getVarValues();
			bool isFirstOne = true;
			for (size_t j = 0; j < varsCount; j++)
			{
				if (vars[j])
				{
					if (isFirstOne)
						isFirstOne = false;
					else
						isL = false;
				}
			}
		}
		if (!isL)
			break;
		func.nextVariablesSet();
	}

	wcout << L"Таблица предполных классов:" << endl;
	wcout << "T0 T1 S M L" << endl;
	wcout << (isT0 ? "+" : "-") << "  ";
	wcout << (isT1 ? "+" : "-") << "  ";
	wcout << (isS ? "+" : "-") << " ";
	wcout << (isM ? "+" : "-") << " ";
	wcout << (isL ? "+" : "-") << endl;
}
