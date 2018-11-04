#pragma once
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <fstream>
#include <WinSock2.h>
#include <Windows.h>
#include <iostream>
#include <thread>
#include <vector>
#pragma comment(lib, "Ws2_32.lib")

#include "Core//Physics.h"
#include "Core//GameLogic.h"
#include "Core//MultiplayerGameLogicController.h"
#include "Console.h"

#define NETWORK_PROFILING

#define CREATE_NEW_ROOM_ON_SERVER 2147483647

struct ServerRoomInfo
{
	float gameSpeed;
	int ID;
	std::wstring roomName;
	char numberOfPlayers;
	std::wstring nicknamesOfPlayers[4];
};

struct ServerInfo
{
	std::wstring serverName;
	int numberOfConnectedPlayers;
	int pingInMs;
	std::vector<int> activeRoomsIDs;
	std::vector<ServerRoomInfo> activeRooms;
};

union BytesToShortGameTickInfo
{
	ShortGameTickInfo info;
	char bytes[sizeof(ShortGameTickInfo)];
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

union BytesToFloat
{
	float number;
	char bytes[sizeof(float)];
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
	Console out;
	ServerInfo currentServer;
	short currentRoomID = -1;

	MultiplayerGameLogicController gameController;

	bool SynchronizeGame();
public:
	bool ConnectToServer(std::wstring *nickname, const char* ip, unsigned short port);
	ServerInfo* GetInfoAboutServer();
	bool CreateNewRoom();
	bool JoinExistingRoom(short roomID);
	ServerInfo* LeaveRoom();
	bool VoteForStart();
	int Ping();
	int GameTick(char dir);
	bool StopMatch();
	bool PauseMatch();
	bool UnpauseMatch();
	bool Disconnect();
	bool ChangeRoomName(std::wstring* newRoomName);
	static void ZeroBuff(char* firstByte, int sizeOfBuff);
	std::vector<PhysicalObject>& GetPhysicsForRenderer();
};