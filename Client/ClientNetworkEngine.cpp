#include "ClientNetworkEngine.h"

bool ClientNetworkEngine::ConnectToServer(std::wstring *nickname, const char* ip, unsigned short port)
{
#ifdef NETWORK_PROFILING
	out.Write(ip);
#endif
	game.Init(64, 36, 5, 5, 5, 5);
	int succ;
	WSAData wData;
	succ = WSAStartup(MAKEWORD(2, 2), &wData);
	SOCKADDR_IN address;
	sock = socket(AF_INET, SOCK_STREAM, NULL);
	address.sin_addr.s_addr = inet_addr(ip);
	address.sin_family = AF_INET;
	address.sin_port = htons(port);
	connect(sock, (SOCKADDR*)&address, sizeof(address));

	if (sock == INVALID_SOCKET)
	{
#ifdef NETWORK_PROFILING
		out.Write("connect() error\n");
#endif
		return NULL;
	}

	/* Maximum size of packet in this function, 64 wchars + 1 header byte */
	char buff[129];
	int firstBuffLenght = 1;
	if (nickname->length() >= 64)
	{
		return false;
	}
	buff[0] = (char)nickname->length();
	BytesToWchar nicknameConv;
	for (int i = 0; i < nickname->length(); i++, firstBuffLenght += 2)
	{
		nicknameConv.wchar = nickname->at(i);
		buff[1 + i * sizeof(wchar_t)] = nicknameConv.bytes[0];
		buff[2 + i * sizeof(wchar_t)] = nicknameConv.bytes[1];
	}
	int sendRes = send(sock, buff, firstBuffLenght, NULL);
#ifdef NETWORK_PROFILING
	out.Write("Sended 1st packet to server! " + std::to_string(sendRes) + " bytes." + "\n");
#endif

	BytesToULL uuidconv;
	if (recv(sock, buff, sizeof(buff), NULL) != 9 || buff[0] != '\x01')
	{
#ifdef NETWORK_PROFILING
		out.Write("Error while handshaking with server!\n");
#endif
		return false;
	}
#ifdef NETWORK_PROFILING
	out.Write("Received 1st packet from server!\n");
#endif
	for (int i = 0; i < 8; i++)
	{
		uuidconv.bytes[i] = buff[i + 1];
	}
	UUID = uuidconv.integer;
#ifdef NETWORK_PROFILING
	out.Write(std::to_string(UUID) + " - UUID\n");
#endif
	return true;
}

