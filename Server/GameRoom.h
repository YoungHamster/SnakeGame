#pragma once

#include <fstream>
#include <WinSock2.h>
#include <Windows.h>
#include <iostream>
#include <thread>
#include <vector>
#include "Core//GameLogic.h"
#include "connection.h"

#define SIZE_OF_GAME_STATES_HISTORY 8

class GameRoom
{
private:
	GameLogic game;
public:
	bool matchRunning = false;
	bool roomActive = false;
	std::wstring title = L"GAME ROOM";
	connection* players[4];
	float GameSpeed = 1.0;

	MultiplayerGameLogicController gameController[SIZE_OF_GAME_STATES_HISTORY];

	void Init(connection *roomcreator);
	int ConnectPlayer(connection *player);
	void OneTick();
	bool AnyConnectedPlayers();
	std::vector<PhysicalObject>& GetPhysicsForPlayer();
	void ChangePlayerDirection(char dir, connection* player);
	short GetSnakeLenght(int SnakeID);
	std::vector<SnakeBlock>& GetSnake(int SnakeID);
	char GetPlayerDir(int playerID);
	int DisconnectPlayer(connection *player);
	int ReloadGame() { game.Init(64, 36, 5, 5, 5, 5); return 0; }

	char GetNumberOfPlayers();
};