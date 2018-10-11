#pragma once
#include <WinSock2.h>
#include "GameRoom.h"

struct connection
{
	SOCKET connectSock;
	unsigned long long uuid;
	wchar_t* nickname;
	char gameDir = 0;
	bool recvedPhysics = false;
	bool connected = false;
	bool votedForStart = false;
	bool compressPhysics = true;
	int gameDataProtocol = 0;
	GameRoom* gameRoom = NULL;
};