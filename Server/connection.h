#pragma once
#include <WinSock2.h>

struct connection
{
	int GameDir = 0;
	bool RecvedPhysics = false;
	wchar_t* nickname;
	unsigned long long uuid;
	bool connected = false;
	bool votedForStart = false;
	SOCKET connectSock;
};