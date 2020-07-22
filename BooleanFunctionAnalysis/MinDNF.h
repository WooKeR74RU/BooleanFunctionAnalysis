#pragma once

#include "BooleanFunction.h"

enum VALUE { ZERO, ONE, BOTH };
struct VarsSet
{
	vector<VALUE> data;
	size_t onesCount;
	VarsSet();
	VarsSet(size_t varsCount);
	VarsSet(const vector<bool>& vars);
	bool contains(const VarsSet& minterm) const;
	size_t size() const;
	bool operator==(const VarsSet& other) const;
	bool operator<(const VarsSet& other) const;
};

bool isUnite(const VarsSet& vars1, const VarsSet& vars2);
VarsSet unionSets(const VarsSet& vars1, const VarsSet& vars2);

void implicantsSortingThrough(size_t curTerm);

void minDNF(BooleanFunction& func);
