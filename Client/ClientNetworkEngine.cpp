#include "ClientNetworkEngine.h"

bool ClientNetworkEngine::Connect(const char* ip, unsigned short port, Menu *menu, Renderer *renderer, HWND windowhandle)
{
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
	if (roomsIdsNumber > 0)
	{
		rooms = new int[roomsIdsNumber];
	}
	else
	{
		return false;
	}
	for (int i = 0; i < roomsIdsNumber; i++)
	{
		for (int j = 0; j < sizeof(int); j++)
		{
			roomIdconv.bytes[j] = buff[i * sizeof(int) + j + 9];
		}
		rooms[i] = roomIdconv.integer;
		std::wstring newButtonText(L"Room ");
		newButtonText += std::to_wstring(i);
		newButtonText += L" UUID";
		newButtonText += std::to_wstring(rooms[i]);
		menu->AddButton(1, true, true, 0, 260 + i * 56, const_cast<wchar_t*>(newButtonText.c_str()), 6, renderer, rooms[i]);
	}
	menu->AddButton(1, true, true, 0, 204, const_cast<wchar_t*>(L"NEW ROOM"), 6, renderer, 2147483647);
	bool playerChoosing = true;
	int playerchoose = 2147483647;
	while (playerChoosing)
	{
		POINT p;
		GetCursorPos(&p);
		ScreenToClient(windowhandle, &p);
		int button = menu->CheckMouseCollision(p);
		if (button == playerchoose)
		{
			playerChoosing = false;
		}
		if (button == 7)
		{
			menu->ChangePage(0);
			return false;
		}
		if (button > -2000000000 && button < 2000000000)
		{
			playerchoose = button;
			playerChoosing = false;
		}
		std::vector<PhysicalObject> emptyPhysics;
		renderer->RenderFrame(emptyPhysics, menu->GetButtonsVectorForRenderer(), false);
		Sleep(10);
	}
	/* Somehow give player rooms ids and get input from him */
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
	return true;
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