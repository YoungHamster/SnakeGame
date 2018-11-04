#include "MultiplayerGameLogicController.h"

int MultiplayerGameLogicController::GameTick()
{
	numberOfPreviousStateInBuffer = numberOfCurrentStateInBuffer;
	if (numberOfCurrentStateInBuffer >= SIZE_OF_GAME_STATES_BUFFER)
	{
		numberOfCurrentStateInBuffer = 0;
	}
	else
	{
		++numberOfCurrentStateInBuffer;
	}

	gameStatesBuffer[numberOfCurrentStateInBuffer] = gameStatesBuffer[numberOfPreviousStateInBuffer];

	++gameStatesBuffer[numberOfCurrentStateInBuffer].shortInfo.tickID;

	/* This array should make code much more readable */
	char playersDirs[4];
	playersDirs[0] = gameStatesBuffer[numberOfCurrentStateInBuffer].shortInfo.playersDirs[0];
	playersDirs[1] = gameStatesBuffer[numberOfCurrentStateInBuffer].shortInfo.playersDirs[0];
	playersDirs[2] = gameStatesBuffer[numberOfCurrentStateInBuffer].shortInfo.playersDirs[0];
	playersDirs[3] = gameStatesBuffer[numberOfCurrentStateInBuffer].shortInfo.playersDirs[0];
	gameStatesBuffer[numberOfCurrentStateInBuffer].game.OneTick(playersDirs[0], playersDirs[1], playersDirs[2], playersDirs[3]);

	/* Put players directions to short info structure */
	gameStatesBuffer[numberOfCurrentStateInBuffer].shortInfo.playersDirs[0] = gameStatesBuffer[numberOfCurrentStateInBuffer].game.playerDirs[0];
	gameStatesBuffer[numberOfCurrentStateInBuffer].shortInfo.playersDirs[1] = gameStatesBuffer[numberOfCurrentStateInBuffer].game.playerDirs[1];
	gameStatesBuffer[numberOfCurrentStateInBuffer].shortInfo.playersDirs[2] = gameStatesBuffer[numberOfCurrentStateInBuffer].game.playerDirs[2];
	gameStatesBuffer[numberOfCurrentStateInBuffer].shortInfo.playersDirs[3] = gameStatesBuffer[numberOfCurrentStateInBuffer].game.playerDirs[3];

	/* Put snakes lenghts to short info structure */
	gameStatesBuffer[numberOfCurrentStateInBuffer].shortInfo.snakesLenghts[0] = gameStatesBuffer[numberOfCurrentStateInBuffer].game.snakes[0].size();
	gameStatesBuffer[numberOfCurrentStateInBuffer].shortInfo.snakesLenghts[1] = gameStatesBuffer[numberOfCurrentStateInBuffer].game.snakes[1].size();
	gameStatesBuffer[numberOfCurrentStateInBuffer].shortInfo.snakesLenghts[2] = gameStatesBuffer[numberOfCurrentStateInBuffer].game.snakes[2].size();
	gameStatesBuffer[numberOfCurrentStateInBuffer].shortInfo.snakesLenghts[3] = gameStatesBuffer[numberOfCurrentStateInBuffer].game.snakes[3].size();

	/* If you are reding this, sorry for long string, there i write physical object of 4 snakes heads to short info from physics buffer */
	gameStatesBuffer[numberOfCurrentStateInBuffer].shortInfo.snakesHeads[0] = gameStatesBuffer[numberOfCurrentStateInBuffer].game.physics[gameStatesBuffer[numberOfCurrentStateInBuffer].game.snakes[0][0].PhysicalBodyID];
	gameStatesBuffer[numberOfCurrentStateInBuffer].shortInfo.snakesHeads[0] = gameStatesBuffer[numberOfCurrentStateInBuffer].game.physics[gameStatesBuffer[numberOfCurrentStateInBuffer].game.snakes[0][0].PhysicalBodyID];
	gameStatesBuffer[numberOfCurrentStateInBuffer].shortInfo.snakesHeads[0] = gameStatesBuffer[numberOfCurrentStateInBuffer].game.physics[gameStatesBuffer[numberOfCurrentStateInBuffer].game.snakes[0][0].PhysicalBodyID];
	gameStatesBuffer[numberOfCurrentStateInBuffer].shortInfo.snakesHeads[0] = gameStatesBuffer[numberOfCurrentStateInBuffer].game.physics[gameStatesBuffer[numberOfCurrentStateInBuffer].game.snakes[0][0].PhysicalBodyID];

	return gameStatesBuffer[numberOfCurrentStateInBuffer].shortInfo.tickID;
}

void MultiplayerGameLogicController::ResimulateGameFromPastTick(int tickID, ShortGameTickInfo* ticksInfo)
{
	/* May be i'll use this implementation */
	/*for (int i = 0; i < SIZE_OF_GAME_STATES_BUFFER; i++)
	{
	if (gameStatesBuffer[i].shortInfo.tickID == tickID)
	{
	numberOfCurrentStateInBuffer = i;
	i = SIZE_OF_GAME_STATES_BUFFER;
	}
	}
	for (int i = 0; i < GetCurrentTickID() - tickID; i++, ticksInfo++)
	{
	ChangePlayesrDirections(ticksInfo->playersDirs);
	GameTick();
	}*/

	/* Now i'm going to use this implementation */
	GameState* gameState;
	for (int i = 0; i < GetCurrentTickID() - tickID; i++)
	{
		gameState = GetGameStatePointerByTickID(tickID + 1 + i); // tickid + 1 is first tick with short info that is not equal to server short info
		gameState->shortInfo = ticksInfo[i];
		gameState->game.ChangeApplePostition(ticksInfo[i].applePosition.min.x, ticksInfo[i].applePosition.min.y);
		gameState->game.OneTick(ticksInfo->playersDirs[0], ticksInfo->playersDirs[1], ticksInfo->playersDirs[2], ticksInfo->playersDirs[3]);
	}
}

/* Server sends 4 directions and id of last tick,
if directions are different, then client searches for
last tick, when directions were same and calls this functions */
int MultiplayerGameLogicController::ChangePlayersDirections(char playersDirs[4])
{
	gameStatesBuffer[numberOfCurrentStateInBuffer].game.playerDirs[0] = playersDirs[0];
	gameStatesBuffer[numberOfCurrentStateInBuffer].game.playerDirs[1] = playersDirs[1];
	gameStatesBuffer[numberOfCurrentStateInBuffer].game.playerDirs[2] = playersDirs[2];
	gameStatesBuffer[numberOfCurrentStateInBuffer].game.playerDirs[3] = playersDirs[3];
	return 0;
}

int MultiplayerGameLogicController::GetCurrentTickID()
{
	return gameStatesBuffer[numberOfCurrentStateInBuffer].shortInfo.tickID;
}

ShortGameTickInfo* MultiplayerGameLogicController::GetShortTickInfoPointerByTickID(int gameTickID)
{
	for (int i = 0; i < SIZE_OF_GAME_STATES_BUFFER; i++)
	{
		if (gameStatesBuffer[i].shortInfo.tickID == gameTickID)
		{
			return &gameStatesBuffer[i].shortInfo;
		}
	}
	return NULL;
}

GameState* MultiplayerGameLogicController::GetGameStatePointerByTickID(int gameTickID)
{
	for (int i = 0; i < SIZE_OF_GAME_STATES_BUFFER; i++)
	{
		if (gameStatesBuffer[i].shortInfo.tickID == gameTickID)
		{
			return &gameStatesBuffer[i];
		}
	}
	return NULL;
}