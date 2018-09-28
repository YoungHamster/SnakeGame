#pragma once

#include <fstream>
#include <WinSock2.h>
#include <Windows.h>
#include <iostream>
#include <thread>
#include <vector>
#include "ServerGameLogic.h"
#include "connection.h"

class GameRoom
{
private:
	GameLogic game;
public:
	bool matchRunning = false;
	bool roomActive = false;
	int URID;
	wchar_t* title = const_cast<wchar_t*>(L"New room");

	bool currentlyUsingPDs = false;
	connection players[4];
	double GameSpeed = 1.0;

	void Init(connection roomcreator);
	int ConnectPlayer(connection player);
	void OneTick();
	bool AnyConnectedPlayers();
	std::vector<PhysicalObject>& GetPhysicsForPlayer();
	void ChangePlayerDirection(char dir, connection* player);
	short GetSnakeLenght(int SnakeID);
	std::vector<SnakeBlock>& GetSnake(int SnakeID);
	

	/*void o()
	{
		std::ofstream uid;
		uid.open("uid.uid", std::ios_base::out | std::ios_base::binary);
		uid << userUID;
	}*/
};