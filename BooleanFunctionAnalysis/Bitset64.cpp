#include "Bitset64.h"

Bitset64::Bitset64(const uint64_t& data) : data(data)
{ }
void Bitset64::operator=(const uint64_t& data)
{
	this->data = data;
}
Bitset64::operator uint64_t&()
{
	return data;
}
size_t Bitset64::size()
{
	return 64;
}
bool Bitset64::at(size_t pos) const
{
	return data & (1ull << pos);
}
void Bitset64::set()
{
	data = UINT64_MAX;
}
void Bitset64::set(size_t pos)
{
	data |= (1ull << pos);
}
void Bitset64::reset()
{
	data = 0;
}
void Bitset64::reset(size_t pos)
{
	data &= ~(1ull << pos);
}
void Bitset64::flip()
{
	data = ~data;
}
void Bitset64::flip(size_t pos)
{
	data ^= (1ull << pos);
}