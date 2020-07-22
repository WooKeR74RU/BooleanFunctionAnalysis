#include "BooleanFunction.h"

#include <cctype>
#include <set>
#include "Operations.h"
#include "Utilities.h"

map<wstring, BooleanFunction::Unary> BooleanFunction::unaryFuncs;
map<wstring, BooleanFunction::Binary> BooleanFunction::binaryFuncs;

BooleanFunction::Block::Block(const Block& other) : type(other.type)
{
	if (type == VALUE)
		value = other.value;
	if (type == VARIABLE)
	{
		new (&variable) wstring;
		variable = wstring(other.variable);
	}
	if (type == UNARY)
	{
		new (&unary) Unary;
		unary = other.unary;
	}
	if (type == BINARY)
	{
		new (&binary) Binary;
		binary = other.binary;
	}
}
BooleanFunction::Block::Block(bool value) : value(value)
{
	type = VALUE;
}
BooleanFunction::Block::Block(const wstring& variable) : variable(variable)
{
	type = VARIABLE;
}
BooleanFunction::Block::Block(Unary unary) : unary(unary)
{
	type = UNARY;
}
BooleanFunction::Block::Block(Binary binary) : binary(binary)
{
	type = BINARY;
}
BooleanFunction::Block::~Block()
{
	if (type == VARIABLE)
		variable.~wstring();
	if (type == UNARY)
		unary.~unary();
	if (type == BINARY)
		binary.~binary();
}
bool BooleanFunction::getValue(const wstring& variable) const
{
	if (!exists(varNames, variable))
		return false;
	size_t pos = getPos(varNames, variable);
	return varValues.at(varNames.size() - 1 - pos);
}
size_t BooleanFunction::getPrior(const wstring& oper)
{
	if (oper == L"!")
		return 0;
	if (oper == L"*")
		return 1;
	if (oper == L"+" || oper == L"^")
		return 2;
	if (oper == L"->")
		return 3;
	if (oper == L"=")
		return 4;
	throw;
}
wstring BooleanFunction::getNext(const wstring& expr, size_t& it)
{
	if (expr[it] == '(' || expr[it] == ')' || expr[it] == '0' || expr[it] == '1' ||
		expr[it] == '!' || expr[it] == '*' || expr[it] == '+' || expr[it] == '^' || expr[it] == '=')
	{
		return wstring(1, expr[it++]);
	}
	if (it + 1 < expr.size() && expr[it] == '-' && expr[it + 1] == '>')
	{
		it += 2;
		return L"->";
	}
	wstring variable;
	while (it < expr.size() && iswalnum(expr[it]))
		variable.push_back(expr[it++]);
	if (variable.empty())
		throw;
	return variable;
}
void BooleanFunction::initialize()
{
	unaryFuncs[L"!"] = negationF;
	binaryFuncs[L"*"] = conjunctionF;
	binaryFuncs[L"+"] = disjunctionF;
	binaryFuncs[L"->"] = implicationF;
	binaryFuncs[L"^"] = exclusiveOrF;
	binaryFuncs[L"="] = equalityF;
}
BooleanFunction::BooleanFunction(const wstring& expr)
{
	set<wstring> setVarNames;
	vector<wstring> opers;
	size_t it = 0;
	while (it < expr.size())
	{
		wstring block = getNext(expr, it);
		if (block == L"(")
		{
			opers.push_back(L"(");
			continue;
		}
		if (block == L")")
		{
			while (opers.back() != L"(")
			{
				sequence.push_back(Block(binaryFuncs.at(opers.back())));
				opers.pop_back();
			}
			opers.pop_back();
			while (!opers.empty() && opers.back() != L"(" && getPrior(opers.back()) == 0)
			{
				sequence.push_back(Block(unaryFuncs.at(opers.back())));
				opers.pop_back();
			}
			continue;
		}
		if (block == L"0" || block == L"1")
		{
			sequence.push_back(Block(block == L"1"));
			while (!opers.empty() && opers.back() != L"(" && getPrior(opers.back()) == 0)
			{
				sequence.push_back(Block(unaryFuncs.at(opers.back())));
				opers.pop_back();
			}
			continue;
		}
		if (!unaryFuncs.count(block) && !binaryFuncs.count(block))
		{
			sequence.push_back(Block(block));
			setVarNames.insert(block);
			while (!opers.empty() && opers.back() != L"(" && getPrior(opers.back()) == 0)
			{
				sequence.push_back(Block(unaryFuncs.at(opers.back())));
				opers.pop_back();
			}
			continue;
		}
		while (!opers.empty() && opers.back() != L"(" && getPrior(opers.back()) <= getPrior(block))
		{
			sequence.push_back(Block(binaryFuncs.at(opers.back())));
			opers.pop_back();
		}
		opers.push_back(block);
	}
	while (!opers.empty())
	{
		sequence.push_back(Block(binaryFuncs.at(opers.back())));
		opers.pop_back();
	}
	varNames = vector<wstring>(setVarNames.begin(), setVarNames.end());
	size_t setsCount = getSetsCount();
	vecFunc.resize(setsCount);
	setVariablesSet();
	for (size_t i = 0; i < setsCount; i++)
	{
		vecFunc[i] = consider();
		nextVariablesSet();
	}
}
vector<wstring> BooleanFunction::reduceDummies()
{
	vector<wstring> dummyVarNames;
	size_t allVarSetsCount = getSetsCount();
	vector<wstring> allVarNames = varNames;
	for (size_t i = 0; i < allVarNames.size(); i++)
	{
		size_t step = 1ull << (allVarNames.size() - 1 - i);
		bool isDummy = true;
		for (size_t j = 0; j < allVarSetsCount; j++)
		{
			if (vecFunc[j] != vecFunc[j + step])
			{
				isDummy = false;
				break;
			}
			if (j % step == step - 1)
				j += step;
		}
		if (isDummy)
		{
			remove(varNames, allVarNames[i]);
			dummyVarNames.push_back(allVarNames[i]);
		}
	}
	size_t setsCount = getSetsCount();
	vecFunc.resize(setsCount);
	setVariablesSet();
	for (size_t i = 0; i < setsCount; i++)
	{
		vecFunc[i] = consider();
		nextVariablesSet();
	}
	return dummyVarNames;
}
bool BooleanFunction::isTautologic() const
{
	return vecFunc.size() == 1;
}
size_t BooleanFunction::getVarsCount() const
{
	return varNames.size();
}
const vector<wstring>& BooleanFunction::getVarNames() const
{
	return varNames;
}
vector<bool> BooleanFunction::getVarValues() const
{
	vector<bool> res(varNames.size());
	for (size_t i = 0; i < varNames.size(); i++)
		res[i] = getValue(varNames[i]);
	return res;
}
const vector<bool>& BooleanFunction::getVecFunc() const
{
	return vecFunc;
}
size_t BooleanFunction::getSetsCount() const
{
	return 1ull << getVarsCount();
}
void BooleanFunction::setVariablesSet(size_t index)
{
	varValues = index;
}
void BooleanFunction::nextVariablesSet()
{
	varValues++;
}
bool BooleanFunction::consider() const
{
	vector<bool> stack;
	for (size_t i = 0; i < sequence.size(); i++)
	{
		if (sequence[i].type == Block::VALUE)
			stack.push_back(sequence[i].value);
		if (sequence[i].type == Block::VARIABLE)
			stack.push_back(getValue(sequence[i].variable));
		if (sequence[i].type == Block::UNARY)
		{
			bool val = stack.back();
			stack.pop_back();
			bool res = sequence[i].unary(val);
			stack.push_back(res);
		}
		if (sequence[i].type == Block::BINARY)
		{
			bool b = stack.back();
			stack.pop_back();
			bool a = stack.back();
			stack.pop_back();
			bool res = sequence[i].binary(a, b);
			stack.push_back(res);
		}
	}
	if (stack.size() != 1)
		throw;
	return stack[0];
}
