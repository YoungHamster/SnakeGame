#pragma once
#include <WinSock2.h>
#include "GameRoom.h"

struct connection
{
	SOCKET connectSock;
	unsigned long long uuid;
	std::wstring nickname;
	short nicknameLenght;
	char gameDir = LEFT;
	bool recvedLastTickInfo = false;
	bool connected = false;
	bool votedForStart = false;
	short ownIdConnectionsArray;
	short roomIdInRoomsArray = -1;
};