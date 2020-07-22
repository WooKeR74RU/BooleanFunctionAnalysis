#pragma once

#include <Windows.h>
#include <string>

class Console
{
private:
	HANDLE consoleIn;
	HANDLE consoleOut;
public:
	Console();
	void setCharacter(int x, int y, wchar_t c);
	void setString(int x, int y, const std::wstring& str);
	void setCursorPosition(int x, int y);
	std::wstring readString();
};