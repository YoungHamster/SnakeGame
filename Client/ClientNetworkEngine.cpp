#include "ClientNetworkEngine.h"

bool ClientNetworkEngine::Connect(const char* ip, unsigned short port, Menu *menu, Renderer *renderer, HWND windowhandle)
{
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
		std::cout << "connect() error" << std::endl;
		return false;
	}
	char buff[4009];
	buff[0] = '\x01';
	buff[1] = '\x00';
	buff[2] = '\x43';
	send(sock, buff, 3, NULL);

	BytesToULL uuidconv;
	BytesToInt roomIdconv;
	int roomsIdsNumber = (recv(sock, buff, sizeof(buff), NULL) - 9) / 4;
	for (int i = 0; i < 8; i++)
	{
		uuidconv.bytes[i] = buff[i + 1];
	}
	UUID = uuidconv.integer;
	if (rooms)
	{
		delete[] rooms;
	}
	if (roomsIdsNumber >= 0)
	{
		rooms = new int[roomsIdsNumber];
	}
	else
	{
		return false;
	}
	std::wstring* roomsTexts = new std::wstring[roomsIdsNumber];
	for (int i = 0; i < roomsIdsNumber; i++)
	{
		for (int j = 0; j < sizeof(int); j++)
		{
			roomIdconv.bytes[j] = buff[i * sizeof(int) + j + 9];
		}
		rooms[i] = roomIdconv.integer;
		//std::wstring newButtonText(L"Room ");
		/*newButtonText += std::to_wstring(rooms[i]);
		newButtonText += L" UUID";
		newButtonText += std::to_wstring(rooms[i]);
		menu->AddButton(1, true, true, 0, 260 + i * 56, const_cast<wchar_t*>(newButtonText.c_str()), 6, renderer, rooms[i]);*/
		roomsTexts[i] = L"ROOM ";
		roomsTexts[i] += std::to_wstring(rooms[i]);
		menu->AddButton(7, true, true, 0, 120 + i * 56, const_cast<wchar_t*>(roomsTexts[i].c_str()), 6, renderer, rooms[i]);
	}
	std::vector<PhysicalObject> emptyPhysics;
	renderer->RenderFrame(emptyPhysics, menu->GetButtonsVectorForRenderer(), false);
	//Sleep(1000);
	bool playerChoosing = true;
	int playerchoose = 2147483647;
	while (playerChoosing)
	{
		POINT p;
		GetCursorPos(&p);
		ScreenToClient(windowhandle, &p);
		int button = menu->CheckMouseCollision(p);
		if (GetAsyncKeyState(VK_LBUTTON) < 0)
		{
			if (button == playerchoose)
			{
				playerChoosing = false;
			}
			if (button == 7)
			{
				menu->ChangePage(6);
				Sleep(200);
				return false;
			}
			if (button > -2000000000 && button < 2000000000 && button > 0 && button > 18)
			{
				playerchoose = button;
				playerChoosing = false;
			}
		}
		std::vector<PhysicalObject> emptyPhysics;
		renderer->RenderFrame(emptyPhysics, menu->GetButtonsVectorForRenderer(), false);
		Sleep(10);
	}
	buff[0] = 0x02;
	if (playerchoose != 2147483647)
	{
		buff[1] = 0x02;
		roomIdconv.integer = playerchoose;
		for (int i = 0; i < 4; i++)
		{
			buff[i + 2] = roomIdconv.bytes[i];
		}
		send(sock, buff, 6, NULL);
	}
	else
	{
		buff[1] = 0x01;
		send(sock, buff, 2, NULL);
	}
	recv(sock, buff, sizeof(buff), NULL);
	for (int i = 0; i < 4; i++)
	{
		roomIdconv.bytes[i] = buff[i + 1];
	}
	roomId = roomIdconv.integer;
	menu->ChangePage(4);
	std::wstring* UUIDandURID = new std::wstring[2];
	UUIDandURID[0] = const_cast<wchar_t*>(L"UUID - ");
	UUIDandURID[1] = const_cast<wchar_t*>(L"URID - ");
	UUIDandURID[1] += std::to_wstring(roomId);
	UUIDandURID[0] += std::to_wstring(UUID);
	menu->ChangeButtonText(4, 1, const_cast<wchar_t*>(UUIDandURID[0].c_str()));
	menu->ChangeButtonText(4, 2, const_cast<wchar_t*>(UUIDandURID[1].c_str()));
	return true;
}