ServerInfo* ClientNetworkEngine::GetInfoAboutServer()
{
	currentServer.activeRooms.clear();
	currentServer.activeRoomsIDs.clear();
	currentServer.serverName.clear();
	/* Get active server rooms info */
	char buff[4001];
	buff[0] = 0x01;
	send(sock, buff, 1, NULL);
	int numberOfBytes = recv(sock, buff, sizeof(buff), NULL);
	int numberOfHandledBytes = 2;
#ifdef NETWORK_PROFILING
	out.Write("Got active rooms information!\n");
#endif
	BytesToInt roomsIdsConv;
	BytesToShort stringsSizesConv;
	BytesToFloat gameSpeedConv;
	BytesToWchar stringsConv;
	short stringsSizes[5];
	if (numberOfBytes > 2)
	{
		while (numberOfHandledBytes < numberOfBytes)
		{
			currentServer.activeRooms.resize(currentServer.activeRooms.size() + 1);
			for (int i = 0; i < sizeof(int); i++, numberOfHandledBytes++)
			{
				roomsIdsConv.bytes[i] = buff[numberOfHandledBytes];
			}
			currentServer.activeRooms[currentServer.activeRooms.size() - 1].ID = roomsIdsConv.integer;
			for (int i = 0; i < sizeof(float); i++, numberOfHandledBytes++)
			{
				gameSpeedConv.bytes[i] = buff[numberOfBytes];
			}
			currentServer.activeRooms[currentServer.activeRooms.size() - 1].gameSpeed = gameSpeedConv.number;
			for (int i = 0; i < 5; i++, numberOfHandledBytes += sizeof(short))
			{
				stringsSizesConv.bytes[0] = buff[numberOfHandledBytes];
				stringsSizesConv.bytes[1] = buff[numberOfHandledBytes + 1];
				stringsSizes[i] = stringsSizesConv.integer;
			}
			currentServer.activeRooms[currentServer.activeRooms.size() - 1].roomName.resize(stringsSizes[0]);
			currentServer.activeRooms[currentServer.activeRooms.size() - 1].nicknamesOfPlayers[0].resize(stringsSizes[1]);
			currentServer.activeRooms[currentServer.activeRooms.size() - 1].nicknamesOfPlayers[1].resize(stringsSizes[2]);
			currentServer.activeRooms[currentServer.activeRooms.size() - 1].nicknamesOfPlayers[2].resize(stringsSizes[3]);
			currentServer.activeRooms[currentServer.activeRooms.size() - 1].nicknamesOfPlayers[3].resize(stringsSizes[4]);
			for (int i = 0; i < stringsSizes[0]; i++, numberOfHandledBytes += sizeof(wchar_t))
			{
				stringsConv.bytes[0] = buff[numberOfHandledBytes];
				stringsConv.bytes[1] = buff[numberOfHandledBytes + 1];
				currentServer.activeRooms[currentServer.activeRooms.size() - 1].roomName[i] = stringsConv.wchar;
			}
			for (int i = 0; i < 4; i++)
			{
				for (int j = 0; j < stringsSizes[1 + i]; j++, numberOfHandledBytes += sizeof(wchar_t))
				{
					stringsConv.bytes[0] = buff[numberOfHandledBytes];
					stringsConv.bytes[1] = buff[numberOfHandledBytes + 1];
					currentServer.activeRooms[currentServer.activeRooms.size() - 1].nicknamesOfPlayers[i][j] = stringsConv.wchar;
				}
			}
		}
	}
	/* Test ping */
	buff[0] = 0x06;
	send(sock, buff, 1, NULL);
	for (int i = 0; i < 512; i++)
	{
		buff[i] = (char)randomNumber(60, 127);
	}
	auto time = std::chrono::high_resolution_clock::now();
	send(sock, buff, 512, NULL);
	recv(sock, buff, 512, NULL);
	auto secondTime = std::chrono::high_resolution_clock::now();
	long long delta = std::chrono::duration_cast<std::chrono::nanoseconds>(secondTime - time).count();
	currentServer.pingInMs = delta / 1000000;
#ifdef NETWORK_PROFILING
	out.Write("Ping = " + std::to_string(delta) + "nanoseconds\n");
#endif

	return &currentServer;
}

bool ClientNetworkEngine::CreateNewRoom()
{
	char buff[3];
	buff[0] = 0x02;
	send(sock, buff, 1, NULL);
	recv(sock, buff, sizeof(buff), NULL);
	if (buff[0] != 0x02)
	{
#ifdef NETWORK_PROFILING
		out.Write("Error while creating room! Server sent wrong packet!\n");
#endif
		return false;
	}
	BytesToShort roomIDconv;
	roomIDconv.bytes[0] = buff[1];
	roomIDconv.bytes[1] = buff[2];
	currentRoomID = roomIDconv.integer;
#ifdef NETWORK_PROFILING
	out.Write("Created new room!\n");
#endif
	return true;
}

bool ClientNetworkEngine::JoinExistingRoom(short roomID)
{
	char buff[3];
	buff[0] = 0x03;
	BytesToShort roomIDconv;
	roomIDconv.integer = roomID;
	buff[1] = roomIDconv.bytes[0];
	buff[2] = roomIDconv.bytes[1];
	send(sock, buff, 3, NULL);
	recv(sock, buff, 3, NULL);
	if (buff[0] != 0x03)
	{
#ifdef NETWORK_PROFILING
		out.Write("Error while trying to join existing room with id: " + std::to_string(roomID) + '\n');
#endif
		return false;
	}
	currentRoomID = roomID;
#ifdef NETWORK_PROFILING
	out.Write("Connected to room with id: " + std::to_string(roomID) + "\n");
#endif
	return true;
}

