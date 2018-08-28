#include "GameRoom.h"

void GameRoom::Init(connection* roomCreator)
{
	players[0] = *roomCreator;
	roomActive = true;
	URID = randomNumber(-2000000000, 2000000000);
	title = players[0].nickname;
	game.Init(64, 36, 5, 5, 5, 5);
}

void GameRoom::ConnectPlayer(connection* player)
{
	for (int i = 0; i < 4; i++)
	{
		if (!players[i].connected)
		{
			players[i] = *player;
			return;
		}
	}
}

void GameRoom::OneTick()
{
	if (!players[0].connected && !players[1].connected && !players[2].connected && !players[3].connected)
	{
		roomActive = false;
		return;
	}
	game.OneTick(((players[0] != NULL) ? players[0]->GameDir : 0),
		((players[1] != NULL) ? players[1]->GameDir : 0),
		((players[2] != NULL) ? players[2]->GameDir : 0),
		((players[3] != NULL) ? players[3]->GameDir : 0), &currentlyUsingPDs);
	for (int i = 0; i < 4; i++)
	{
		if (players[i] != NULL)
		{
			players[i]->RecvedPhysics = false;
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

std::vector<PhysicalObject>& GameRoom::GetPhysicsForPlayer()
{
	return game.physics;
}

void GameRoom::ChangePlayerDirection(char dir, connection* player)
{
	while (currentlyUsingPDs)
	{
		Sleep(1);
	}
	player->GameDir = (int)dir;
}