int* ClientNetworkEngine::ConnectPart1(const char* ip, unsigned short port, Menu *menu, Renderer *renderer, HWND windowhandle, int* numberOfRooms)
{
	ConnectionState = HANDSHAKING;
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
		out.Write("connect() error\n");
		std::cout << "connect() error" << std::endl;
		ConnectionState = DISCONNECTED;
		return false;
	}
	char buff[4009];
	buff[0] = '\x01';
	buff[1] = '\x00';
	buff[2] = '\x43';
	send(sock, buff, 3, NULL);

	BytesToULL uuidconv;
	BytesToInt roomIdconv;
	int recvRes = recv(sock, buff, sizeof(buff), NULL);

	out.Write("Received 1st packet from server! " + std::to_string(recvRes) + " bytes.");
	std::cout << "Received 1st packet from server! " << recvRes << " bytes." << std::endl;

	int roomsNumber = (recvRes - 9) / 4;
	for (int i = 0; i < 8; i++)
	{
		uuidconv.bytes[i] = buff[i + 1];
	}
	UUID = uuidconv.integer;

	out.Write(std::to_string(UUID) + " - UUID");

	if (rooms)
	{
		delete[] rooms;
	}
	if (roomsNumber > 0)
	{
		rooms = new int[roomsNumber];
		*numberOfRooms = roomsNumber;
	}
	else
	{
		return NULL;
	}
	for (int i = 0; i < roomsNumber; i++)
	{
		for (int j = 0; j < sizeof(int); j++)
		{
			roomIdconv.bytes[j] = buff[i * sizeof(int) + j + 9];
		}
		rooms[i] = roomIdconv.integer;
	}
	return rooms;
}

bool ClientNetworkEngine::ConnectPart2(int playerChoose)
{
	char buff[100];
	buff[0] = '\x02';

	BytesToInt roomIDConverter;
	if (playerChoose != 2147483647)
	{
		buff[1] = '\x02';
		roomIDConverter.integer = playerChoose;
		for (int i = 0; i < 4; i++)
		{
			buff[i + 2] = roomIDConverter.bytes[i];
		}
		send(sock, buff, 6, NULL);
	}
	else
	{
		buff[1] = 0x01;
		send(sock, buff, 2, NULL);
	}
	recv(sock, buff, sizeof(buff), NULL);
	for (int i = 0; i < 4; i++)
	{
		roomIDConverter.bytes[i] = buff[i + 1];
	}
	roomId = roomIDConverter.integer;
	ConnectionState = WAITING_MATCH_START;
	/*menu->ChangePage(4);
	std::wstring* UUIDandURID = new std::wstring[2];
	UUIDandURID[0] = const_cast<wchar_t*>(L"UUID - ");
	UUIDandURID[1] = const_cast<wchar_t*>(L"URID - ");
	UUIDandURID[1] += std::to_wstring(roomId);
	UUIDandURID[0] += std::to_wstring(UUID);
	menu->ChangeButtonText(4, 1, const_cast<wchar_t*>(UUIDandURID[0].c_str()));
	menu->ChangeButtonText(4, 2, const_cast<wchar_t*>(UUIDandURID[1].c_str()));*/
	return true;
}

NetStates ClientNetworkEngine::NetworkTick(NetEngineInput input)
{
	if (ConnectionState == MATCH_DESYNC)
	{
		if (SynchronizeGame())
		{
			ConnectionState = MATCH_OK;
		}
		else
		{
			return ConnectionState;
		}
	}
	if (input == TURN_UP || input == TURN_DOWN || input == TURN_LEFT || input == TURN_RIGHT)\
	{
		return GameTick(input);
	}
	switch (input)
	{
	case VOTE_FOR_START:
		if (ConnectionState == WAITING_MATCH_START)
		{
			if (VoteForStart())
			{
				if (SynchronizeGame())
				{
					ConnectionState = MATCH_OK;
				}
				else
				{
					ConnectionState = MATCH_DESYNC;
				}
			}
		}
		break;
	case DISCONNECT:
		closesocket(sock);
		ConnectionState = DISCONNECTED;
		break;
	}

	return ConnectionState;
}

