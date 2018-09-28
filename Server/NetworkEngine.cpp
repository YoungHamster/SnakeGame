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
	std::thread acceptingthr(AcceptingThread, listenSock, address, 1000, &rooms[0]);
	acceptingthr.detach();
	std::cout << "Server started!" << std::endl;
	return true;
}

void NetworkEngine::AcceptingThread(SOCKET listenSock, SOCKADDR_IN address, int maxconnnumber, GameRoom *firstRoom)
{
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
		std::cout << "Error while handshaking with player" << std::endl;
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
		NewRoom.detach();
		/* Instead of creating new thread i call this function in this thread */
		/*std::thread newPlayer(AsyncUserConnectionThr, room, &room->players[0]);
		newPlayer.detach();*/
		AsyncUserConnectionThr(room, &room->players[0]);
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

				/* Instead of creating new thread i call this function in this thread */
				/*std::thread newPlayer(AsyncUserConnectionThr, room, &room->players[newclientid]);
				newPlayer.detach();*/
				AsyncUserConnectionThr(room, &room->players[newclientid]);
				return;
			}
		}
	}
	std::cout << "Error while handshaking with player\n";
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

void NetworkEngine::AsyncUserConnectionThr(GameRoom *room, connection *player)
{
	char buff[1024*1024];
	ZeroBuff(&buff[0], sizeof(buff));
	while (serverrunning && player->connected && room->roomActive)
	{
		Sleep(3);
		recv(player->connectSock, buff, sizeof(buff), NULL);
		if (buff[0] == '\x0a')
		{
			DoubleToBytes conv;
			for (int i = 0; i < sizeof(double); i++)
			{
				conv.bytes[i] = buff[i + 1];
			}
			room->GameSpeed = conv.number;
		}
		if (buff[0] == '\x04')
		{
			player->votedForStart = true;
			for (int i = 0; i < 4; i++)
			{
				if (room->players[i].connected && !room->players[i].votedForStart)
				{
					if (send(player->connectSock, "\x05", 1, NULL) <= 0)
					{
						std::cout << "Disconnected user with uuid: " << player->uuid << std::endl;
						player->connected = false;
						closesocket(player->connectSock);
						if (!room->AnyConnectedPlayers()) room->roomActive = false;
					}
					continue;
				}
			}
			if (send(player->connectSock, "\x04", 1, NULL) <= 0)
			{
				std::cout << "Disconnected user with uuid: " << player->uuid << std::endl;
				player->connected = false;
				closesocket(player->connectSock);
				if (!room->AnyConnectedPlayers()) room->roomActive = false;
			}
			std::cout << "Started match in room: " << room->URID << std::endl;
			continue;
		}
		if (buff[0] == '\x07')
		{
			if (room->matchRunning)
			{
				if (buff[2] > 0 && buff[2] < 5)
				{
					room->ChangePlayerDirection(buff[1], player);
				}
				player->GameDataProtocol = (int)buff[1];
				SendPhysicsToClient(room, player);
				continue;
			}
			if(send(player->connectSock, "\x08", 1, NULL) == -1)
			{
				room->matchRunning = false;
				std::cout << "Disconnected user with uuid: " << player->uuid << std::endl;
				player->connected = false;
				closesocket(player->connectSock);
				if (!room->AnyConnectedPlayers()) room->roomActive = false;
			}
			continue;
		}
		if (buff[0] == '\xff')
		{
			std::cout << "Disconnect packet \ xff from user with uuid: " << player->uuid << std::endl;
			player->connected = false;
			closesocket(player->connectSock);
			if (!room->AnyConnectedPlayers()) room->roomActive = false;
			continue;
		}
		player->connected = false;
		closesocket(player->connectSock);
		if(!room->AnyConnectedPlayers()) room->roomActive = false;
		/*player = NULL;*/
		std::cout << "Disconnected some user\n";
	}
}

