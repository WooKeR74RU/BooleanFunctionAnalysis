#pragma once

#include <cstdint>

class Bitset64
{
private:
	uint64_t data;
public:
	Bitset64(const uint64_t& data = 0);
	void operator=(const uint64_t& data);
	operator uint64_t&();
	static size_t size();
	bool at(size_t pos) const;
	void set();
	void set(size_t pos);
	void reset();
	void reset(size_t pos);
	void flip();
	void flip(size_t pos);
};