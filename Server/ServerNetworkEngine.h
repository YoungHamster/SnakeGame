#pragma once
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <fstream>
#include <WinSock2.h>
#include <Windows.h>
#include <iostream>
#include <thread>
#include <vector>
#include <chrono>
#pragma comment(lib, "Ws2_32.lib")

#include "GameRoom.h"
#include "connection.h"

#define NUMBER_OF_ROOMS 1000

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

static GameRoom rooms[NUMBER_OF_ROOMS];
static connection players[4000];

/* static array of pointers on functions for network final state machine */
static int(*functions[12])(char* buff, int buffSize, GameRoom *room, connection *player);

class NetworkEngine
{
private:
	long succ;
	WSAData wData;
	SOCKADDR_IN address;
	SOCKET listenSock;
public:
	std::wstring serverName = L"SnakeGame server";

	bool Init();
	static void AcceptingThread(SOCKET listenSock, SOCKADDR_IN address, connection* firstConnection, GameRoom *firstRoom);
	static int Handshake(GameRoom *firstRoom, connection *player);
	static GameRoom* NewRoom(GameRoom newroom, GameRoom* firstRoom);
	static bool AnyActiveRooms(GameRoom *firstRoom);
	static void AsyncRoomThr(GameRoom *room);
	static void AsyncUserConnection(GameRoom *firstRoom, connection *player);
	static void SendPhysicsToClient(GameRoom *room, connection *client);
	static const char* WSAErrorToString();
	//static unsigned int randNum();
	static void ZeroBuff(char *firstByte, int sizeOfBuff);
};