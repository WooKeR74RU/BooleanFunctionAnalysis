#include "MinDNF.h"

#include <iostream>
#include <algorithm>
#include <set>
#include "Utilities.h"

VarsSet::VarsSet() = default;
VarsSet::VarsSet(size_t varsCount) : data(varsCount), onesCount(0)
{ }
VarsSet::VarsSet(const vector<bool>& vars) : onesCount(0)
{
	this->data.resize(vars.size());
	for (size_t j = 0; j < vars.size(); j++)
	{
		if (vars[j])
		{
			this->data[j] = ONE;
			onesCount++;
		}
		else
		{
			this->data[j] = ZERO;
		}
	}
}
bool VarsSet::contains(const VarsSet& minterm) const
{
	for (size_t i = 0; i < data.size(); i++)
	{
		if (data[i] != BOTH && data[i] != minterm.data[i])
			return false;
	}
	return true;
}
size_t VarsSet::size() const
{
	size_t varsCount = 0;
	for (size_t i = 0; i < data.size(); i++)
	{
		if (data[i] != BOTH)
			varsCount++;
	}
	return varsCount;
}
bool VarsSet::operator==(const VarsSet& other) const
{
	return onesCount == other.onesCount && data == other.data;
}
bool VarsSet::operator<(const VarsSet& other) const
{
	if (onesCount != other.onesCount)
		return onesCount < other.onesCount;
	return data < other.data;
}

bool isUnite(const VarsSet& vars1, const VarsSet& vars2)
{
	bool isFirstDiff = true;
	for (size_t i = 0; i < vars1.data.size(); i++)
	{
		if (vars1.data[i] != vars2.data[i])
		{
			if (!isFirstDiff)
				return false;
			isFirstDiff = false;
		}
	}
	return true;
}
VarsSet unionSets(const VarsSet& vars1, const VarsSet& vars2)
{
	VarsSet res(vars1.data.size());
	for (size_t i = 0; i < vars1.data.size(); i++)
	{
		if (vars1.data[i] == vars2.data[i])
			res.data[i] = vars1.data[i];
		else
			res.data[i] = BOTH;
	}
	res.onesCount = min(vars1.onesCount, vars2.onesCount);
	return res;
}

size_t primeImplicantsCount;
vector<vector<size_t>> implicantsByTerm;
size_t termsCount;
vector<vector<size_t>> termsByImplicant;

vector<vector<size_t>> conImplicantSets;
set<size_t> curImplicants;
set<size_t> curTerms;
void implicantsSortingThrough(size_t curTerm)
{
	if (curTerms.size() == termsCount)
	{
		conImplicantSets.push_back(vector<size_t>(curImplicants.begin(), curImplicants.end()));
		return;
	}
	if (curTerm == termsCount)
		return;
	if (curTerms.count(curTerm))
	{
		implicantsSortingThrough(curTerm + 1);
	}
	else
	{
		for (size_t i = 0; i < implicantsByTerm[curTerm].size(); i++)
		{
			size_t implicant = implicantsByTerm[curTerm][i];
			vector<size_t> newTerms;
			for (size_t j = 0; j < termsByImplicant[implicant].size(); j++)
			{
				size_t term = termsByImplicant[implicant][j];
				if (!curTerms.count(term))
					newTerms.push_back(term);
			}
			for (size_t i = 0; i < newTerms.size(); i++)
				curTerms.insert(newTerms[i]);
			curImplicants.insert(implicant);
			implicantsSortingThrough(curTerm + 1);
			curImplicants.erase(implicant);
			for (size_t i = 0; i < newTerms.size(); i++)
				curTerms.erase(newTerms[i]);
		}
	}
}

