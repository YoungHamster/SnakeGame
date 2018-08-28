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

void NetworkEngine::AcceptingThread(SOCKET listenSock, SOCKADDR_IN address, connection *connections, int maxconnnumber, GameRoom *firstRoom)
{
	connection *firstconnection = connections;
	int addrlen = sizeof(address);
	SOCKET connectSock;
	connection newPlayer;
	while (serverrunning)
	{
		newPlayer.connected = false;
		if (newPlayer.connectSock = accept(listenSock, (SOCKADDR*)&address, &addrlen))
		{
			if (newPlayer.connectSock != INVALID_SOCKET)
			{
				newPlayer.connected = true;
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

void NetworkEngine::Handshake(connection client, GameRoom *firstRoom)
{
	GameRoom *room = firstRoom;
	int nicknamelenght = 0;
	char buff[1500];
	ZeroBuff(&buff[0], 1500);

	recv(client.connectSock, buff, sizeof(buff), NULL);
	if (buff[0] != '\x01')
	{
		std::cout << "pizdets" << std::endl;
		closesocket(client.connectSock);
		client.connected = false;
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
		/*nickname[i] = 0;
		nickname[i] |= buff[i + 1];
		nickname[i] <<= 8;
		nickname[i] |= buff[i + 2];*/
		WcharToBytes nicknameConv;
		nicknameConv.bytes[0] = buff[i + 1];
		nicknameConv.bytes[1] = buff[i + 2];
		nickname[i] = nicknameConv.wchar;
	}

	/* Respone */
	int bufflenght = 9;
	client.nickname = nickname;
	client.uuid |= randNum();
	client.uuid <<= 32;
	client.uuid |= randNum();
	buff[0] = '\x01';
	ULLToBytes uuidConv;
	uuidConv.integer = client.uuid;
	for (int i = 0; i < 8; i++)
	{
		
		buff[i + 1] = uuidConv.bytes[i];
	}
	if (AnyActiveRooms(firstRoom))
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
	}
	send(client.connectSock, buff, bufflenght, NULL);
	
	/* Get */
	ZeroBuff(&buff[0], 1500);
	recv(client.connectSock, buff, sizeof(buff), NULL);
	if (buff[0] != '\x02')
	{
		closesocket(client.connectSock);
		client.connected = false;
		return;
	}
	if (buff[1] == '\x01')
	{
		GameRoom newroom;
		newroom.Init(client);
		std::cout << "Recently connected client data: nickname - " << client.nickname << " uuid - " << client.uuid << " room id - " << newroom.URID << std::endl;
		room = NewRoom(newroom, firstRoom);
		buff[0] = '\x03';
		IntToBytes i2b;
		i2b.integer = room->URID;
		buff[1] = i2b.bytes[0];
		buff[2] = i2b.bytes[1];
		buff[3] = i2b.bytes[2];
		buff[4] = i2b.bytes[3];
		buff[5] = '\0';
		send(client.connectSock, buff, 6, NULL);
		std::thread NewRoom(AsyncRoomThr, room);
		std::thread newPlayer(AsyncUserConnectionThr, room, &room->players[0]);
		newPlayer.detach();
		NewRoom.detach();
		return;
	}
	if (buff[1] == '\x02')
	{
		IntToBytes i2b;
		i2b.bytes[0] = buff[2];
		i2b.bytes[1] = buff[3];
		i2b.bytes[2] = buff[4];
		i2b.bytes[3] = buff[5];
		//BytesToInt(&buff[2], &urid);
		room = firstRoom;
		for (int i = 0; i < 1000; i++, room++)
		{
			if (room->URID == i2b.integer)
			{
				buff[0] = '\x03';
				buff[5] = '\0';
				int newclientid = room->ConnectPlayer(client);
				send(client.connectSock, buff, 6, NULL);
				std::cout << L"Recently connected client data: nickname - " << client.nickname << L" uuid - " << client.uuid << " room id - " << room->URID << std::endl;
				std::thread newPlayer(AsyncUserConnectionThr, room, &room->players[newclientid]);
				newPlayer.detach();
				return;
			}
		}
	}
	std::cout << "pizdets\n";
	client.connected = false;
	closesocket(client.connectSock);
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
		firstRoom++;
	}
}

bool NetworkEngine::AnyActiveRooms(GameRoom *firstRoom)
{
	for (int i = 0; i < 1000; i++)
	{
		if (firstRoom->roomActive) return true;
		/* I surprisingly found that when i add number to pointer i add sizeof(pointer type)*number */
		firstRoom++;
	}
	return false;
}

void NetworkEngine::AsyncRoomThr(GameRoom *room)
{
	int lastTickTime = 0;
	int player1dir, player2dir, player3dir, player4dir;
	int player1tempdir, player2tempdir, player3tempdir, player4tempdir;
	while (serverrunning && room->AnyConnectedPlayers() && room->roomActive)
	{
		Sleep(50);
		if (clock() - lastTickTime >= 100)
		{
			room->OneTick();
			lastTickTime = clock();
		}
	}
}

void NetworkEngine::AsyncUserConnectionThr(GameRoom *room, connection *player)
{
	char buff[1500];
	ZeroBuff(&buff[0], sizeof(buff));
	while (serverrunning && player->connected && room->roomActive)
	{
		Sleep(10);
		recv(player->connectSock, buff, sizeof(buff), NULL);
		if (buff[0] == '\x04')
		{
			player->votedForStart = true;
			for (int i = 0; i < 4; i++)
			{
				if (room->players[i].connected && !room->players[i].votedForStart)
				{
					if (send(player->connectSock, "\x05", 1, NULL) <= 0)
					{
						std::cout << "Pizdec from user with uuid: " << player->uuid << std::endl;
						player->connected = false;
						closesocket(player->connectSock);
						if (!room->AnyConnectedPlayers()) room->roomActive = false;
					}
					continue;
				}
			}
			if (send(player->connectSock, "\x04", 1, NULL) <= 0)
			{
				std::cout << "Pizdec from user with uuid: " << player->uuid << std::endl;
				player->connected = false;
				closesocket(player->connectSock);
				if (!room->AnyConnectedPlayers()) room->roomActive = false;
			}
			//player->votedForStart = false;
			continue;
		}
		if (buff[0] == '\x07')
		{
			if (room->matchRunning)
			{
				switch (buff[1])
				{
				case '\0':
					SendPhysicsToClient(room, player, room->GetPhysicsForPlayer());
					continue;
					break;
				default:
					room->ChangePlayerDirection(buff[1], player);
					SendPhysicsToClient(room, player, room->GetPhysicsForPlayer());
					continue;
					break;
				}
			}
			if(send(player->connectSock, "\x08", 1, NULL) == -1)
			{
				room->matchRunning = false;
				std::cout << "Pizdec from user with uuid: " << player->uuid << std::endl;
				player->connected = false;
				closesocket(player->connectSock);
				if (!room->AnyConnectedPlayers()) room->roomActive = false;
			}
			continue;
		}
		if (buff[0] == '\xff')
		{
			std::cout << "Pizdec packet '\xff' from user with uuid: " << player->uuid << std::endl;
			player->connected = false;
			closesocket(player->connectSock);
			if (!room->AnyConnectedPlayers()) room->roomActive = false;
			continue;
		}
		player->connected = false;
		closesocket(player->connectSock);
		if(!room->AnyConnectedPlayers()) room->roomActive = false;
		/*player = NULL;*/
		std::cout << "Pizdec\n";
	}
}

void NetworkEngine::SendPhysicsToClient(GameRoom *room, connection *client, std::vector<PhysicalObject>& physics)
{
	int physicssize = physics.size();
	//char* physicsbuff = new char[physicssize * sizeof(PhysicalObject) + 1];
	char physicsbuff[46080];
	physicsbuff[0] = '\0f';
	for (int i = 1; i <= physicssize; i++)
	{
		PhysicalObjectToBytes conv;
		conv.obj = physics[i - 1];
		for (int j = 0; j < sizeof(PhysicalObject); j++)
		{
			physicsbuff[i * sizeof(PhysicalObject) + j] = conv.bytes[j];
			
		}
	}
	while (client->RecvedPhysics)
	{
		Sleep(1);
	}
	if (send(client->connectSock, physicsbuff, physicssize * sizeof(PhysicalObject) + 1, NULL) <= 0)
	{
		std::cout << "Pizdec from user with uuid: " << client->uuid << std::endl;
		client->connected = false;
		closesocket(client->connectSock);
		if (!room->AnyConnectedPlayers()) room->roomActive = false;
	}
	//delete[] physicsbuff;
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

void NetworkEngine::ZeroBuff(char *firstByte, int sizeOfBuff)
{
	for (int i = 0; i < sizeOfBuff; i++, firstByte++)
	{
		*firstByte = '\0';
	}
}