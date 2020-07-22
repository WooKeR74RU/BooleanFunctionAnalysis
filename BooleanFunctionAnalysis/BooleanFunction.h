#pragma once
#include <string>
#include <vector>
#include <map>
#include <functional>
using namespace std;
#include "Bitset64.h"

class BooleanFunction
{
private:
	typedef function<bool(bool)> Unary;
	typedef function<bool(bool, bool)> Binary;
	static map<wstring, Unary> unaryFuncs;
	static map<wstring, Binary> binaryFuncs;
	struct Block
	{
		enum BlockType { VALUE, VARIABLE, UNARY, BINARY } type;
		union
		{
			bool value;
			wstring variable;
			Unary unary;
			Binary binary;
		};
		Block(const Block& other);
		Block(bool value);
		Block(const wstring& variable);
		Block(Unary unary);
		Block(Binary binary);
		~Block();
	};
	vector<Block> sequence;
	Bitset64 varValues;
	vector<wstring> varNames;
	vector<bool> vecFunc;
	bool getValue(const wstring& variable) const;
	static size_t getPrior(const wstring& oper);
	static wstring getNext(const wstring& expr, size_t& it);
public:
	static void initialize();
	BooleanFunction(const wstring& expr);
	vector<wstring> reduceDummies();
	bool isTautologic() const;
	size_t getVarsCount() const;
	const vector<wstring>& getVarNames() const;
	vector<bool> getVarValues() const;
	const vector<bool>& getVecFunc() const;
	size_t getSetsCount() const;
	void setVariablesSet(size_t index = 0);
	void nextVariablesSet();
	bool consider() const;
};