NetStates ClientNetworkEngine::GameTick(NetEngineInput input)
{
	if (input != TURN_UP && input != TURN_DOWN && input != TURN_LEFT && input != TURN_RIGHT)
	{
		ConnectionState = SOMEERROR;
		out.Write("WRONG PARAMETERS TO GameTick() FUNCTION");
		return ConnectionState;
	}
	char dir = (char)input;
	char buff[3];
	buff[0] = '\x07';
	buff[1] = 2;
	buff[2] = dir;
	send(sock, buff, 3, NULL);
	// 2 BYTES-HEADER, 4 BYTES-SNAKESDIRECTIONS, 4 BYTES-2 SHORTS(APPLE AABB.min), 8 BYTES - 4 SHORTS(LENGHTS OF SNAKES)
	char oneTickPacket[2 + 4 + 4 + 8];
	recv(sock, oneTickPacket, sizeof(oneTickPacket), NULL);
	if (oneTickPacket[0] != '\x0f' || oneTickPacket[1] != '\x03')
	{
		ConnectionState = SOMEERROR;
		out.Write("SERVER SENDED WRONG PACKET!!!");
		return ConnectionState;
	}
	PhysicalObject apple;
	apple.type = APPLE;
	BytesToShort shortsConv;
	short shorts[2 + 4];
	for (int i = 0; i < 6; i++)
	{
		shortsConv.bytes[0] = oneTickPacket[6 + i * sizeof(short)];
		shortsConv.bytes[0] = oneTickPacket[6 + i * sizeof(short) + 1];
		shorts[i] = shortsConv.integer;
	}

	game.OneTick(oneTickPacket[2], oneTickPacket[3], oneTickPacket[4], oneTickPacket[5]);

	apple.borders.min.x = shorts[0];
	apple.borders.min.y = shorts[1];
	apple.borders.max.x = apple.borders.min.x + 1;
	apple.borders.max.y = apple.borders.min.y + 1;
	for (int i = 0; i < game.physics.size(); i++)
	{
		if (game.physics[i].type == APPLE)
		{
			game.physics[i] = apple;
			i = game.physics.size();
		}
	}
	if (game.snakes[0].size() != shorts[2] || game.snakes[1].size() != shorts[3] || game.snakes[2].size() != shorts[4] || game.snakes[3].size() != shorts[5])
	{
		ConnectionState = MATCH_DESYNC;
	}
	return ConnectionState;
}

/**/
/* */
bool ClientNetworkEngine::SynchronizeGame()
{
	char enterbuff[3];
	enterbuff[0] = '\x07';
	enterbuff[1] = 1;
	enterbuff[2] = 0;
	send(sock, enterbuff, 3, NULL);

	char buffsizebytes[2];
	recv(sock, buffsizebytes, 2, NULL);

	BytesToShort buffsizeConverter;
	buffsizeConverter.bytes[0] = buffsizebytes[0];
	buffsizeConverter.bytes[1] = buffsizebytes[1];
	short buffsize = buffsizeConverter.integer;

	char *buff = new char[buffsize];
	recv(sock, buff, buffsize, NULL);

	short physicssize;
	short snake1Size;
	short snake2Size;
	short snake3Size;
	short snake4Size;
	BytesToShort lenghtsConverter;
	lenghtsConverter.bytes[0] = buff[2];
	lenghtsConverter.bytes[1] = buff[3];
	physicssize = lenghtsConverter.integer;
	
	lenghtsConverter.bytes[0] = buff[4];
	lenghtsConverter.bytes[1] = buff[5];
	snake1Size = lenghtsConverter.integer;

	lenghtsConverter.bytes[0] = buff[6];
	lenghtsConverter.bytes[1] = buff[7];
	snake2Size = lenghtsConverter.integer;

	lenghtsConverter.bytes[0] = buff[8];
	lenghtsConverter.bytes[1] = buff[9];
	snake3Size = lenghtsConverter.integer;

	lenghtsConverter.bytes[0] = buff[10];
	lenghtsConverter.bytes[1] = buff[11];
	snake4Size = lenghtsConverter.integer;
	if (buff[0] != '\x0f' && buff[1] != '\x02')
	{
		ConnectionState = SOMEERROR;
		return false;
	}
	BytesToPhysicalObject physObjConv;
	game.physics.clear();
	game.snakes[0].clear();
	game.snakes[1].clear();
	game.snakes[2].clear();
	game.snakes[3].clear();
	for (int i = 0; i < physicssize; i++)
	{
		for (int j = 0; j < sizeof(PhysicalObject); j++)
		{
			physObjConv.bytes[j] = buff[2 + i * sizeof(PhysicalObject) + j];
		}
		game.physics.push_back(physObjConv.obj);
	}

	/* Pretty stupid way to convert bytes to snake blocks, but I can't come up with smth better :) */
	BytesToSnakeBlock snakeBlocksConverter;
	for (int k = 0; k < 4; k++)
	{
		int NumberOfConvertedSnakeBlocks = 0;
		int SizeOfSnakeToBeConverted = snake1Size;
		if (k > 2)
		{
			NumberOfConvertedSnakeBlocks = snake1Size + snake2Size + snake3Size;
			SizeOfSnakeToBeConverted = snake4Size;
		}
		else
		{
			if (k > 1)
			{
				NumberOfConvertedSnakeBlocks = snake1Size + snake2Size;
				SizeOfSnakeToBeConverted = snake3Size;
			}
			else
			{
				if (k > 0)
				{
					NumberOfConvertedSnakeBlocks = snake1Size;
					SizeOfSnakeToBeConverted = snake2Size;
				}
			}
		}
		for (int i = NumberOfConvertedSnakeBlocks; i < NumberOfConvertedSnakeBlocks + SizeOfSnakeToBeConverted; i++)
		{
			for (int j = 0; j < sizeof(SnakeBlock); j++)
			{
				snakeBlocksConverter.bytes[j] = buff[2 + physicssize * sizeof(PhysicalObject) + i * sizeof(SnakeBlock) + j + NumberOfConvertedSnakeBlocks * sizeof(SnakeBlock)];
			}
			game.snakes[k].push_back(snakeBlocksConverter.block);
		}
	}

}