void NetworkEngine::SendPhysicsToClient(GameRoom *room, connection *client)
{
	int physicssize = room->GetPhysicsForPlayer().size();
	int sendresult = -1;
	switch (client->GameDataProtocol)
	{
	case 0:
	{
		char CompressedPhsycics[36 * 64 + 2];
		CompressedPhsycics[0] = '\x0f';
		CompressedPhsycics[1] = '\x01';
		AABB tempbox;
		for (int i = 0; i < 36; i++)
		{
			for (int j = 0; j < 64; j++)
			{
				tempbox.min.x = j;
				tempbox.min.y = i;
				tempbox.max.x = j + 1;
				tempbox.max.y = i + 1;
				CompressedPhsycics[i * 64 + j + 2] = 0;
				for (int n = 0; n < physicssize; n++)
				{
					if (AABBvsAABB(room->GetPhysicsForPlayer()[n].borders, tempbox) && room->GetPhysicsForPlayer()[n].type != DEAD_SNAKE)
					{
						CompressedPhsycics[i * 64 + j + 2] = room->GetPhysicsForPlayer()[n].type;
						n = physicssize;
					}
				}
			}
		}
		while (client->RecvedPhysics)
		{
			Sleep(1);
		}
		sendresult = send(client->connectSock, CompressedPhsycics, sizeof(CompressedPhsycics), NULL);
		break;
	}
	case 1:
	{
		int snake1Size = room->GetSnakeLenght(0);
		int snake2Size = room->GetSnakeLenght(1);
		int snake3Size = room->GetSnakeLenght(2);
		int snake4Size = room->GetSnakeLenght(3);
		// 20736 bytes-maximum size of 1*1 physical objects on 64*36 field(9 bytes on object), 11520 bytes-maximum size of SnakeBlock pointing to snakes on 64*36 field(5 bytes on SnakeBlock)
		// 2 bytes - packet header
		// 10 bytes - lenght of physics vector, lenghts of 4 SnakeBlocks vector(5 shorts)
		short buffsize = physicssize * sizeof(PhysicalObject) + (snake1Size + snake2Size + snake3Size + snake4Size) * sizeof(SnakeBlock) + 2 + 10;
		ShortToBytes buffsizeConverter;
		buffsizeConverter.integer = buffsize;
		send(client->connectSock, buffsizeConverter.bytes, sizeof(ShortToBytes), NULL);
		char *buff = new char[buffsize];
		//char buff[20736 + 11520 + 2 + 10];
		buff[0] = '\x0f';
		buff[1] = '\x02';
		ShortToBytes lenghtsConverter;
		lenghtsConverter.integer = physicssize;
		buff[2] = lenghtsConverter.bytes[0];
		buff[3] = lenghtsConverter.bytes[1];

		lenghtsConverter.integer = snake1Size;
		buff[4] = lenghtsConverter.bytes[0];
		buff[5] = lenghtsConverter.bytes[1];

		lenghtsConverter.integer = snake2Size;
		buff[6] = lenghtsConverter.bytes[0];
		buff[7] = lenghtsConverter.bytes[1];

		lenghtsConverter.integer = snake3Size;
		buff[8] = lenghtsConverter.bytes[0];
		buff[9] = lenghtsConverter.bytes[1];

		lenghtsConverter.integer = snake4Size;
		buff[10] = lenghtsConverter.bytes[0];
		buff[11] = lenghtsConverter.bytes[1];
		PhysicalObjectToBytes physicalObjectsConv;
		for (int i = 0; i < physicssize; i++)
		{
			physicalObjectsConv.obj = room->GetPhysicsForPlayer()[i];
			for (int j = 0; j < sizeof(PhysicalObject); j++)
			{
				buff[i * sizeof(PhysicalObject) + j + 12] = physicalObjectsConv.bytes[j];
			}
		}
		SnakeBlockToBytes snakeBlocksConv;
		for (int i = 0; i < snake1Size + snake2Size + snake3Size + snake4Size; i++)
		{
			if (i > snake1Size + snake2Size + snake3Size)
			{
				snakeBlocksConv.block = room->GetSnake(3)[i - (snake1Size + snake2Size + snake3Size)];
			}
			else
			{
				if (i > snake1Size + snake2Size)
				{
					snakeBlocksConv.block = room->GetSnake(2)[i - (snake1Size + snake2Size)];
				}
				else
				{
					if (i > snake1Size)
					{
						snakeBlocksConv.block = room->GetSnake(1)[i - snake1Size];
					}
					else
					{
						snakeBlocksConv.block = room->GetSnake(0)[i];
					}
				}
			}
			for (int j = 0; j < sizeof(SnakeBlock); j++)
			{
				buff[physicssize * sizeof(PhysicalObject) + i * sizeof(SnakeBlock) + j + 12] = snakeBlocksConv.bytes[j];
			}
		}
		sendresult = send(client->connectSock, buff, buffsize, NULL);
		break;
	}
	case 2:
	{
		// 2 BYTES-HEADER, 4 BYTES-SNAKESDIRECTIONS, 4 BYTES-2 SHORTS(APPLE AABB.min), 8 BYTES - 4 SHORTS(LENGHTS OF SNAKES)
		char oneTickPacket[2 + 4 + 4 + 8];
		oneTickPacket[0] = '\x0f';
		oneTickPacket[1] = '\x03';
		oneTickPacket[2] = room->players[0].GameDir;
		oneTickPacket[3] = room->players[1].GameDir;
		oneTickPacket[4] = room->players[2].GameDir;
		oneTickPacket[5] = room->players[3].GameDir;
		ShortToBytes converter;
		for (int i = 0; i < room->GetPhysicsForPlayer().size(); i++)
		{
			if (room->GetPhysicsForPlayer()[i].type == APPLE)
			{
				converter.integer = room->GetPhysicsForPlayer()[i].borders.min.x;
				oneTickPacket[6] = converter.bytes[0];
				oneTickPacket[7] = converter.bytes[1];
				converter.integer = room->GetPhysicsForPlayer()[i].borders.min.y;
				oneTickPacket[8] = converter.bytes[0];
				oneTickPacket[9] = converter.bytes[1];
				i = room->GetPhysicsForPlayer().size();
			}
		}
		for (int i = 0; i < 4; i++)
		{
			converter.integer = room->GetSnakeLenght(i);
			oneTickPacket[10 + i * 2] = converter.bytes[0];
			oneTickPacket[11 + i * 2] = converter.bytes[1];
		}
		sendresult = send(client->connectSock, oneTickPacket, sizeof(oneTickPacket), NULL);
		break;
	}
	}
	if (sendresult <= 0)
	{
		std::cout << "Error from user with uuid: " << client->uuid << std::endl;
		client->connected = false;
		closesocket(client->connectSock);
		if (!room->AnyConnectedPlayers()) room->roomActive = false;
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