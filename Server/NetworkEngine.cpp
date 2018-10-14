#include "NetworkEngine.h"

//void AcceptingThread(SOCKET listenSock, SOCKADDR_IN address, connection *connections, int maxconnnumber, std::vector<connection*> *connectedusersqueue);

bool NetworkEngine::Init()
{
	if (WSAStartup(MAKEWORD(2, 2), &wData) != 0) return false;

	std::cout << "Starting server at port 25565..." << std::endl;
	address.sin_addr.s_addr = inet_addr("0.0.0.0");
	address.sin_family = AF_INET;

	u_short port;
	address.sin_port = htons(25565);

	listenSock = socket(AF_INET, SOCK_STREAM, NULL);
	bind(listenSock, (SOCKADDR*)&address, sizeof(address));
	listen(listenSock, SOMAXCONN);
	std::thread acceptingthr(AcceptingThread, listenSock, address, &players[0], &rooms[0]);
	acceptingthr.detach();
	std::cout << "Server started!" << std::endl;

	functions[0] = SendMainInformationAboutServer;
	functions[1] = SendActiveRoomsIds;
	functions[2] = CreateNewRoom;
	functions[3] = JoinExistingRoom;
	functions[4] = LeaveRoom;
	functions[5] = VoteForStart;
	functions[6] = Pong;
	functions[7] = SendTickInformation;
	functions[8] = SynchronizeGame;
	functions[9] = StopMatch;
	functions[10] = PauseMatch;
	functions[11] = Disconnect;
	return true;
}

void NetworkEngine::AcceptingThread(SOCKET listenSock, SOCKADDR_IN address, connection* firstConnection, GameRoom *firstRoom)
{
	int addrlen = sizeof(address);
	SOCKET connectSock;
	connection *newPlayer = firstConnection;
	while (serverrunning)
	{
		while (newPlayer->connected)
		{
			newPlayer = firstConnection;
			for (int i = 0; i < 10000; i++, newPlayer++)
			{
				if (!newPlayer->connected)
				{
					i = 10000;
				}
			}
		}
		if (newPlayer->connectSock = accept(listenSock, (SOCKADDR*)&address, &addrlen))
		{
			if (newPlayer->connectSock != INVALID_SOCKET)
			{
				newPlayer->connected = true;
				std::cout << "Connection was found:" << inet_ntoa(address.sin_addr) << ':' << address.sin_port << std::endl;
				std::thread handshake(Handshake, newPlayer, firstRoom);
				handshake.detach();
			}
			else
			{
				std::cout << "accept() function error: " << WSAErrorToString() << std::endl;
			}
		}
		//std::cout << "10000 connections was found!!!" << std::endl;
	}
}

void NetworkEngine::AsyncUserConnection(GameRoom *firstRoom, connection connection)
{
	char buff[1024 * 20];
	while (serverrunning && connection.connected)
	{
		recv(connection.connectSock, buff, sizeof(buff), NULL);
		functions[buff[0]](buff, sizeof(buff), firstRoom, &connection);
	}
}

GameRoom* NetworkEngine::NewRoom(GameRoom newroom, GameRoom* firstRoom)
{
	for (int i = 0; i < 1000; i++, firstRoom++)
	{
		if (!firstRoom->roomActive)
		{
			*firstRoom = newroom;
			return firstRoom;
		}
	}
}

void NetworkEngine::AsyncRoomThr(GameRoom *room)
{
	int lastTickTime = 0;
	//double GameSpeed = 1.0;
	while (serverrunning && room->AnyConnectedPlayers() && room->roomActive)
	{
		if (room->GameSpeed > 0)
		{
			Sleep(100 / room->GameSpeed / 2);
		}
		if (clock() - lastTickTime >= 75 / room->GameSpeed && room->matchRunning)
		{
			room->OneTick();
			lastTickTime = clock();
		}
	}
}

