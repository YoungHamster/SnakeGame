#include "GameRoom.h"

void GameRoom::Init(connection* roomCreator)
{
	roomActive = true;
	URID = randomNumber(-2000000000, 2000000000);
	players[0] = roomCreator;
	title = roomCreator->nickname;
	game.Init(64, 36, 5, 5, 5, 5);
}

void GameRoom::ConnectPlayer(connection* player)
{
	for (int i = 0; i < 4; i++)
	{
		if (!players[i]->connected)
		{
			players[i] = player;
			return;
		}
	}
}

bool GameRoom::AnyConnectedPlayers()
{
	for (int i = 0; i < 4; i++)
	{
		if (players[i] != NULL)
		{
			if (players[i]->connected) return true;
		}
	}
	return false;
}