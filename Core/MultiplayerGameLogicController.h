#pragma once
#include "Core//GameLogic.h"

#define SIZE_OF_GAME_STATES_BUFFER 8

struct ShortGameTickInfo
{
	int tickID;
	char playersDirs[4];
	int snakesLenghts[4];
	AABB applePosition;
	PhysicalObject snakesHeads[4];
};

struct GameState
{
	ShortGameTickInfo shortInfo;
	GameLogic game;
};

class MultiplayerGameLogicController
{
private:
	int numberOfCurrentStateInBuffer = 0;
	int numberOfPreviousStateInBuffer = SIZE_OF_GAME_STATES_BUFFER - 1;
	GameState gameStatesBuffer[SIZE_OF_GAME_STATES_BUFFER];
public:
	/* Puts new tick on place of last tick in history */
	int GameTick();

	/* When server finds deviation in client game history it sends id of last tick,
	that was equal on server and client, and info about ticks needed to simulate game
	to current time, then client calls this function */
	/* Number of elements in ticksInfo array should be equal to currentTickID-tickID */
	void ResimulateGameFromPastTick(int tickID, ShortGameTickInfo* ticksInfo);

	int ChangePlayersDirections(char playersDirs[4]);
	int GetCurrentTickID();
	ShortGameTickInfo* GetShortTickInfoPointerByTickID(int gameTickID);
	GameState* GetGameStatePointerByTickID(int gameTickID);
};