#include "NetworkEngine.h"

//void AcceptingThread(SOCKET listenSock, SOCKADDR_IN address, connection *connections, int maxconnnumber, std::vector<connection*> *connectedusersqueue);

bool NetworkEngine::Init()
{
	if (WSAStartup(MAKEWORD(2, 2), &wData) != 0) return false;

	address.sin_addr.s_addr = inet_addr("0.0.0.0");
	address.sin_family = AF_INET;

	u_short port;
	std::cout << "Enter open port for server: ";
	std::cin >> port;
	std::cout << std::endl;
	address.sin_port = htons(25565);

	listenSock = socket(AF_INET, SOCK_STREAM, NULL);
	bind(listenSock, (SOCKADDR*)&address, sizeof(address));
	listen(listenSock, SOMAXCONN);
	std::thread acceptingthr(AcceptingThread, listenSock, address, &connections[0], 1000, &rooms[0]);
	acceptingthr.detach();
	return true;
}

void NetworkEngine::AcceptingThread(SOCKET listenSock, SOCKADDR_IN address, connection *connections, int maxconnnumber, GameRoom *rooms)
{
	connection *firstconnection = connections;
	int addrlen = sizeof(address);
	SOCKET connectSock;
	while (serverrunning)
	{
		connections = firstconnection;
		for (int i = 0; i < maxconnnumber; i++, connections += sizeof(connection))
		{
			if (!connections->connected)
			{
				if (connectSock = accept(listenSock, (SOCKADDR*)&address, &addrlen))
				{
					if (connectSock != INVALID_SOCKET)
					{
						connections->connected = true;
						connections->connectSock = connectSock;
						std::cout << "Connection was found:" << inet_ntoa(address.sin_addr) << ':' << address.sin_port << std::endl;
						std::thread handshake(Handshake, connections, rooms);
						handshake.detach();
					}
					else
					{
						std::cout << "accept() function error: " << WSAErrorToString() << std::endl;
					}
				}
			}
		}
		std::cout << "10000 connections was found!!!" << std::endl;
	}
}

void NetworkEngine::Handshake(connection *client, GameRoom *firstRoom)
{
	GameRoom *room = firstRoom;
	int nicknamelenght = 0;
	char buff[1500];
	recv(client->connectSock, buff, sizeof(buff), NULL);
	if (buff[0] != '\x01')
	{
		closesocket(client->connectSock);
		client->connected = false;
		return;
	}
	for (int i = 0; i < sizeof(buff); i++)
	{
		if (buff[i + 1] == '\0') continue;
		nicknamelenght++;
	}
	wchar_t* nickname = new wchar_t[nicknamelenght / 2];
	for (int i = 0; i < nicknamelenght / 2; i++)
	{
		nickname[i] = 0;
		nickname[i] |= buff[i + 1];
		nickname[i] <<= 8;
		nickname[i] |= buff[i + 2];
	}
	client->nickname = nickname;
	client->uuid |= randNum();
	client->uuid <<= 32;
	client->uuid |= randNum();
	buff[0] = '\x01';
	for (int i = 0; i < 8; i++)
	{
		buff[i + 1] = client->uuid >> (1 + i) * 8;
	}
	if (AnyActiveRooms(firstRoom))
	{
		buff[9] = '\x02';
		room = firstRoom;
		for (int i = 0; i < 250; i++)
		{
			/*buff[10 + i * sizeof(int)] = room->urid;
			buff[11 + i * sizeof(int)] = room->urid >> 8;
			buff[12 + i * sizeof(int)] = room->urid >> 16;
			buff[13 + i * sizeof(int)] = room->urid >> 24;*/
			BytesToInt(&buff[10 + i * sizeof(int)], &room->URID);
			buff[14 + i * sizeof(int)] = '\0';
			room += sizeof(GameRoom);
		}
	}
	else
	{
		buff[9] = '\0';
	}
	send(client->connectSock, buff, sizeof(buff), NULL);

	recv(client->connectSock, buff, sizeof(buff), NULL);
	if (buff[0] != '\x02')
	{
		closesocket(client->connectSock);
		client->connected = false;
		return;
	}
	if (buff[1] == '\x01')
	{
		GameRoom newroom;
		newroom.Init(client);
		std::wcout << L"Recently connected client data: nickname - " << client->nickname; std::wcout << L" uuid - " << client->uuid; std::wcout << " room id - " << newroom.URID << std::endl;
		room = NewRoom(newroom, firstRoom);
		buff[0] = '\x03';
		IntToBytes(&buff[1], &room->URID);
		buff[5] = '\0';
		send(client->connectSock, buff, 6, NULL);
		std::thread NewRoom(AsyncRoomThr, room);
		std::thread newPlayer(AsyncUserConnectionThr, room, client);
		newPlayer.detach();
		NewRoom.detach();
		return;
	}
	if (buff[1] == '\x02')
	{
		int urid;
		/*urid |= buff[2];
		urid <<= 8;
		urid |= buff[3];
		urid <<= 8;
		urid |= buff[4];
		urid <<= 8;
		urid |= buff[5];*/
		BytesToInt(&buff[2], &urid);
		room = firstRoom;
		for (int i = 0; i < 250; i++)
		{
			if (room->URID == urid)
			{
				buff[0] = '\x03';
				/*buff[1] = room->urid;
				buff[2] = room->urid >> 8;
				buff[3] = room->urid >> 16;
				buff[4] = room->urid >> 24;*/
				IntToBytes(&buff[1], &room->URID);
				buff[5] = '\0';
				send(client->connectSock, buff, 6, NULL);
				room->ConnectPlayer(client);
				std::thread newPlayer(AsyncUserConnectionThr, room, client);
				newPlayer.detach();
				std::wcout << L"Recently connected client data: nickname - " << client->nickname << L" uuid - " << client->uuid << " room id - " << urid << std::endl;
				return;
			}
			room += sizeof(GameRoom);
		}
	}
	client->connected = false;
	closesocket(client->connectSock);
}

