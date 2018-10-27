#include "GameRoom.h"

void GameRoom::Init(connection *roomCreator)
{
	players[0] = roomCreator;
	roomActive = true;
	title = players[0]->nickname;
	game.Init(64, 36, 5, 5, 5, 5);
}

int GameRoom::ConnectPlayer(connection *player)
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
		if (players[i] == NULL)
		{
			players[i] = player;
			return i;
		}
	}
	return -1;
}

void GameRoom::OneTick()
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
	if (players[0] == NULL && players[1] == NULL && players[2] == NULL && players[3] == NULL)
	{
		roomActive = false;
		return;
	}
	if ((players[0] == NULL ? true : false) && (players[1] == NULL ? true : false) &&
		(players[2] == NULL ? true : false) && (players[3] == NULL ? true : false))
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
			players[i]->recvedLastTickInfo = false;
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

int GameRoom::DisconnectPlayer(connection *player)
{
	for (int i = 0; i < 4; i++)
	{
		if (players[i] == player)
		{
			players[i] = NULL;
		}
	}
	return 0;
}

char GameRoom::GetPlayerDir(int playerID)
{
	return game.playerDirs[playerID];
}