#include <iostream>
//#include <codecvt>
using namespace std;
#include "BooleanFunction.h"
#include "Analysis.h"
#include "KarnaughMap.h"
#include "MinDNF.h"
#include "Classes.h"

void analysis(const wstring& expr)
{
	wcout << "F = " << expr << endl << endl;
	BooleanFunction func(expr);
	if (func.isTautologic())
	{
		wcout << L"Функция F тождественно равна " << func.consider() << ".";
		return;
	}
	truthTable(func);
	wcout << endl;
	vecFunc(func);
	wcout << endl;
	vector<wstring> dummyVars = func.reduceDummies();
	if (!dummyVars.empty())
	{
		for (size_t i = 0; i < dummyVars.size(); i++)
		{
			wcout << L"Перменная ";
			wcout << dummyVars[i];
			wcout << L" - фиктивная." << endl;
		}
		if (func.isTautologic())
		{
			wcout << L"Функция F тождественно равна " << func.consider() << ".";
			return;
		}
		wcout << L"Дальнейшие вычисления происходят без учета фиктивных переменных." << endl << endl;
		truthTable(func);
		wcout << endl;
		vecFunc(func);
		wcout << endl;
	}
	else
	{
		wcout << L"Функция не имеет фиктивных переменных." << endl << endl;
	}
	minterms(func);
	maxterms(func);
	wcout << endl;
	fullDNF(func);
	fullCNF(func);
	wcout << endl;
	karnaughMap(func);
	wcout << endl;
	minDNF(func);
	wcout << endl;
	zhegalkinPolynomial(func);
	wcout << endl;
	classes(func);
}

int main()
{
	BooleanFunction::initialize();

	FILE* filePtr;
	freopen_s(&filePtr, "input.txt", "r", stdin);
	freopen_s(&filePtr, "output.txt", "w", stdout);
	setlocale(LC_ALL, "Russian");
	//wcin.imbue(locale(locale::empty(), new codecvt_utf8<wchar_t>));
	//wcout.imbue(locale(locale::empty(), new codecvt_utf8<wchar_t>));

	wstring expr;
	wcin >> expr;

	analysis(expr);

	return 0;
}