bool ClientNetworkEngine::VoteForStart()
{
	char buff[100];
	buff[0] = 0x04;
	send(sock, buff, 1, NULL);
	recv(sock, buff, sizeof(buff), NULL);
	if (buff[0] == 0x04)
	{
		return true;
	}
	if (buff[0] == 0x05)
	{
		return false;
	}
	return false;
}

std::vector<PhysicalObject>* ClientNetworkEngine::SendDirGetPhysics(char dir)
{
	std::vector<PhysicalObject> *physics = new std::vector<PhysicalObject>;
	char buff[2304 * sizeof(PhysicalObject)];
	buff[0] = 0x07;
	if (dir >= 0 && dir <= 4)
	{
		buff[1] = dir;
	}
	else
	{
		buff[1] = '\0';
	}
	send(sock, buff, 2, NULL);
	int physicsSize = (recv(sock, buff, sizeof(buff), NULL) - 1) / 4;
	if (buff[0] == 0x08)
	{
		matchRunning = false;
		return NULL;
	}
	BytesToPhysicalObject physicsConv;
	if (buff[0] == 0x07)
	{
		for (int i = 0; i < physicsSize; i++)
		{
			for (int j = 0; j < sizeof(PhysicalObject); j++)
			{
				physicsConv.bytes[j] = buff[i * sizeof(PhysicalObject) + j + 1];
			}
			physics->push_back(physicsConv.obj);
		}
		return physics;
	}
	return NULL;
}

char* ClientNetworkEngine::SendDirGetCompressedPhysics(char dir)
{
	char* lowCompressedPhysics = new char[39 * 66 + 1];
	lowCompressedPhysics[0] = '\x07';
	if (dir >= 0 && dir <= 4)
	{
		lowCompressedPhysics[1] = dir;
	}
	else
	{
		lowCompressedPhysics[1] = '\0';
	}
	send(sock, lowCompressedPhysics, 2, NULL);
	recv(sock, lowCompressedPhysics, 39 * 66 + 1, NULL);
	return lowCompressedPhysics;
}

std::vector<PhysicalObject>& ClientNetworkEngine::GetPhysicsForRenderer()
{
	return game.physics;
}

void ClientNetworkEngine::Disconnect()
{
	send(sock, "\xff", 1, NULL);
	closesocket(sock);
}

void ClientNetworkEngine::ZeroBuff(char* firstByte, int sizeOfBuff)
{
	for (int i = 0; i < sizeOfBuff; i++, firstByte++)
	{
		*firstByte = 0;
	}
}