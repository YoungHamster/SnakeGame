#pragma once
#include <WinSock2.h>
#include "GameRoom.h"

struct connection
{
	SOCKET connectSock;
	unsigned long long uuid; // deprecated
	wchar_t* nickname;
	short nicknameLenght;
	char gameDir = LEFT;
	bool recvedLastTickInfo = false;
	bool connected = false;
	bool votedForStart = false;
	bool compressPhysics = true;
	short ownIdConnectionsArray;
	short roomIdInRoomsArray;
};