GameRoom* NetworkEngine::NewRoom(GameRoom newroom, GameRoom* firstRoom)
{
	for (int i = 0; i < 250; i++)
	{
		if (!firstRoom->roomActive)
		{
			*firstRoom = newroom;
			return firstRoom;
		}
		firstRoom += sizeof(GameRoom);
	}
}

bool NetworkEngine::AnyActiveRooms(GameRoom *firstRoom)
{
	for (int i = 0; i < 1000; i++)
	{
		if (firstRoom->roomActive) return true;
		firstRoom += sizeof(GameRoom);
	}
	return false;
}

void NetworkEngine::AsyncRoomThr(GameRoom *room)
{
	int player1dir, player2dir, player3dir, player4dir;
	int player1tempdir, player2tempdir, player3tempdir, player4tempdir;
	while (serverrunning && room->AnyConnectedPlayers())
	{
		room->OneTick();
	}
}

void NetworkEngine::AsyncUserConnectionThr(GameRoom *room, connection *player)
{
	char buff[1500];
	while (serverrunning && room->roomActive)
	{
		recv(player->connectSock, buff, sizeof(buff), NULL);
		if (buff[0] == '')
		{

		}
	}
}

void NetworkEngine::SendPhysicsToClient(connection *client, std::vector<PhysicalObject>& physics)
{
	char* physicsbuff = new char[physics.size() * sizeof(PhysicalObject) + 1];
	physicsbuff[0] = '\0f';
	for (int i = 1; i < physics.size(); i++)
	{
		physicsbuff[i * sizeof(PhysicalObject)] = physics[i].type;
		physicsbuff[i * sizeof(PhysicalObject) + 1] = physics[i].type >> 8;
		physicsbuff[i * sizeof(PhysicalObject) + 2] = physics[i].type >> 16;
		physicsbuff[i * sizeof(PhysicalObject) + 3] = physics[i].type >> 24;
		physicsbuff[i * sizeof(PhysicalObject) + 4] = physics[i].borders.max.x;
		physicsbuff[i * sizeof(PhysicalObject) + 5] = physics[i].borders.max.x >> 8;
		physicsbuff[i * sizeof(PhysicalObject) + 6] = physics[i].borders.max.x >> 16;
		physicsbuff[i * sizeof(PhysicalObject) + 7] = physics[i].borders.max.x >> 24;
		physicsbuff[i * sizeof(PhysicalObject) + 8] = physics[i].borders.max.y;
		physicsbuff[i * sizeof(PhysicalObject) + 9] = physics[i].borders.max.y >> 8;
		physicsbuff[i * sizeof(PhysicalObject) + 10] = physics[i].borders.min.y >> 16;
		physicsbuff[i * sizeof(PhysicalObject) + 12] = physics[i].borders.min.y >> 24;
		physicsbuff[i * sizeof(PhysicalObject) + 13] = physics[i].borders.min.x;
		physicsbuff[i * sizeof(PhysicalObject) + 14] = physics[i].borders.min.x >> 8;
		physicsbuff[i * sizeof(PhysicalObject) + 15] = physics[i].borders.min.x >> 16;
		physicsbuff[i * sizeof(PhysicalObject) + 16] = physics[i].borders.min.x >> 24;
		physicsbuff[i * sizeof(PhysicalObject) + 17] = physics[i].borders.min.y;
		physicsbuff[i * sizeof(PhysicalObject) + 18] = physics[i].borders.min.y >> 8;
		physicsbuff[i * sizeof(PhysicalObject) + 19] = physics[i].borders.min.y >> 16;
		physicsbuff[i * sizeof(PhysicalObject) + 20] = physics[i].borders.min.y >> 24;
	}
	send(client->connectSock, physicsbuff, physics.size() * sizeof(PhysicalObject) + 1, NULL);
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

unsigned int NetworkEngine::randNum()
{
	std::random_device rd;
	std::mt19937 gen(rd());
	//std::uniform_int_distribution<> dist(min, max);
	return gen();
}

void NetworkEngine::IntToBytes(char *firstByte, int *integer)
{
	*firstByte = *integer;
	firstByte++;
	*firstByte = *integer >> 8;
	firstByte++;
	*firstByte = *integer >> 16;
	firstByte++;
	*firstByte = *integer >> 24;
}

void NetworkEngine::BytesToInt(char *firstByte, int *integer)
{
	*integer |= *firstByte;
	*integer <<= 8;
	firstByte++;
	*integer |= *firstByte;
	*integer <<= 8;
	firstByte++;
	*integer |= *firstByte;
	*integer <<= 8;
	firstByte++;
	*integer |= *firstByte;
}