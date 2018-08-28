#pragma once
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <fstream>
#include <WinSock2.h>
#include <Windows.h>
#include <iostream>
#include <thread>
#include <vector>
#include <bitset>
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#pragma comment(lib, "Ws2_32.lib")
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include "GameRoom.h"
#include "connection.h"

static bool serverrunning = true;

union PhysicalObjectToBytesConverter
{
	PhysicalObject obj;
	char bytes[sizeof(PhysicalObject)];
};

class NetworkEngine
{
private:
	long succ;
	WSAData wData;
	SOCKADDR_IN address;
	SOCKET listenSock;
	connection connections[1000];
	GameRoom rooms[1000];
public:
	bool Init();
	static void AcceptingThread(SOCKET listenSock, SOCKADDR_IN address, connection *connections, int maxconnnumber, GameRoom *firstRoom);
	static void Handshake(connection client, GameRoom *firstRoom);
	static GameRoom* NewRoom(GameRoom newroom, GameRoom* firstRoom);
	static bool AnyActiveRooms(GameRoom *firstRoom);
	static void AsyncRoomThr(GameRoom *room);
	static void AsyncUserConnectionThr(GameRoom *room, connection *player);
	static void SendPhysicsToClient(connection *client, std::vector<PhysicalObject>& physics);
	static const char* WSAErrorToString();
	static unsigned int randNum();
	static void IntToBytes(char *firstByte, int *integer);
	static void BytesToInt(char *firstByte, int *integer);
	static void ZeroBuff(char *firstByte, int sizeOfBuff);
};