#pragma once
#include <Windows.h>
#include <string>

class Console
{
private:
	HANDLE out;
	HANDLE in;
	bool consoleAllocated = false;
public:
	void Write(std::string str);
	void Write(const char* buff);
	void CloseConsole();
};