const char* NetworkEngine::WSAErrorToString()
{
	switch (WSAGetLastError())
	{
	case WSANOTINITIALISED:
		return "WSAStartup() function failed or wasn't called";
		break;
	case WSAECONNRESET:
		return "an incoming connection was indicated, but was subsequently terminated by the remote peer prior to accepting the call";
		break;
	case WSAEFAULT:
		return "the addrlen parameter is too small or addr is not a valid part of the user address space";
		break;
	case WSAEINTR:
		return "a blocking Windows Sockets 1.1 call was canceled through WSACancelBlockingCall.";
		break;
	case WSAEINVAL:
		return "the listen function was not invoked prior to accept.";
		break;
	case WSAEINPROGRESS:
		return "a blocking Windows Sockets 1.1 call is in progress, or the service provider is still processing a callback function.";
		break;
	case WSAEMFILE:
		return "the queue is nonempty upon entry to accept and there are no descriptors available.";
		break;
	case WSAENETDOWN:
		return "the network subsystem has failed.";
		break;
	case WSAENOBUFS:
		return "no buffer space is available.";
		break;
	case WSAENOTSOCK:
		return "the descriptor is not a socket.";
		break;
	case WSAEOPNOTSUPP:
		return "the referenced socket is not a type that supports connection-oriented service.";
		break;
	case WSAEWOULDBLOCK:
		return "the socket is marked as nonblocking and no connections are present to be accepted.";
		break;
	}
	return "no error occured.";
}

void NetworkEngine::ZeroBuff(char *firstByte, int sizeOfBuff)
{
	for (int i = 0; i < sizeOfBuff; i++)
	{
		firstByte[i] = '\0';
	}
}

bool AnyActiveRooms(GameRoom *firstRoom)
{
	for (int i = 0; i < 1000; i++)
	{
		if (firstRoom->roomActive) return true;
		/* I surprisingly found that when i add number to pointer i add sizeof(pointer type)*number */
		firstRoom++;
	}
	return false;
}

unsigned int randNum()
{
	std::random_device rd;
	std::mt19937 gen(rd());
	//std::uniform_int_distribution<> dist(min, max);
	return gen();
}

int NetworkEngine::Handshake(GameRoom *firstRoom, connection *player)
{
	GameRoom *room = firstRoom;
	int nicknamelenght = 0;
	char buff[1500];
	//ZeroBuff(&buff[0], 1500);
	for (int i = 0; i < sizeof(buff); i++)
	{
		buff[i] = '\0';
	}

	recv(player->connectSock, buff, sizeof(buff), NULL);
	if (buff[0] != '\x01')
	{
		std::cout << "Error while handshaking with player, player sended wrong packet!" << std::endl;
		closesocket(player->connectSock);
		player->connected = false;
		return -1;
	}
	for (int i = 0; i < sizeof(buff) - 1; i++)
	{
		if (buff[i + 1] == '\0') continue;
		nicknamelenght++;
	}
	wchar_t* nickname = new wchar_t[nicknamelenght / 2];
	for (int i = 0; i < nicknamelenght / 2; i++)
	{
		WcharToBytes nicknameConv;
		nicknameConv.bytes[0] = buff[i + 1];
		nicknameConv.bytes[1] = buff[i + 2];
		nickname[i] = nicknameConv.wchar;
	}

	/* Respone */
	int bufflenght = 9;
	player->nickname = nickname;
	player->uuid |= randNum();
	player->uuid <<= 32;
	player->uuid |= randNum();

	/* Write information about user in console */
	std::cout << "New player: UUID-" << player->uuid << " , nikcname-" << player->nickname << std::endl;

	buff[0] = '\x01';
	ULLToBytes uuidConv;
	uuidConv.integer = player->uuid;
	for (int i = 0; i < 8; i++)
	{

		buff[i + 1] = uuidConv.bytes[i];
	}
	/*if (AnyActiveRooms(firstRoom))
	{
		buff[9] = '\x02';
		room = firstRoom;
		for (int i = 0; i < 250; i++, room++)
		{
			if (room->roomActive)
			{
				IntToBytes URIDconv;
				URIDconv.integer = room->URID;
				buff[10 + i * sizeof(int)] = URIDconv.bytes[0];
				buff[11 + i * sizeof(int)] = URIDconv.bytes[1];
				buff[12 + i * sizeof(int)] = URIDconv.bytes[2];
				buff[13 + i * sizeof(int)] = URIDconv.bytes[3];
				buff[14 + i * sizeof(int)] = '\0';
				bufflenght = 14 + i * sizeof(int);
			}
		}
	}
	else
	{
		buff[9] = '\0';
	}*/
	send(player->connectSock, buff, bufflenght, NULL);
	std::thread newUserThread(AsyncUserConnection, firstRoom, player);
}

