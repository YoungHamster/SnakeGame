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

#define NETWORK_PROFILING

#define CREATE_NEW_ROOM_ON_SERVER 2147483647

struct ServerInfo
{
	std::wstring serverName;
	int numberOfConnectedPlayers;
	int pingInMs;
	std::vector<std::wstring> roomsNames;
	std::vector<int> activeRoomsIDs;
};

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

union ByteToBool
{
	bool boolean;
	char byte;
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
	NetStates connectionState = DISCONNECTED;
	Console out;
	ServerInfo currentServer;
	short currentRoomID = -1;

	bool SynchronizeGame();
public:
	bool ConnectToServer(std::wstring *nickname, const char* ip, unsigned short port);
	ServerInfo GetInfoAboutServer();
	bool CreateNewRoom();
	bool JoinExistingRoom(short roomID);
	bool LeaveRoom();
	bool VoteForStart();
	int Ping();
	NetStates GameTick(char dir);
	bool StopMatch();
	bool PauseMatch();
	bool Disconnect();
	static void ZeroBuff(char* firstByte, int sizeOfBuff);
	std::vector<PhysicalObject>& GetPhysicsForRenderer();
};