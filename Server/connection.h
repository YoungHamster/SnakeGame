#pragma once
#include <WinSock2.h>

struct connection
{
	SOCKET connectSock;
	unsigned long long uuid;
	wchar_t* nickname;
	char GameDir = 0;
	bool RecvedPhysics = false;
	bool connected = false;
	bool votedForStart = false;
};