ServerInfo* ClientNetworkEngine::LeaveRoom()
{
	char buff[1];
	buff[0] = 0x04;
	send(sock, buff, 1, NULL);
	recv(sock, buff, 1, NULL);
	if (buff[0] != 0x04)
	{
#ifdef NETWORK_PROFILING
		out.Write("Error while trying to leave room, server sent wrong respone!\n");
#endif
		return false;
	}
#ifdef NETWORK_PROFILING
	out.Write("Left room.\n");
#endif
	return GetInfoAboutServer();
}

bool ClientNetworkEngine::VoteForStart()
{
	char buff[2];
	buff[0] = 0x05;
	send(sock, buff, 1, NULL);
	recv(sock, buff, 2, NULL);
	if (buff[0] != 0x05)
	{
#ifdef NETWORK_PROFILING
		out.Write("Error while trying to vote for match start, server sent wrong respone!\n");
#endif
		return false;
	}
	if (buff[1] != true)
	{
#ifdef NETWORK_PROFILING
		out.Write("Can't start match, all players should vote for start!\n");
#endif
		return false;
	}
#ifdef NETWORK_PROFILING
	out.Write("Started match!\n");
#endif
	return true;
}

int ClientNetworkEngine::Ping()
{
	char buff[512];
	for (int i = 0; i < 512; i++)
	{
		buff[i] = randomNumber(0, 255);
	}
	buff[0] = 0x06;
	if (send(sock, buff, 1, NULL) != 1)
	{
#ifdef NETWORK_PROFILING
		out.Write("Can't ping server!\n");
#endif
		return -1;
	}
	auto time = std::chrono::high_resolution_clock::now();
	send(sock, buff, 512, NULL);
	recv(sock, buff, 512, NULL);
	auto secondTime = std::chrono::high_resolution_clock::now();
	long long ping = std::chrono::duration_cast<std::chrono::nanoseconds>(secondTime - time).count();
	double kbpersec = 1000 * 1000 * 1000 / ping;
#ifdef NETWORK_PROFILING
	out.Write("Ping = " + std::to_string(ping) + "nanosecond, connection speed = " + std::to_string(kbpersec) + "kb/sec\n");
#endif
	return (int)ping / 1000000;
}

int ClientNetworkEngine::GameTick(char dir)
{
	char buff[2 + 4 * sizeof(ShortGameTickInfo)];
	buff[0] = 0x0E;
	buff[1] = dir;
	BytesToShortGameTickInfo ticksShortInfoConv;
	for (int i = 0; i < 4; i++)
	{
		ticksShortInfoConv.info = *gameController.GetShortTickInfoPointerByTickID(gameController.GetCurrentTickID() - i);
		for (int j = 0; j < sizeof(ShortGameTickInfo); j++)
		{
			buff[2 + j] = ticksShortInfoConv.bytes[j];
		}
	}
	send(sock, buff, sizeof(buff), NULL);
	recv(sock, buff, sizeof(buff), NULL);
	switch (buff[1])
	{
	case 0x00:
		gameController.ChangePlayersDirections(&buff[3]);
		gameController.GameTick();
		break;
	case 0x01:
		int lastEqualTickID;
		BytesToInt lastEqualTickIDConv;
		for (int i = 0; i < sizeof(int); i++)
		{
			lastEqualTickIDConv.bytes[i] = buff[2 + i];
		}
		lastEqualTickID = lastEqualTickIDConv.integer;
		ShortGameTickInfo *ticksInfo = new ShortGameTickInfo[gameController.GetCurrentTickID() - lastEqualTickID];
		for (int i = 0; i < gameController.GetCurrentTickID() - lastEqualTickID; i++)
		{
			for (int j = 0; j < sizeof(ShortGameTickInfo); j++)
			{
				ticksShortInfoConv.bytes[j] = buff[j + 2 + sizeof(int) + i * sizeof(ShortGameTickInfo)];
			}
			ticksInfo[i] = ticksShortInfoConv.info;
		}
		gameController.ResimulateGameFromPastTick(lastEqualTickID, ticksInfo);
		break;
	case 0x02:
		SynchronizeGame();
		break;
	}
	if (buff[1] == 0x01)
	{
		game.OneTick(buff[2], buff[3], buff[4], buff[5]);
		BytesToShort someStuffConverter[6];
		for (int i = 0; i < 6; i++)
		{
			someStuffConverter[i].bytes[0] = buff[6 + i * sizeof(short)];
			someStuffConverter[i].bytes[1] = buff[7 + i * sizeof(short)];
		}
		game.ChangeApplePostition(someStuffConverter[0].integer, someStuffConverter[1].integer);
		if (game.snakes[0].size() != someStuffConverter[2].integer || game.snakes[1].size() != someStuffConverter[3].integer
			|| game.snakes[2].size() != someStuffConverter[4].integer || game.snakes[3].size() != someStuffConverter[5].integer)
		{
			if (!SynchronizeGame())
			{
#ifdef NETWORK_PROFILING
				out.Write("Crytical error! can't synchronyze game!\n");
#endif
			}
		}
	}	
	return 0;
}

