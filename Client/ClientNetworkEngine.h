#pragma once
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <fstream>
#include <WinSock2.h>
#include <Windows.h>
#include <iostream>
#include <thread>
#include <vector>
#pragma comment(lib, "Ws2_32.lib")

#include "Physics.h"
#include "Menu.h"

union BytesToULL
{
	unsigned long long integer;
	char bytes[8];
};

union BytesToInt
{
	int integer;
	char bytes[4];
};

union BytesToWchar
{
	wchar_t wchar;
	char bytes[2];
};

union BytesToPhysicalObject
{
	PhysicalObject obj;
	char bytes[sizeof(PhysicalObject)];
};

class ClientNetworkEngine
{
private:
	unsigned long long UUID;
	wchar_t* nickname = NULL;
	SOCKET sock;
	int* rooms;
	int roomId;
	bool matchRunning = false;;
public:
	bool Connect(const char* ip, unsigned short port, Menu *menu, Renderer *renderer, HWND windowhandle);
	bool VoteForStart();
	std::vector<PhysicalObject>* SendDirGetPhysics(char dir);
	void Disconnect();
	static void ZeroBuff(char* firstByte, int sizeOfBuff);
	//char[(36 + 3) * 64 + 1] SendDirGetCompressedPhysics(char dir);
	char* SendDirGetCompressedPhysics(char dir);
};