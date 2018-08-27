#pragma once
#include <WinSock2.h>

struct connection
{
	wchar_t* nickname;
	unsigned long long uuid;
	bool connected = false;
	SOCKET connectSock;
};