bool ClientNetworkEngine::SynchronizeGame()
{
#ifdef NETWORK_PROFILING

#endif
	char enterbuff[1];
	enterbuff[0] = 0x08;
	send(sock, enterbuff, 1, NULL);

	char buffsizebytes[2];
	recv(sock, buffsizebytes, 2, NULL);

	BytesToShort buffsizeConverter;
	buffsizeConverter.bytes[0] = buffsizebytes[0];
	buffsizeConverter.bytes[1] = buffsizebytes[1];
	short buffsize = buffsizeConverter.integer;
#ifdef NETWORK_PROFILING
	out.Write("Size of synchronyzing packet = " + std::to_string(buffsize) + "\n");
#endif
	char *buff = new char[buffsize];
	recv(sock, buff, buffsize, NULL);

	/* 1st short - physics size, 4 others - snakes lenghts*/
	BytesToShort lenghtsConverter[5];
	for (int i = 0; i < 5; i++)
	{
		lenghtsConverter[i].bytes[0] = buff[1 + i * sizeof(short)];
		lenghtsConverter[i].bytes[1] = buff[2 + i * sizeof(short)];
	}
	if (buff[0] != 0x08)
	{
		return false;
	}
	game.physics.clear();
	game.snakes[0].clear();
	game.snakes[1].clear();
	game.snakes[2].clear();
	game.snakes[3].clear();
	game.physics.resize(lenghtsConverter[0].integer);
	for (int i = 0; i < 4; i++)
	{
		game.snakes[i].resize(lenghtsConverter[i + 1].integer);
	}
	BytesToPhysicalObject physicsConv;
	for (int i = 0; i < lenghtsConverter[0].integer; i++)
	{
		for (int j = 0; j < sizeof(PhysicalObject); j++)
		{
			physicsConv.bytes[j] = buff[j + 11 + sizeof(PhysicalObject) * i];
		}
		game.physics[i] = physicsConv.obj;
	}

	BytesToSnakeBlock snakesConv;
	for (int i = 0; i < lenghtsConverter[1].integer; i++)
	{
		for (int j = 0; j < sizeof(SnakeBlock); j++)
		{
			snakesConv.bytes[j] = buff[j + 11 + lenghtsConverter[0].integer * sizeof(PhysicalObject) + i * sizeof(SnakeBlock)];
		}
		game.snakes[0][i] = snakesConv.block;
	}
	for (int i = 0; i < lenghtsConverter[2].integer; i++)
	{
		for (int j = 0; j < sizeof(SnakeBlock); j++)
		{
			snakesConv.bytes[j] = buff[j + 11 + lenghtsConverter[0].integer * sizeof(PhysicalObject) + lenghtsConverter[1].integer * sizeof(SnakeBlock) + i * sizeof(SnakeBlock)];
		}
		game.snakes[1][i] = snakesConv.block;
	}
	for (int i = 0; i < lenghtsConverter[3].integer; i++)
	{
		for (int j = 0; j < sizeof(SnakeBlock); j++)
		{
			snakesConv.bytes[j] = buff[j + 11 + lenghtsConverter[0].integer * sizeof(PhysicalObject) + (lenghtsConverter[1].integer + lenghtsConverter[2].integer) * sizeof(SnakeBlock) + i * sizeof(SnakeBlock)];
		}
		game.snakes[2][i] = snakesConv.block;
	}
	for (int i = 0; i < lenghtsConverter[4].integer; i++)
	{
		for (int j = 0; j < sizeof(SnakeBlock); j++)
		{
			snakesConv.bytes[j] = buff[j + 11 + lenghtsConverter[0].integer * sizeof(PhysicalObject) + (lenghtsConverter[1].integer + lenghtsConverter[2].integer + lenghtsConverter[3].integer) * sizeof(SnakeBlock) + i * sizeof(SnakeBlock)];
		}
		game.snakes[3][i] = snakesConv.block;
	}

#ifdef NETWORK_PROFILING
	out.Write("Synchronyzed game!\n");
#endif
	return true;
}

