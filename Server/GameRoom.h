#pragma once

#include <fstream>
#include <WinSock2.h>
#include <Windows.h>
#include <iostream>
#include <thread>
#include <vector>
#include "GameLogic.h"
#include "connection.h"

class GameRoom
{
private:
	bool MatchRunning = false;
	unsigned long long userUID;
	//Client cl[4];
	GameLogic game;
public:
	bool matchRunning = false;
	bool roomActive = false;
	int URID;
	wchar_t* title = const_cast<wchar_t*>(L"New room");

	bool currentlyUsingPDs = false;
	bool votedForStart[4];
	//connection* players[4];
	connection players[4];
	int p1d = 0, p2d = 0, p3d = 0, p4d = 0;// p1td = 0, p2td = 0, p3td = 0, p4td = 0;

	void Init(connection roomcreator);
	int ConnectPlayer(connection player);
	void OneTick();
	bool AnyConnectedPlayers();
	std::vector<PhysicalObject>& GetPhysicsForPlayer();
	void ChangePlayerDirection(char dir, connection* player);

	void o()
	{
		std::ofstream uid;
		uid.open("uid.uid", std::ios_base::out | std::ios_base::binary);
		uid << userUID;
	}
};