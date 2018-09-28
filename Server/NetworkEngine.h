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
	char bytes[sizeof(PhysicalObject)]; // 9 bytes
};

union ULLToBytes
{
	unsigned long long integer;
	char bytes[sizeof(unsigned long long)]; // 8 bytes
};

union IntToBytes
{
	int integer;
	char bytes[sizeof(int)]; // 4 bytes
};

union WcharToBytes
{
	wchar_t wchar;
	char bytes[sizeof(wchar_t)]; // 2 bytes
};

union ShortToBytes
{
	short integer;
	char bytes[sizeof(short)]; // 2 bytes
};

union DoubleToBytes
{
	double number;
	char bytes[sizeof(double)]; // 8 bytes
};

union SnakeBlockToBytes
{
	SnakeBlock block;
	char bytes[sizeof(SnakeBlock)];
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
	static void SendPhysicsToClient(GameRoom *room, connection *client);
	static const char* WSAErrorToString();
	static unsigned int randNum();
	static void ZeroBuff(char *firstByte, int sizeOfBuff);
};