bool ClientNetworkEngine::StopMatch()
{
	char buff[1];
	buff[0] = 0x09;
	send(sock, buff, 1, NULL);
	recv(sock, buff, 1, NULL);
	if (buff[0] != 0x09)
	{
#ifdef NETWORK_PROFILING
		out.Write("Crytical error! Can't stop match!\n");
#endif
		return false;
	}
#ifdef NETWORK_PROFILING
	out.Write("Stopped match.\n");
#endif
	return false;
}

bool ClientNetworkEngine::PauseMatch()
{
	char buff[1];
	buff[0] = 0x0A;
	send(sock, buff, 1, NULL);
	recv(sock, buff, 1, NULL);
	if (buff[0] != 0x0A)
	{
#ifdef NETWORK_PROFILING
		out.Write("Crytical error! Can't pause match\n");
#endif
		return false;
	}
#ifdef NETWORK_PROFILING
	out.Write("Paused match.\n");
#endif
	return false;
}

bool ClientNetworkEngine::UnpauseMatch()
{
	char buff[1];
	buff[0] = 0x0B;
	send(sock, buff, 1, NULL);
	recv(sock, buff, 1, NULL);
	if (buff[0] != 0x0B)
	{
#ifdef NETWORK_PROFILING
		out.Write("Crytical error! Can't unpause match\n");
#endif
		return false;
	}
#ifdef NETWORK_PROFILING
	out.Write("Unpaused match.\n");
#endif
	return false;
}

bool ClientNetworkEngine::Disconnect()
{
	char buff[1];
	buff[0] = 0x0C;
	send(sock, buff, 1, NULL);
	Sleep(100);
	closesocket(sock);
#ifdef NETWORK_PROFILING
	out.Write("Disconnected.\n");
#endif
	return true;
}

bool ClientNetworkEngine::ChangeRoomName(std::wstring* newRoomName)
{
	BytesToShort roomNameSizeConv;
	roomNameSizeConv.integer = (short)newRoomName->size();
	char* buff = new char[1 + roomNameSizeConv.integer * sizeof(wchar_t)];
	buff[0] = 0x0D;
	buff[1] = roomNameSizeConv.bytes[0];
	buff[2] = roomNameSizeConv.bytes[1];
	BytesToWchar roomNameConv;
	for (int i = 0; i < roomNameSizeConv.integer; i++)
	{
		roomNameConv.wchar = newRoomName->at(i);
		buff[3 + i * sizeof(wchar_t)] = roomNameConv.bytes[0];
		buff[4 + i * sizeof(wchar_t)] = roomNameConv.bytes[1];
	}
	send(sock, buff, 3 + roomNameSizeConv.integer, NULL);
	recv(sock, buff, 1, NULL);
	return (bool)buff[0];
}

std::vector<PhysicalObject>& ClientNetworkEngine::GetPhysicsForRenderer()
{
	return game.physics;
}

void ClientNetworkEngine::ZeroBuff(char* firstByte, int sizeOfBuff)
{
	for (int i = 0; i < sizeOfBuff; i++, firstByte++)
	{
		*firstByte = 0;
	}
}