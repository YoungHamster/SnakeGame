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
	menu->ChangeButtonText(4, 2, const_cast<wchar_t*>(UUIDandURID[0].c_str()));
	menu->ChangeButtonText(4, 3, const_cast<wchar_t*>(UUIDandURID[1].c_str()));
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