#include "Console.h"

Console::Console()
{
	consoleIn = GetStdHandle(STD_INPUT_HANDLE);
	consoleOut = GetStdHandle(STD_OUTPUT_HANDLE);
}

void Console::setCharacter(int x, int y, wchar_t c)
{
	COORD coord;
	coord.X = x;
	coord.Y = y;
	wchar_t tmp[] = { c };
	DWORD count;
	WriteConsoleOutputCharacterW(consoleOut, tmp, 1, coord, &count);
	if (count != 1)
		throw;
}

void Console::setString(int x, int y, const std::wstring& str)
{
	COORD coord;
	coord.X = x;
	coord.Y = y;
	DWORD count;
	WriteConsoleOutputCharacterW(consoleOut, str.c_str(), str.size(), coord, &count);
	if (count != str.size())
		throw;
}

void Console::setCursorPosition(int x, int y)
{
	COORD coord;
	coord.X = x;
	coord.Y = y;
	SetConsoleCursorPosition(consoleOut, coord);
}

std::wstring Console::readString()
{
	std::wstring res;
	while (true)
	{
		const size_t BUFFER_SIZE = 32;
		wchar_t buffer[BUFFER_SIZE];
		DWORD readCount;
		ReadConsoleW(consoleIn, buffer, BUFFER_SIZE, &readCount, NULL);
		res += std::wstring(buffer, readCount);
		if (buffer[readCount - 1] == '\n')
			break;
	}
	return res;
}