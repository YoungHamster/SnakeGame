#pragma once
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <fstream>
#include <WinSock2.h>
#include <Windows.h>
#include <iostream>
#include <thread>
#include <vector>
#pragma comment(lib, "Ws2_32.lib")

#include "GameRoom.h"
#include "connection.h"

static bool serverrunning = true;

union PhysicalObjectToBytes
{
	PhysicalObject obj;
	char bytes[sizeof(PhysicalObject)];
};

union ULLToBytes
{
	unsigned long long integer;
	char bytes[8];
};

union IntToBytes
{
	int integer;
	char bytes[4];
};

union WcharToBytes
{
	wchar_t wchar;
	char bytes[2];
};

class NetworkEngine
{
private:
	long succ;
	WSAData wData;
	SOCKADDR_IN address;
	SOCKET listenSock;
	//connection connections[1000];
	GameRoom rooms[1000];
public:
	bool Init();
	static void AcceptingThread(SOCKET listenSock, SOCKADDR_IN address, int maxconnnumber, GameRoom *firstRoom);
	static void Handshake(connection client, GameRoom *firstRoom);
	static GameRoom* NewRoom(GameRoom newroom, GameRoom* firstRoom);
	static bool AnyActiveRooms(GameRoom *firstRoom);
	static void AsyncRoomThr(GameRoom *room);
	static void AsyncUserConnectionThr(GameRoom *room, connection *player);
	static void SendPhysicsToClient(GameRoom *room, connection *client, std::vector<PhysicalObject>& physics);
	static const char* WSAErrorToString();
	static unsigned int randNum();
	static void ZeroBuff(char *firstByte, int sizeOfBuff);
};