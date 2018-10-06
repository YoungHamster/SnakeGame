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
#include "GameLogic.h"
#include "Console.h"

#define CREATE_NEW_ROOM_ON_SERVER 2147483647

enum NetStates
{
	SOMEERROR,
	DISCONNECTED,
	HANDSHAKING,
	WAITING_MATCH_START,
	MATCH_OK,
	MATCH_DESYNC
};

enum NetEngineInput
{
	VOTE_FOR_START,
	TURN_UP,
	TURN_DOWN,
	TURN_LEFT,
	TURN_RIGHT,
	DISCONNECT
};

union BytesToULL
{
	unsigned long long integer;
	char bytes[sizeof(unsigned long long)]; // 8 bytes
};

union BytesToInt
{
	int integer;
	char bytes[sizeof(int)]; // 4 bytes
};

union BytesToWchar
{
	wchar_t wchar;
	char bytes[sizeof(wchar_t)]; // 2 bytes
};

union BytesToPhysicalObject
{
	PhysicalObject obj;
	char bytes[sizeof(PhysicalObject)]; // 9 bytes
};

union BytesToShort
{
	short integer;
	char bytes[sizeof(short)]; // 2 bytes
};

union BytesToSnakeBlock
{
	SnakeBlock block;
	char bytes[sizeof(SnakeBlock)];
};

class ClientNetworkEngine
{
private:
	unsigned long long UUID;
	wchar_t* nickname = NULL;
	SOCKET sock;
	int* rooms;
	int roomId;
	bool matchRunning = false;
	GameLogic game;
	NetStates ConnectionState = DISCONNECTED;
	Console out;

	bool SynchronizeGame();
	NetStates GameTick(NetEngineInput input);
public:
	int* ConnectPart1(const char* ip, unsigned short port, int* numberOfRooms);
	bool ConnectPart2(int playerChoose);
	bool VoteForStart();
	std::vector<PhysicalObject>* SendDirGetPhysics(char dir);
	void Disconnect();
	static void ZeroBuff(char* firstByte, int sizeOfBuff);
	//char[(36 + 3) * 64 + 1] SendDirGetCompressedPhysics(char dir);
	char* SendDirGetCompressedPhysics(char dir);
	NetStates NetworkTick(NetEngineInput input);
	std::vector<PhysicalObject>& GetPhysicsForRenderer();
};