int SendMainInformationAboutServer(char *buff, int buffSize, GameRoom *firstRoom, connection *player)
{
	char infobuff[100];
	infobuff[0] = 0x00;
}

int SendActiveRoomsIds(char *buff, int buffSize, GameRoom *firstRoom, connection *player)
{
	GameRoom *room = firstRoom;
	std::vector<int> roomsIds;
	for (int i = 0; i < 1000; i++)
	{
		if (room->roomActive)
		{
			roomsIds.push_back(room->URID);
		}
		room++;
	}
	ShortToBytes conv;
	conv.integer = roomsIds.size();
	send(player->connectSock, '\x03' + conv.bytes, 3, NULL);
	char *urids = new char[sizeof(int) * roomsIds.size()];
	IntToBytes idsConv;
	for (int i = 0; i < roomsIds.size(); i++)
	{
		idsConv.integer = roomsIds[i];
		for (int j = 0; j < sizeof(int); j++)
		{
			urids[i * sizeof(int) + j] = idsConv.bytes[j];
		}
	}
	send(player->connectSock, urids, sizeof(int) * roomsIds.size(), NULL);
}

int CreateNewRoom(char *buff, int buffSize, GameRoom *firstRoom, connection *player)
{
	for (int i = 0; i < 1000; i++)
	{
		if (!firstRoom[i].roomActive)
		{
			firstRoom[i].Init(player);
			player->gameRoom = &firstRoom[i];
			player->roomIdInRoomsArray = i;
			char buff[3];
			buff[0] = 0x03;
			ShortToBytes conv;
			conv.integer = i;
			buff[1] = conv.bytes[0];
			buff[2] = conv.bytes[1];
			send(player->connectSock, buff, 3, NULL);
			return 0;
		}
	}
}

int JoinExistingRoom(char *buff, int buffSize, GameRoom *firstRoom, connection *player)
{
	for (int i = 0; i < 1000; i++)
	{
		if (firstRoom[i].roomActive)
		{
			if (firstRoom[i].ConnectPlayer(player) == -1)
			{
				return -1; // probably all places in room are taken
			}
			else
			{
				player->gameRoom = &firstRoom[i];
				player->roomIdInRoomsArray = i;
				buff[0] = 0x04;
				ShortToBytes conv;
				conv.integer = i;
				buff[1] = conv.bytes[0];
				buff[2] = conv.bytes[1];
				send(player->connectSock, buff, 3, NULL);
			}
		}
	}
}

int LeaveRoom(char *buff, int buffSize, GameRoom *firstRoom, connection *player)
{
	firstRoom[player->roomIdInRoomsArray].DisconnectPlayer(player);
	player->roomIdInRoomsArray = -1;
	player->gameRoom = NULL;
	char respone = 0x05;
	send(player->connectSock, &respone, 1, NULL);
}

int VoteForStart(char* buff, int buffSize, GameRoom *firstRoom, connection *player)
{
	if (firstRoom[player->roomIdInRoomsArray].matchRunning)
	{
		return -2;
	}
	char respone[2];
	respone[0] = 0x06;
	player->votedForStart = true;
	for (int i = 0; i < 4; i++)
	{
		if (firstRoom[player->roomIdInRoomsArray].players[i] != NULL)
		{
			if (firstRoom[player->roomIdInRoomsArray].players[i]->votedForStart == false)
			{
				respone[1] = false;
				send(player->connectSock, respone, 2, NULL);
				return -1;
			}
		}
	}
	firstRoom[player->roomIdInRoomsArray].matchRunning = true;
	respone[0] = true;
	send(player->connectSock, respone, 2, NULL);
	return 0;
}