void minDNF(BooleanFunction& func)
{
	conImplicantSets.clear();
	vector<VarsSet> primeImplicants;
	vector<bool> vecFunc = func.getVecFunc();
	size_t setsCount = func.getSetsCount();
	vector<VarsSet> minterms;
	func.setVariablesSet();
	for (size_t i = 0; i < setsCount; i++)
	{
		if (vecFunc[i])
		{
			vector<bool> vars = func.getVarValues();
			minterms.push_back(VarsSet(vars));
		}
		func.nextVariablesSet();
	}
	vector<VarsSet> curStep = minterms;
	size_t maxOnesCount = func.getVarsCount();
	while (!curStep.empty())
	{
		vector<VarsSet> secondStep;
		sort(curStep.begin(), curStep.end());
		removeDuplicates(curStep);
		vector<size_t> onesBegins(maxOnesCount + 2);
		size_t lastCount = 0;
		for (size_t i = 0; i < curStep.size(); i++)
		{
			if (lastCount != curStep[i].onesCount)
			{
				lastCount = curStep[i].onesCount;
				onesBegins[lastCount] = i;
			}
		}
		vector<bool> used(curStep.size());
		for (size_t i = 0; i < curStep.size(); i++)
		{
			size_t j = onesBegins[curStep[i].onesCount + 1];
			while (j < curStep.size() && curStep[i].onesCount + 1 == curStep[j].onesCount)
			{
				if (isUnite(curStep[i], curStep[j]))
				{
					used[i] = used[j] = true;
					secondStep.push_back(unionSets(curStep[i], curStep[j]));
				}
				j++;
			}
		}
		for (size_t i = 0; i < curStep.size(); i++)
		{
			if (!used[i])
				primeImplicants.push_back(curStep[i]);
		}
		curStep = secondStep;
	}
	primeImplicantsCount = primeImplicants.size();
	vector<wstring> varNames = func.getVarNames();
	size_t varsCount = func.getVarsCount();
	wcout << L"Сокращенная ДНФ:" << endl;
	for (size_t i = 0; i < primeImplicantsCount; i++)
	{
		bool isFirst = true;
		for (size_t j = 0; j < varsCount; j++)
		{
			if (primeImplicants[i].data[j] != BOTH)
			{
				if (!isFirst)
					wcout << '*';
				isFirst = false;
				if (primeImplicants[i].data[j] == ZERO)
					wcout << '!';
				wcout << varNames[j];
			}
		}
		if (i + 1 != primeImplicantsCount)
			wcout << '+';
	}
	wcout << endl << endl;
	termsByImplicant.resize(primeImplicantsCount);
	termsCount = minterms.size();
	implicantsByTerm.resize(termsCount);
	for (size_t i = 0; i < primeImplicantsCount; i++)
	{
		for (size_t j = 0; j < termsCount; j++)
		{
			if (primeImplicants[i].contains(minterms[j]))
			{
				termsByImplicant[i].push_back(j);
				implicantsByTerm[j].push_back(i);
			}
		}
	}
	implicantsSortingThrough(0);
	removeDuplicates(conImplicantSets);
	vector<pair<size_t, vector<VarsSet>>> minImplicantSets(conImplicantSets.size());
	for (size_t i = 0; i < conImplicantSets.size(); i++)
	{
		for (size_t j = 0; j < conImplicantSets[i].size(); j++)
		{
			size_t implicant = conImplicantSets[i][j];
			minImplicantSets[i].first += primeImplicants[implicant].size();
			minImplicantSets[i].second.push_back(primeImplicants[implicant]);
		}
	}
	sort(minImplicantSets.begin(), minImplicantSets.end());
	wcout << L"Минимальные ДНФ:" << endl;
	size_t minVarsCount = minImplicantSets[0].first;
	for (size_t k = 0; k < minImplicantSets.size(); k++)
	{
		if (minVarsCount != minImplicantSets[k].first)
			break;
		for (size_t i = 0; i < minImplicantSets[k].second.size(); i++)
		{
			VarsSet& vars = minImplicantSets[k].second[i];
			bool isFirst = true;
			for (size_t j = 0; j < varsCount; j++)
			{
				if (vars.data[j] != BOTH)
				{
					if (!isFirst)
						wcout << '*';
					isFirst = false;
					if (vars.data[j] == ZERO)
						wcout << '!';
					wcout << varNames[j];
				}
			}
			if (i + 1 != minImplicantSets[k].second.size())
				wcout << '+';
		}
		wcout << endl;
	}
}
