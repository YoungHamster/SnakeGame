#include "ServerNetworkEngine.h"

int SendMainInformationAboutServer(char *buff, int buffSize, GameRoom *firstRoom, connection *player);
int SendActiveRoomsIds(char *buff, int buffSize, GameRoom *firstRoom, connection *player);
int CreateNewRoom(char *buff, int buffSize, GameRoom *firstRoom, connection *player);
int JoinExistingRoom(char *buff, int buffSize, GameRoom *firstRoom, connection *player);
int LeaveRoom(char *buff, int buffSize, GameRoom *firstRoom, connection *player);
int VoteForStart(char* buff, int buffSize, GameRoom *firstRoom, connection *player);
int Pong(char* buff, int buffSize, GameRoom *firstRoom, connection *player);
int SendTickInformation(char* buff, int buffSize, GameRoom *firstRoom, connection *player);
int SynchronizeGame(char* buff, int buffSize, GameRoom *firstRoom, connection *player);
int StopMatch(char* buff, int buffSize, GameRoom *firstRoom, connection *player);
int PauseMatch(char* buff, int buffSize, GameRoom *firstRoom, connection *player);
int Disconnect(char* buff, int buffSize, GameRoom *firstRoom, connection *player);

bool NetworkEngine::Init()
{
	if (WSAStartup(MAKEWORD(2, 2), &wData) != 0) return false;

	std::cout << "Starting server at port 25565..." << std::endl;
	address.sin_addr.s_addr = inet_addr("0.0.0.0");
	address.sin_family = AF_INET;

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
				std::thread handshake(Handshake, firstRoom, newPlayer);
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

void NetworkEngine::AsyncUserConnection(GameRoom *firstRoom, connection *connection)
{
	char buff[1024 * 20];
	while (serverrunning && connection->connected)
	{
		if (recv(connection->connectSock, buff, sizeof(buff), NULL) <= 0)
		{
			closesocket(connection->connectSock);
			return;
		}
		functions[buff[0]](buff, sizeof(buff), firstRoom, connection);
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
	return NULL;
}

void NetworkEngine::AsyncRoomThr(GameRoom *room)
{
	int lastTickTime = 0;
	//double GameSpeed = 1.0;
	while (serverrunning && room->AnyConnectedPlayers() && room->roomActive)
	{
		if (room->GameSpeed > 0)
		{
			Sleep(100 / (DWORD)room->GameSpeed / 2);
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
	for (int i = 0; i < NUMBER_OF_ROOMS; i++)
	{
		if (firstRoom[i].roomActive) return true;
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
	int nicknamelenght = 0;
	char buff[1500];
	//ZeroBuff(&buff[0], 1500);
	for (int i = 0; i < sizeof(buff); i++)
	{
		buff[i] = '\0';
	}

	recv(player->connectSock, buff, sizeof(buff), NULL);
	if (buff[0] > 129)
	{
		std::cout << "Error while handshaking with player, player sended wrong packet!" << std::endl;
		closesocket(player->connectSock);
		player->connected = false;
		return -1;
	}
	nicknamelenght = buff[0];
	wchar_t* nickname = new wchar_t[nicknamelenght / 2];
	WcharToBytes nicknameConv;
	for (int i = 0; i < nicknamelenght / 2; i++)
	{
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
	std::cout << "New player: UUID-" << player->uuid; std::wcout << " , nickname-" << player->nickname << std::endl;

	buff[0] = '\x01';
	ULLToBytes uuidConv;
	uuidConv.integer = player->uuid;
	for (int i = 0; i < 8; i++)
	{

		buff[i + 1] = uuidConv.bytes[i];
	}
	send(player->connectSock, buff, bufflenght, NULL);
	std::thread newUserThread(AsyncUserConnection, firstRoom, player);
	newUserThread.detach();
	return 0;
}

int SendMainInformationAboutServer(char *buff, int buffSize, GameRoom *firstRoom, connection *player)
{
	if (buff[0] != 0x00)
	{
		return -1;
	}
	char infobuff[100];
	infobuff[0] = 0x00;
	return 0;
}

int SendActiveRoomsIds(char *buff, int buffSize, GameRoom *firstRoom, connection *player)
{
	if (buff[0] != 0x01)
	{
		return -1;
	}
	GameRoom *room = firstRoom;
	char activeRoomsIds[4000];
	int numberOfActiveRooms = 0;
	IntToBytes idsConv;
	for (int i = 0; i < NUMBER_OF_ROOMS; i++)
	{
		if (firstRoom[i].roomActive)
		{
			idsConv.integer = i;
			for (int j = 0; j < sizeof(int); j++)
			{
				activeRoomsIds[i * sizeof(int) + j] = idsConv.bytes[j];
			}
			numberOfActiveRooms++;
		}
	}
	send(player->connectSock, activeRoomsIds, sizeof(int) * numberOfActiveRooms, NULL);
	return 0;
}

int CreateNewRoom(char *buff, int buffSize, GameRoom *firstRoom, connection *player)
{
	if (buff[0] != 0x02)
	{
		return -1;
	}
	for (int i = 0; i < NUMBER_OF_ROOMS; i++)
	{
		if (!firstRoom[i].roomActive)
		{
			firstRoom[i].Init(player);
			player->roomIdInRoomsArray = i;
			char buff[3];
			buff[0] = 0x02;
			ShortToBytes conv;
			conv.integer = i;
			buff[1] = conv.bytes[0];
			buff[2] = conv.bytes[1];
			send(player->connectSock, buff, 3, NULL);
			return 0;
		}
	}
	return -2;
}

int JoinExistingRoom(char *buff, int buffSize, GameRoom *firstRoom, connection *player)
{
	if (buff[0] != 0x03)
	{
		return -1;
	}
	ShortToBytes roomIDconv;
	roomIDconv.bytes[0] = buff[1];
	roomIDconv.bytes[1] = buff[2];
	if (roomIDconv.integer < 0 || roomIDconv.integer > NUMBER_OF_ROOMS)
	{
		return -2;
	}
	if (!firstRoom[roomIDconv.integer].roomActive)
	{
		send(player->connectSock, "\xff", 1, NULL);
		return -3;
	}
	if (firstRoom[roomIDconv.integer].ConnectPlayer(player) == -1)
	{
		send(player->connectSock, "\xff", 1, NULL);
		return -4;
	}
	send(player->connectSock, "\x03", 1, NULL);
	return 0;
}

int LeaveRoom(char *buff, int buffSize, GameRoom *firstRoom, connection *player)
{
	if (buff[0] != 0x04)
	{
		return -1;
	}
	firstRoom[player->roomIdInRoomsArray].DisconnectPlayer(player);
	player->roomIdInRoomsArray = -1;
	char respone = 0x04;
	send(player->connectSock, &respone, 1, NULL);
	return 0;
}

int VoteForStart(char* buff, int buffSize, GameRoom *firstRoom, connection *player)
{
	if (buff[0] != 0x05)
	{
		return -1;
	}
	if (firstRoom[player->roomIdInRoomsArray].matchRunning)
	{
		return -2;
	}
	char respone[2];
	respone[0] = 0x05;
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
	respone[1] = true;
	send(player->connectSock, respone, 2, NULL);
	return 0;
}

/* Player sends 512bytes to server and server immidiatly respones with 512bytes and counts how many time it took then we count speed of connection and ping */
int Pong(char* buff, int buffSize, GameRoom *firstRoom, connection *player)
{
	if (buff[0] != 0x06)
	{
		return -1;
	}
	char buffer[512];
	auto time = std::chrono::high_resolution_clock::now();
	recv(player->connectSock, buffer, sizeof(buffer), NULL);
	send(player->connectSock, buffer, sizeof(buffer), NULL);
	auto secondTime = std::chrono::high_resolution_clock::now();
	long long ping = std::chrono::duration_cast<std::chrono::nanoseconds>(secondTime - time).count();
	double kbpersec = 1000 * 1000 * 1000 / (double)ping;
	std::cout << "Statistics of " << player->nickname << " with uuid-" << player->uuid << ": speed = " << kbpersec << "kb/sec, ping = " << ping / 1000000 << "ms, or " << ping << "ns" << std::endl;
	return 0;
}

int SendTickInformation(char* buff, int buffSize, GameRoom *firstRoom, connection *player)
{
	if (buff[0] != 0x07)
	{
		return -1;
	}
	// 2 BYTES-HEADER, 4 BYTES-SNAKESDIRECTIONS, 4 BYTES-2 SHORTS(APPLE AABB.min), 8 BYTES - 4 SHORTS(LENGHTS OF SNAKES)
	char oneTickPacket[2 + 4 + 4 + 8];
	oneTickPacket[0] = 0x07;
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
			i = (int)firstRoom[player->roomIdInRoomsArray].GetPhysicsForPlayer().size();
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
	if (buff[0] != 0x08)
	{
		return -1;
	}
	short snake1Size = firstRoom[player->roomIdInRoomsArray].GetSnakeLenght(0);
	short snake2Size = firstRoom[player->roomIdInRoomsArray].GetSnakeLenght(1);
	short snake3Size = firstRoom[player->roomIdInRoomsArray].GetSnakeLenght(2);
	short snake4Size = firstRoom[player->roomIdInRoomsArray].GetSnakeLenght(3);
	short physicsSize = firstRoom[player->roomIdInRoomsArray].GetPhysicsForPlayer().size();
	// 1 byte - packet header
	// 10 bytes - 5shorts: lenght of physics vector, lenghts of 4 SnakeBlocks vector
	short buffsize = physicsSize * sizeof(PhysicalObject) + (snake1Size + snake2Size + snake3Size + snake4Size) * sizeof(SnakeBlock) + 1 + 10;
	ShortToBytes buffsizeConverter; // union
	buffsizeConverter.integer = buffsize;
	send(player->connectSock, buffsizeConverter.bytes, sizeof(ShortToBytes), NULL); // send size of buffer for экономия of memory on client side
	char *buffer = new char[buffsize];
	buffer[0] = 0x08;
	ShortToBytes lenghtsConverter; // union
	lenghtsConverter.integer = physicsSize;
	buffer[1] = lenghtsConverter.bytes[0];
	buffer[2] = lenghtsConverter.bytes[1];

	for (int i = 0; i < 4; i++)
	{
		lenghtsConverter.integer = firstRoom[player->roomIdInRoomsArray].GetSnakeLenght(i);
		buffer[3 + i * sizeof(short)] = lenghtsConverter.bytes[0];
		buffer[4 + i * sizeof(short)] = lenghtsConverter.bytes[1];
	}

	PhysicalObjectToBytes physicalObjectsConv; // union
	for (int i = 0; i < physicsSize; i++)
	{
		physicalObjectsConv.obj = firstRoom[player->roomIdInRoomsArray].GetPhysicsForPlayer()[i];
		for (int j = 0; j < sizeof(PhysicalObject); j++)
		{
			buffer[i * sizeof(PhysicalObject) + j + 11] = physicalObjectsConv.bytes[j];
		}
	}
	SnakeBlockToBytes snakeBlocksConv; // union
	for (int i = 0; i < snake1Size; i++)
	{
		snakeBlocksConv.block = firstRoom[player->roomIdInRoomsArray].GetSnake(0)[i];
		int bytenum = i * sizeof(SnakeBlock) + physicsSize * sizeof(PhysicalObject) + 11;
		for (int j = 0; j < sizeof(SnakeBlock); j++)
		{
			buffer[j + bytenum] = snakeBlocksConv.bytes[j];
		}
	}
	for (int i = 0; i < snake2Size; i++)
	{
		snakeBlocksConv.block = firstRoom[player->roomIdInRoomsArray].GetSnake(1)[i];
		int bytenum = snake1Size * sizeof(SnakeBlock) + i * sizeof(SnakeBlock) + physicsSize * sizeof(PhysicalObject) + 11;
		for (int j = 0; j < sizeof(SnakeBlock); j++)
		{
			buffer[j + bytenum] = snakeBlocksConv.bytes[j];
		}
	}
	for (int i = 0; i < snake3Size; i++)
	{
		snakeBlocksConv.block = firstRoom[player->roomIdInRoomsArray].GetSnake(2)[i];
		int bytenum = snake1Size * sizeof(SnakeBlock) + snake2Size * sizeof(SnakeBlock) + i * sizeof(SnakeBlock) + physicsSize * sizeof(PhysicalObject) + 11;
		for (int j = 0; j < sizeof(SnakeBlock); j++)
		{
			buffer[j + bytenum] = snakeBlocksConv.bytes[j];
		}
	}
	for (int i = 0; i < snake4Size; i++)
	{
		snakeBlocksConv.block = firstRoom[player->roomIdInRoomsArray].GetSnake(3)[i];
		int bytenum = snake1Size * sizeof(SnakeBlock) + snake2Size * sizeof(SnakeBlock) + snake3Size * sizeof(SnakeBlock) + i * sizeof(SnakeBlock) + physicsSize * sizeof(PhysicalObject) + 11;
		for (int j = 0; j < sizeof(SnakeBlock); j++)
		{
			buffer[j + bytenum] = snakeBlocksConv.bytes[j];
		}
	}
	return send(player->connectSock, buffer, buffsize, NULL);
}

int StopMatch(char* buff, int buffSize, GameRoom *firstRoom, connection *player)
{
	if (buff[0] != 0x09)
	{
		return -1;
	}
	char respone;
	respone = 0x09;
	firstRoom[player->roomIdInRoomsArray].matchRunning = false;
	firstRoom[player->roomIdInRoomsArray].ReloadGame();
	send(player->connectSock, &respone, 1, NULL);
	return 0;
}

int PauseMatch(char* buff, int buffSize, GameRoom *firstRoom, connection *player)
{
	if (buff[0] != 0x0A)
	{
		return -1;
	}
	char respone;
	respone = 0x0A;
	firstRoom[player->roomIdInRoomsArray].matchRunning = false;
	send(player->connectSock, &respone, 1, NULL);
	return 0;
}

int Disconnect(char* buff, int buffSize, GameRoom *firstRoom, connection *player)
{
	if (buff[0] != 0x0B)
	{
		return -1;
	}
	firstRoom[player->roomIdInRoomsArray].DisconnectPlayer(player);
	player->connected = false;
	//player->gameRoom = NULL;
	closesocket(player->connectSock);
	return 0;
}