/* Player sends 512bytes to server and server immidiatly respones with 512bytes and counts how many time it took then we count speed of connection and ping */
int Pong(char* buff, int buffSize, GameRoom *firstRoom, connection *player)
{
	char buff[512];
	auto time = std::chrono::high_resolution_clock::now();
	recv(player->connectSock, buff, sizeof(buff), NULL);
	send(player->connectSock, buff, sizeof(buff), NULL);
	auto secondTime = std::chrono::high_resolution_clock::now();
	long long delta = std::chrono::duration_cast<std::chrono::nanoseconds>(secondTime - time).count();
	long long ping = delta;
	double kbpersec = 1000 * 1000 * 1000 / delta;
	std::cout << "Statistics of player with uuid-" << player->uuid << ": speed=" << kbpersec << " kb/sec, ping = " << ping / 1000000 << "ms, or " << ping << "ns" << std::endl;
}

int SendTickInformation(char* buff, int buffSize, GameRoom *firstRoom, connection *player)
{
	// 2 BYTES-HEADER, 4 BYTES-SNAKESDIRECTIONS, 4 BYTES-2 SHORTS(APPLE AABB.min), 8 BYTES - 4 SHORTS(LENGHTS OF SNAKES)
	char oneTickPacket[2 + 4 + 4 + 8];
	oneTickPacket[0] = 0x08;
	oneTickPacket[1] = 0x01;
	oneTickPacket[2] = firstRoom[player->roomIdInRoomsArray].players[0]->gameDir;
	oneTickPacket[3] = firstRoom[player->roomIdInRoomsArray].players[1]->gameDir;
	oneTickPacket[4] = firstRoom[player->roomIdInRoomsArray].players[2]->gameDir;
	oneTickPacket[5] = firstRoom[player->roomIdInRoomsArray].players[3]->gameDir;
	ShortToBytes converter;
	for (int i = 0; i < firstRoom[player->roomIdInRoomsArray].GetPhysicsForPlayer().size(); i++)
	{
		if (firstRoom[player->roomIdInRoomsArray].GetPhysicsForPlayer()[i].type == APPLE)
		{
			converter.integer = firstRoom[player->roomIdInRoomsArray].GetPhysicsForPlayer()[i].borders.min.x;
			oneTickPacket[6] = converter.bytes[0];
			oneTickPacket[7] = converter.bytes[1];
			converter.integer = firstRoom[player->roomIdInRoomsArray].GetPhysicsForPlayer()[i].borders.min.y;
			oneTickPacket[8] = converter.bytes[0];
			oneTickPacket[9] = converter.bytes[1];
			i = firstRoom[player->roomIdInRoomsArray].GetPhysicsForPlayer().size();
		}
	}
	for (int i = 0; i < 4; i++)
	{
		converter.integer = firstRoom[player->roomIdInRoomsArray].GetSnakeLenght(i);
		oneTickPacket[10 + i * 2] = converter.bytes[0];
		oneTickPacket[11 + i * 2] = converter.bytes[1];
	}
	return send(player->connectSock, oneTickPacket, sizeof(oneTickPacket), NULL);
}

