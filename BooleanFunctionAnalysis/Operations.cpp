#include "Operations.h"

bool negationF(bool val)
{
	return !val;
}

bool conjunctionF(bool a, bool b)
{
	return a & b;
}
bool disjunctionF(bool a, bool b)
{
	return a | b;
}
bool implicationF(bool a, bool b)
{
	return !a | b;
}
bool exclusiveOrF(bool a, bool b)
{
	return a ^ b;
}
bool equalityF(bool a, bool b)
{
	return a == b;
}