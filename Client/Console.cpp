#include "Console.h"

void Console::Write(std::string str)
{
	if (!consoleAllocated)
	{
		AllocConsole();
		out = GetStdHandle(STD_OUTPUT_HANDLE);
		consoleAllocated = true;
	}
	WriteConsole(out, str.c_str(), str.length(), NULL, NULL);
}

void Console::Write(const char* buff)
{
	std::string str(buff);
	Write(str);
}

void Console::CloseConsole()
{
	FreeConsole();
	consoleAllocated = false;
}