int SynchronizeGame(char* buff, int buffSize, GameRoom *firstRoom, connection *player)
{
	short snake1Size = firstRoom[player->roomIdInRoomsArray].GetSnakeLenght(0);
	short snake2Size = firstRoom[player->roomIdInRoomsArray].GetSnakeLenght(1);
	short snake3Size = firstRoom[player->roomIdInRoomsArray].GetSnakeLenght(2);
	short snake4Size = firstRoom[player->roomIdInRoomsArray].GetSnakeLenght(3);
	short physicsSize = firstRoom[player->roomIdInRoomsArray].GetPhysicsForPlayer().size();
	// 2 bytes - packet header
	// 10 bytes - 5shorts: lenght of physics vector, lenghts of 4 SnakeBlocks vector
	short buffsize = physicsSize * sizeof(PhysicalObject) + (snake1Size + snake2Size + snake3Size + snake4Size) * sizeof(SnakeBlock) + 2 + 10;
	ShortToBytes buffsizeConverter; // union
	buffsizeConverter.integer = buffsize;
	send(player->connectSock, buffsizeConverter.bytes, sizeof(ShortToBytes), NULL); // send size of buffer for экономия of memory on client side
	char *buff = new char[buffsize];
	buff[0] = 0x09;
	ShortToBytes lenghtsConverter; // union
	lenghtsConverter.integer = physicsSize;
	buff[1] = lenghtsConverter.bytes[0];
	buff[2] = lenghtsConverter.bytes[1];

	for (int i = 0; i < 4; i++)
	{
		lenghtsConverter.integer = firstRoom[player->roomIdInRoomsArray].GetSnakeLenght(i);
		buff[3 + i * sizeof(short)] = lenghtsConverter.bytes[0];
		buff[4 + i * sizeof(short)] = lenghtsConverter.bytes[1];
	}

	PhysicalObjectToBytes physicalObjectsConv; // union
	for (int i = 0; i < physicsSize; i++)
	{
		physicalObjectsConv.obj = firstRoom[player->roomIdInRoomsArray].GetPhysicsForPlayer()[i];
		for (int j = 0; j < sizeof(PhysicalObject); j++)
		{
			buff[i * sizeof(PhysicalObject) + j + 11] = physicalObjectsConv.bytes[j];
		}
	}
	SnakeBlockToBytes snakeBlocksConv; // union
	for (int i = 0; i < snake1Size; i++)
	{
		snakeBlocksConv.block = firstRoom[player->roomIdInRoomsArray].GetSnake(0)[i];
		int bytenum = i * sizeof(SnakeBlock) + physicsSize * sizeof(PhysicalObject) + 11;
		for (int j = 0; j < sizeof(SnakeBlock); j++)
		{
			buff[j + bytenum] = snakeBlocksConv.bytes[j];
		}
	}
	for (int i = 0; i < snake2Size; i++)
	{
		snakeBlocksConv.block = firstRoom[player->roomIdInRoomsArray].GetSnake(1)[i];
		int bytenum = snake1Size * sizeof(SnakeBlock) + i * sizeof(SnakeBlock) + physicsSize * sizeof(PhysicalObject) + 11;
		for (int j = 0; j < sizeof(SnakeBlock); j++)
		{
			buff[j + bytenum] = snakeBlocksConv.bytes[j];
		}
	}
	for (int i = 0; i < snake3Size; i++)
	{
		snakeBlocksConv.block = firstRoom[player->roomIdInRoomsArray].GetSnake(2)[i];
		int bytenum = snake1Size * sizeof(SnakeBlock) + snake2Size * sizeof(SnakeBlock) + i * sizeof(SnakeBlock) + physicsSize * sizeof(PhysicalObject) + 11;
		for (int j = 0; j < sizeof(SnakeBlock); j++)
		{
			buff[j + bytenum] = snakeBlocksConv.bytes[j];
		}
	}
	for (int i = 0; i < snake4Size; i++)
	{
		snakeBlocksConv.block = firstRoom[player->roomIdInRoomsArray].GetSnake(3)[i];
		int bytenum = snake1Size * sizeof(SnakeBlock) + snake2Size * sizeof(SnakeBlock) + snake3Size * sizeof(SnakeBlock) + i * sizeof(SnakeBlock) + physicsSize * sizeof(PhysicalObject) + 11;
		for (int j = 0; j < sizeof(SnakeBlock); j++)
		{
			buff[j + bytenum] = snakeBlocksConv.bytes[j];
		}
	}
	return send(player->connectSock, buff, buffsize, NULL);
}

int StopMatch(char* buff, int buffSize, GameRoom *firstRoom, connection *player)
{
	char respone;
	respone = 0x0A;
	firstRoom[player->roomIdInRoomsArray].matchRunning = false;
	firstRoom[player->roomIdInRoomsArray].ReloadGame();
	send(player->connectSock, &respone, 1, NULL);
}

int PauseMatch(char* buff, int buffSize, GameRoom *firstRoom, connection *player)
{
	char respone;
	respone = 0x0B;
	firstRoom[player->roomIdInRoomsArray].matchRunning = false;
	send(player->connectSock, &respone, 1, NULL);
}

int Disconnect(char* buff, int buffSize, GameRoom *firstRoom, connection *player)
{
	firstRoom[player->roomIdInRoomsArray].DisconnectPlayer(player);
	player->connected = false;
	player->gameRoom = NULL;
	closesocket(player->connectSock);
	return 0;
}