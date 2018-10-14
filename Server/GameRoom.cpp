#include "GameRoom.h"

void GameRoom::Init(connection *roomCreator)
{
	players[0] = roomCreator;
	roomActive = true;
	URID = randomNumber(-2000000000, 2000000000);
	title = players[0]->nickname;
	game.Init(64, 36, 5, 5, 5, 5);
}

int GameRoom::ConnectPlayer(connection *player)
{
	for (int i = 0; i < 4; i++)
	{
		if (players[i] == NULL ? true : !players[i]->connected)
		{
			players[i] = player;
			return i;
		}
	}
	return -1;
}

void GameRoom::OneTick()
{
	matchRunning = true;
	for (int i = 0; i < 4; i++)
	{
		if (players[i] != NULL)
		{
			if (!players[i]->connected)
			{
				players[i] = NULL;
			}
		}
	}
	if (players[0] == NULL && players[1] == NULL && players[2] == NULL && players[3] == NULL)
	{
		roomActive = false;
		return;
	}
	if ((players[0] == NULL ? true : players[0]->connected) && (players[1] == NULL ? true : players[1]->connected) &&
		(players[2] == NULL ? true : players[2]->connected) && (players[3] == NULL ? true : players[3]->connected))
	{
		roomActive = false;
		return;
	}
	game.OneTick((players[0] != NULL ? players[0]->gameDir : 0),
		(players[1] != NULL ? players[1]->gameDir : 0),
		(players[2] != NULL ? players[2]->gameDir : 0),
		(players[3] != NULL ? players[3]->gameDir : 0), &currentlyUsingPDs);
	for (int i = 0; i < 4; i++)
	{
		if (players[i] != NULL)
		{
			players[i]->recvedPhysics = false;
		}
	}
}

bool GameRoom::AnyConnectedPlayers()
{
	for (int i = 0; i < 4; i++)
	{
		if (players[i] != NULL)
		{
			if (!players[i]->connected)
			{
				players[i] = NULL;
			}
		}
	}
	for (int i = 0; i < 4; i++)
	{
		if (players[i] != NULL) return true;
	}
	return false;
}

std::vector<PhysicalObject>& GameRoom::GetPhysicsForPlayer()
{
	return game.physics;
}

short GameRoom::GetSnakeLenght(int SnakeID)
{
	return game.snakes[SnakeID].size();
}

std::vector<SnakeBlock>& GameRoom::GetSnake(int SnakeID)
{
	return game.snakes[SnakeID];
}

void GameRoom::ChangePlayerDirection(char dir, connection* player)
{
	while (currentlyUsingPDs)
	{
		Sleep(1);
	}
	player->gameDir = (int)dir;
}

char GameRoom::GetPlayerDir(int playerID)
{
	if (players[playerID] != NULL)
	{
		return players[playerID]->gameDir;
	}
}

int GameRoom::DisconnectPlayer(connection *player)
{
	for (int i = 0; i < 4; i++)
	{
		if (players[i] == player)
		{
			players[i] = NULL;
		}
	}
}