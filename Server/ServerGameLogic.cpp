#include "ServerGameLogic.h"

void GameLogic::Reset()
{
	for (int i = 0; i < 4; i++)
	{
		snakes[i].clear();
	}
	physics.clear();
}

bool GameLogic::Init(short gameFieldWidth, short gameFieldHeight, short snake1Length, short snake2Length, short snake3Length, short snake4Length)
{
	this->gameFieldWidth = gameFieldWidth;
	this->gameFieldHeight = gameFieldHeight;

	Reset();

	physics.resize(5 + snake1Length + snake2Length + snake3Length + snake4Length);
	PhysicalObject tempObj;
	SnakeBlock tempBlock;

	tempObj.type = BORDER;

	tempObj.borders.max.x = 1;
	tempObj.borders.min.x = -1;
	tempObj.borders.min.y = -1;
	tempObj.borders.max.y = gameFieldHeight + 1;

	physics[0] = tempObj;

	tempObj.borders.max.y = 1;
	tempObj.borders.min.y = -1;
	tempObj.borders.min.x = -1;
	tempObj.borders.max.x = gameFieldWidth + 1;

	physics[1] = tempObj;

	tempObj.borders.min.y = -1;
	tempObj.borders.max.y = gameFieldHeight + 1;
	tempObj.borders.min.x = gameFieldWidth - 1;
	tempObj.borders.max.x = gameFieldWidth + 1;

	physics[2] = tempObj;

	tempObj.borders.min.x = -1;
	tempObj.borders.max.x = gameFieldWidth + 1;
	tempObj.borders.min.y = gameFieldHeight;
	tempObj.borders.max.y = gameFieldHeight + 1;

	physics[3] = tempObj;

	if (snake1Length == 0)
	{
		tempObj.type = DEAD_SNAKE;
		physics[4] = tempObj;
		tempBlock.Direction = NULL;
		tempBlock.PhysicalBodyID = 4;
		snakes[0].push_back(tempBlock);
	}
	else
	{
		tempObj.type = SNAKE_1_HEAD_LEFT;
		tempObj.borders.min.x = gameFieldWidth / 2 - snake1Length / 2;
		tempObj.borders.min.y = gameFieldHeight / 2;
		tempObj.borders.max.x = tempObj.borders.min.x + 1;
		tempObj.borders.max.y = tempObj.borders.min.y + 1;
		physics[4] = tempObj;
		tempBlock.Direction = LEFT;
		tempBlock.PhysicalBodyID = 4;
		snakes[0].push_back(tempBlock);

		for (int i = 0; i < snake1Length - 2; i++)
		{
			tempObj.type = SNAKE_1_BODY;
			tempObj.borders.min.x = tempObj.borders.max.x;
			tempObj.borders.max.x = tempObj.borders.max.x + 1;
			physics[5 + i] = tempObj;
			tempBlock.Direction = LEFT;
			tempBlock.PhysicalBodyID = 5 + i;
			snakes[0].push_back(tempBlock);
		}

		tempObj.type = SNAKE_1_TAIL_LEFT;
		tempObj.borders.min.x = tempObj.borders.max.x;
		tempObj.borders.max.x = tempObj.borders.max.x + 1;
		physics[3 + snake1Length] = tempObj;
		tempBlock.Direction = LEFT;
		tempBlock.PhysicalBodyID = 3 + snake1Length;
		snakes[0].push_back(tempBlock);

		snake1alive = true;
	}

	if (snake2Length == 0)
	{
		tempObj.type = DEAD_SNAKE;
		physics[4 + snake1Length] = tempObj;
		tempBlock.Direction = NULL;
		tempBlock.PhysicalBodyID = 4 + snake1Length;
		snakes[1].push_back(tempBlock);
	}
	else
	{
		tempObj.type = SNAKE_2_HEAD_LEFT;
		tempObj.borders.min.x = gameFieldWidth / 2 - snake1Length / 2;
		tempObj.borders.min.y = gameFieldHeight / 2 + 2;
		tempObj.borders.max.x = tempObj.borders.min.x + 1;
		tempObj.borders.max.y = tempObj.borders.min.y + 1;
		physics[4 + snake1Length] = tempObj;
		tempBlock.Direction = LEFT;
		tempBlock.PhysicalBodyID = 4 + snake1Length;
		snakes[1].push_back(tempBlock);

		for (int i = 0; i < snake2Length - 2; i++)
		{
			tempObj.type = SNAKE_2_BODY;
			tempObj.borders.min.x = tempObj.borders.max.x;
			tempObj.borders.max.x = tempObj.borders.max.x + 1;
			physics[5 + i + snake1Length] = tempObj;
			tempBlock.Direction = LEFT;
			tempBlock.PhysicalBodyID = 5 + i + snake1Length;
			snakes[1].push_back(tempBlock);
		}

		tempObj.type = SNAKE_2_TAIL_LEFT;
		tempObj.borders.min.x = tempObj.borders.max.x;
		tempObj.borders.max.x = tempObj.borders.max.x + 1;
		physics[3 + snake1Length + snake2Length] = tempObj;
		tempBlock.Direction = LEFT;
		tempBlock.PhysicalBodyID = 3 + snake1Length + snake2Length;
		snakes[1].push_back(tempBlock);
		snake2alive = true;
	}

	if (snake3Length == 0)
	{
		tempObj.type = DEAD_SNAKE;
		physics[4 + snake1Length + snake2Length] = tempObj;
		tempBlock.Direction = NULL;
		tempBlock.PhysicalBodyID = 4 + snake1Length + snake2Length;
		snakes[2].push_back(tempBlock);
	}
	else
	{
		tempObj.type = SNAKE_3_HEAD_LEFT;
		tempObj.borders.min.x = gameFieldWidth / 2 - snake1Length / 2;
		tempObj.borders.min.y = gameFieldHeight / 2 - 2;
		tempObj.borders.max.x = tempObj.borders.min.x + 1;
		tempObj.borders.max.y = tempObj.borders.min.y + 1;
		physics[4 + snake1Length + snake2Length] = tempObj;
		tempBlock.Direction = LEFT;
		tempBlock.PhysicalBodyID = 4 + snake1Length + snake2Length;
		snakes[2].push_back(tempBlock);

		for (int i = 0; i < snake3Length - 2; i++)
		{
			tempObj.type = SNAKE_3_BODY;
			tempObj.borders.min.x = tempObj.borders.max.x;
			tempObj.borders.max.x = tempObj.borders.max.x + 1;
			physics[5 + i + snake1Length + snake2Length] = tempObj;
			tempBlock.Direction = LEFT;
			tempBlock.PhysicalBodyID = 5 + i + snake1Length + snake2Length;
			snakes[2].push_back(tempBlock);
		}

		tempObj.type = SNAKE_3_TAIL_LEFT;
		tempObj.borders.min.x = tempObj.borders.max.x;
		tempObj.borders.max.x = tempObj.borders.max.x + 1;
		physics[8 + snake1Length + snake2Length] = tempObj;
		tempBlock.Direction = LEFT;
		tempBlock.PhysicalBodyID = 8 + snake1Length + snake2Length;
		snakes[2].push_back(tempBlock);
		snake3alive = true;
	}

	if (snake4Length == 0)
	{
		tempObj.type = DEAD_SNAKE;
		physics[4 + snake1Length + snake2Length + snake3Length] = tempObj;
		tempBlock.Direction = NULL;
		tempBlock.PhysicalBodyID = 4 + snake1Length + snake2Length + snake3Length;
		snakes[3].push_back(tempBlock);
	}
	else
	{
		tempObj.type = SNAKE_4_HEAD_LEFT;
		tempObj.borders.min.x = gameFieldWidth / 2 - snake1Length / 2;
		tempObj.borders.min.y = gameFieldHeight / 2 - 4;
		tempObj.borders.max.x = tempObj.borders.min.x + 1;
		tempObj.borders.max.y = tempObj.borders.min.y + 1;
		physics[4 + snake1Length + snake2Length + snake3Length] = tempObj;
		tempBlock.Direction = LEFT;
		tempBlock.PhysicalBodyID = 4 + snake1Length + snake2Length + snake3Length;
		snakes[3].push_back(tempBlock);

		for (int i = 0; i < snake4Length - 2; i++)
		{
			tempObj.type = SNAKE_4_BODY;
			tempObj.borders.min.x = tempObj.borders.max.x;
			tempObj.borders.max.x = tempObj.borders.max.x + 1;
			physics[5 + i + snake1Length + snake2Length + snake3Length] = tempObj;
			tempBlock.Direction = LEFT;
			tempBlock.PhysicalBodyID = 5 + i + snake1Length + snake2Length + snake3Length;
			snakes[3].push_back(tempBlock);
		}

		tempObj.type = SNAKE_4_TAIL_LEFT;
		tempObj.borders.min.x = tempObj.borders.max.x;
		tempObj.borders.max.x = tempObj.borders.max.x + 1;
		physics[8 + snake1Length + snake2Length + snake3Length] = tempObj;
		tempBlock.Direction = LEFT;
		tempBlock.PhysicalBodyID = 8 + snake1Length + snake2Length + snake3Length;
		snakes[3].push_back(tempBlock);
		snake4alive = true;
	}

	if (snake1Length == 0 && snake2Length == 0 && snake3Length == 0 && snake4Length == 0)
		return false;

	tempObj.type = APPLE;
	physics.push_back(tempObj);
	apple.Init(&physics[physics.size() - 1], physics.size() - 1);

	appleSpawnZone.min.x = 2;
	appleSpawnZone.min.y = 2;
	appleSpawnZone.max.x = gameFieldWidth - 2;
	appleSpawnZone.max.y = gameFieldHeight - 2;

	apple.Spawn(appleSpawnZone, &physics[apple.GetPhysicalBodyID()]);
	playerDirs[0] = LEFT;
	playerDirs[1] = LEFT;
	playerDirs[2] = LEFT;
	playerDirs[3] = LEFT;

	return true;
}

int GameLogic::OneTick(char snake1dir, char snake2dir, char snake3dir, char snake4dir, bool *currentlyUsingPDs)
{
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < physics.size(); j++)
		{
			if (physics[j].type != DEAD_SNAKE && snakes[i][0].PhysicalBodyID != -1)
			{
				if (AABBvsAABB(physics[snakes[i][0].PhysicalBodyID].borders, physics[j].borders) && j != snakes[i][0].PhysicalBodyID)
				{
					if (physics[j].type == APPLE)
					{
						EnlargeSnake(1, i);
						//apple.Spawn(appleSpawnZone, &physics[apple.GetPhysicalBodyID()]);
						SpawnApple();
					}
					else
					{
						KillSnake(i);
					}
				}
			}
		}
	}

	*currentlyUsingPDs = true;
	if (snake1alive)
	{
		if (snake1dir != 0)
		{
			MoveSnake(snake1dir, 0);
			playerDirs[0] = snake1dir;
		}
		else
		{
			playerDirs[0] = HandleAIForSnake(0);
			MoveSnake(playerDirs[0], 0);
		}
	}
	if (snake2alive)
	{
		if (snake2dir != 0)
		{
			MoveSnake(snake2dir, 1);
			playerDirs[1] = snake2dir;
		}
		else
		{
			playerDirs[1] = HandleAIForSnake(1);
			MoveSnake(playerDirs[1], 1);
		}
	}
	if (snake3alive)
	{
		if (snake3dir != 0)
		{
			MoveSnake(snake3dir, 2);
			playerDirs[2] = snake3dir;
		}
		else
		{
			playerDirs[2] = HandleAIForSnake(2);
			MoveSnake(playerDirs[2], 2);
		}
	}
	if (snake4alive)
	{
		if (snake4dir != 0)
		{
			MoveSnake(snake4dir, 3);
			playerDirs[3] = snake4dir;
		}
		else
		{
			playerDirs[3] = HandleAIForSnake(3);
			MoveSnake(playerDirs[3], 3);
		}
	}
	*currentlyUsingPDs = false;
	if (!snake1alive && !snake2alive && !snake3alive && !snake4alive)
	{
		Reset();
		Init(64, 36, 5, 5, 5, 5);
		return 1;
	}
	//if (snake2alive)
	//	MoveSnake(snake2dir, 1);
	//if (snake3alive)
	//	MoveSnake(snake3dir, 2);
	//if (snake4alive)
	//	MoveSnake(snake4dir, 3);

	return 0;
}

bool GameLogic::EnlargeSnake(short Length, short SnakeID)
{
	if (SnakeID > 3 || SnakeID < 0) return false;

	SnakeBlock tempBlock;
	PhysicalObject tempObject;

	for (int i = 0; i < Length; i++)
	{
		tempObject = physics[snakes[SnakeID][snakes[SnakeID].size() - 1].PhysicalBodyID];
		physics[snakes[SnakeID][snakes[SnakeID].size() - 1].PhysicalBodyID].type = physics[snakes[SnakeID][snakes[SnakeID].size() - 1].PhysicalBodyID].type -
			snakes[SnakeID][snakes[SnakeID].size() - 1].Direction;

		switch (snakes[SnakeID][snakes[SnakeID].size() - 1].Direction)
		{
		case UP:
			tempObject.borders.max.y = tempObject.borders.min.y;
			tempObject.borders.min.y = tempObject.borders.min.y - 1;
			break;

		case DOWN:
			tempObject.borders.min.y = tempObject.borders.max.y;
			tempObject.borders.max.y = tempObject.borders.max.y + 1;
			break;

		case LEFT:
			tempObject.borders.min.x = tempObject.borders.max.x;
			tempObject.borders.max.x = tempObject.borders.max.x + 1;
			break;

		case RIGHT:
			tempObject.borders.max.x = tempObject.borders.min.x;
			tempObject.borders.min.x = tempObject.borders.min.x - 1;
			break;

		default:
			return false;
		}
		tempBlock.Direction = snakes[SnakeID][snakes[SnakeID].size() - 1].Direction;

		physics.push_back(tempObject);
		tempBlock.PhysicalBodyID = physics.size() - 1;
		snakes[SnakeID].push_back(tempBlock);
	}

	return true;
}

bool GameLogic::MoveSnakeBlock(SnakeBlock* block)
{
	switch (block->Direction)
	{
	case UP:
		if (physics[block->PhysicalBodyID].type == SNAKE_1_HEAD_RIGHT || physics[block->PhysicalBodyID].type == SNAKE_2_HEAD_RIGHT || physics[block->PhysicalBodyID].type == SNAKE_3_HEAD_RIGHT ||
			physics[block->PhysicalBodyID].type == SNAKE_4_HEAD_RIGHT || physics[block->PhysicalBodyID].type == SNAKE_1_TAIL_RIGHT || physics[block->PhysicalBodyID].type == SNAKE_2_TAIL_RIGHT ||
			physics[block->PhysicalBodyID].type == SNAKE_3_TAIL_RIGHT || physics[block->PhysicalBodyID].type == SNAKE_4_TAIL_RIGHT)
		{
			physics[block->PhysicalBodyID].type = physics[block->PhysicalBodyID].type - 3;
		}

		if (physics[block->PhysicalBodyID].type == SNAKE_1_HEAD_LEFT || physics[block->PhysicalBodyID].type == SNAKE_2_HEAD_LEFT ||
			physics[block->PhysicalBodyID].type == SNAKE_3_HEAD_LEFT || physics[block->PhysicalBodyID].type == SNAKE_4_HEAD_LEFT || physics[block->PhysicalBodyID].type == SNAKE_1_TAIL_LEFT ||
			physics[block->PhysicalBodyID].type == SNAKE_2_TAIL_LEFT || physics[block->PhysicalBodyID].type == SNAKE_3_TAIL_LEFT || physics[block->PhysicalBodyID].type == SNAKE_4_TAIL_LEFT)
		{
			physics[block->PhysicalBodyID].type = physics[block->PhysicalBodyID].type - 2;
		}

		physics[block->PhysicalBodyID].borders.min.y = physics[block->PhysicalBodyID].borders.max.y;
		physics[block->PhysicalBodyID].borders.max.y = physics[block->PhysicalBodyID].borders.max.y + 1;
		break;
	case DOWN:
		if (physics[block->PhysicalBodyID].type == SNAKE_1_HEAD_RIGHT || physics[block->PhysicalBodyID].type == SNAKE_2_HEAD_RIGHT ||
			physics[block->PhysicalBodyID].type == SNAKE_3_HEAD_RIGHT || physics[block->PhysicalBodyID].type == SNAKE_4_HEAD_RIGHT || physics[block->PhysicalBodyID].type == SNAKE_1_TAIL_RIGHT ||
			physics[block->PhysicalBodyID].type == SNAKE_2_TAIL_RIGHT || physics[block->PhysicalBodyID].type == SNAKE_3_TAIL_RIGHT || physics[block->PhysicalBodyID].type == SNAKE_4_TAIL_RIGHT)
		{
			physics[block->PhysicalBodyID].type = physics[block->PhysicalBodyID].type - 2;
		}

		if (physics[block->PhysicalBodyID].type == SNAKE_1_HEAD_LEFT || physics[block->PhysicalBodyID].type == SNAKE_2_HEAD_LEFT || physics[block->PhysicalBodyID].type == SNAKE_3_HEAD_LEFT ||
			physics[block->PhysicalBodyID].type == SNAKE_4_HEAD_LEFT || physics[block->PhysicalBodyID].type == SNAKE_1_TAIL_LEFT || physics[block->PhysicalBodyID].type == SNAKE_2_TAIL_LEFT ||
			physics[block->PhysicalBodyID].type == SNAKE_3_TAIL_LEFT || physics[block->PhysicalBodyID].type == SNAKE_4_TAIL_LEFT)
		{
			physics[block->PhysicalBodyID].type = physics[block->PhysicalBodyID].type - 1;
		}

		physics[block->PhysicalBodyID].borders.max.y = physics[block->PhysicalBodyID].borders.min.y;
		physics[block->PhysicalBodyID].borders.min.y = physics[block->PhysicalBodyID].borders.min.y - 1;
		break;
	case LEFT:
		if (physics[block->PhysicalBodyID].type == SNAKE_1_HEAD_UP || physics[block->PhysicalBodyID].type == SNAKE_2_HEAD_UP || physics[block->PhysicalBodyID].type == SNAKE_3_HEAD_UP ||
			physics[block->PhysicalBodyID].type == SNAKE_4_HEAD_UP || physics[block->PhysicalBodyID].type == SNAKE_1_TAIL_UP || physics[block->PhysicalBodyID].type == SNAKE_2_TAIL_UP ||
			physics[block->PhysicalBodyID].type == SNAKE_3_TAIL_UP || physics[block->PhysicalBodyID].type == SNAKE_4_TAIL_UP)
		{
			physics[block->PhysicalBodyID].type = physics[block->PhysicalBodyID].type + 2;
		}

		if (physics[block->PhysicalBodyID].type == SNAKE_1_HEAD_DOWN || physics[block->PhysicalBodyID].type == SNAKE_2_HEAD_DOWN || physics[block->PhysicalBodyID].type == SNAKE_3_HEAD_DOWN ||
			physics[block->PhysicalBodyID].type == SNAKE_4_HEAD_DOWN || physics[block->PhysicalBodyID].type == SNAKE_1_TAIL_DOWN || physics[block->PhysicalBodyID].type == SNAKE_2_TAIL_DOWN ||
			physics[block->PhysicalBodyID].type == SNAKE_3_TAIL_DOWN || physics[block->PhysicalBodyID].type == SNAKE_4_TAIL_DOWN)
		{
			physics[block->PhysicalBodyID].type = physics[block->PhysicalBodyID].type + 1;
		}

		physics[block->PhysicalBodyID].borders.max.x = physics[block->PhysicalBodyID].borders.min.x;
		physics[block->PhysicalBodyID].borders.min.x = physics[block->PhysicalBodyID].borders.min.x - 1;
		break;
	case RIGHT:
		if (physics[block->PhysicalBodyID].type == SNAKE_1_HEAD_UP || physics[block->PhysicalBodyID].type == SNAKE_2_HEAD_UP || physics[block->PhysicalBodyID].type == SNAKE_3_HEAD_UP ||
			physics[block->PhysicalBodyID].type == SNAKE_4_HEAD_UP || physics[block->PhysicalBodyID].type == SNAKE_1_TAIL_UP || physics[block->PhysicalBodyID].type == SNAKE_2_TAIL_UP ||
			physics[block->PhysicalBodyID].type == SNAKE_3_TAIL_UP || physics[block->PhysicalBodyID].type == SNAKE_4_TAIL_UP)
		{
			physics[block->PhysicalBodyID].type = physics[block->PhysicalBodyID].type + 3;
		}

		if (physics[block->PhysicalBodyID].type == SNAKE_1_HEAD_DOWN || physics[block->PhysicalBodyID].type == SNAKE_2_HEAD_DOWN || physics[block->PhysicalBodyID].type == SNAKE_3_HEAD_DOWN ||
			physics[block->PhysicalBodyID].type == SNAKE_4_HEAD_DOWN || physics[block->PhysicalBodyID].type == SNAKE_1_TAIL_DOWN || physics[block->PhysicalBodyID].type == SNAKE_2_TAIL_DOWN ||
			physics[block->PhysicalBodyID].type == SNAKE_3_TAIL_DOWN || physics[block->PhysicalBodyID].type == SNAKE_4_TAIL_DOWN)
		{
			physics[block->PhysicalBodyID].type = physics[block->PhysicalBodyID].type + 2;
		}

		physics[block->PhysicalBodyID].borders.min.x = physics[block->PhysicalBodyID].borders.max.x;
		physics[block->PhysicalBodyID].borders.max.x = physics[block->PhysicalBodyID].borders.max.x + 1;
		break;
	default:
		return false;
	}
	return true;
}

bool GameLogic::MoveSnake(char Direction, short SnakeID)
{
	if (SnakeID > 3 || SnakeID < 0) return false;

	for (int i = 0; i < snakes[SnakeID].size() - 1; i++)
	{
		snakes[SnakeID][snakes[SnakeID].size() - 1 - i].Direction = snakes[SnakeID][snakes[SnakeID].size() - 2 - i].Direction;
	}

	// Check if player is not trying to turn snakes head(and the entire snake) 180 degrees
	switch (Direction)
	{
	case UP:
		if (snakes[SnakeID][0].Direction != DOWN)
			snakes[SnakeID][0].Direction = UP;
		break;

	case DOWN:
		if (snakes[SnakeID][0].Direction != UP)
			snakes[SnakeID][0].Direction = DOWN;
		break;

	case LEFT:
		if (snakes[SnakeID][0].Direction != RIGHT)
			snakes[SnakeID][0].Direction = LEFT;
		break;

	case RIGHT:
		if (snakes[SnakeID][0].Direction != LEFT)
			snakes[SnakeID][0].Direction = RIGHT;
		break;
	}

	for (int i = 0; i < snakes[SnakeID].size(); i++)
	{
		if (!MoveSnakeBlock(&snakes[SnakeID][i])) return false;
	}

	return true;
}

void GameLogic::KillSnake(short SnakeID)
{
	snakes[SnakeID][0].Direction = NULL;
	switch (SnakeID)
	{
	case 0:
		for (int i = 0; i < snakes[0].size(); i++)
		{
			physics[snakes[SnakeID][i].PhysicalBodyID].type = DEAD_SNAKE;
			snakes[SnakeID][i].PhysicalBodyID = -1;
		}
		snake1alive = false;
		break;
	case 1:
		for (int i = 0; i < snakes[1].size(); i++)
		{
			physics[snakes[SnakeID][i].PhysicalBodyID].type = DEAD_SNAKE;
			snakes[SnakeID][i].PhysicalBodyID = -1;
		}
		snake2alive = false;
		break;
	case 2:
		for (int i = 0; i < snakes[2].size(); i++)
		{
			physics[snakes[SnakeID][i].PhysicalBodyID].type = DEAD_SNAKE;
			snakes[SnakeID][i].PhysicalBodyID = -1;
		}
		snake3alive = false;
		break;
	case 3:
		for (int i = 0; i < snakes[3].size(); i++)
		{
			physics[snakes[SnakeID][i].PhysicalBodyID].type = DEAD_SNAKE;
			snakes[SnakeID][i].PhysicalBodyID = -1;
		}
		snake4alive = false;
		break;
	}
	snakes[SnakeID].erase(snakes[SnakeID].begin() + 1, snakes[SnakeID].end());
}

void GameLogic::SpawnApple()
{
	AABB* possibleApplePositions = new AABB[(gameFieldWidth - 2) * (gameFieldHeight - 2)];
	AABB possibleApplePosition;
	int numberOfPossibleApplePositions = 0;
	for (int i = 1; i < gameFieldWidth - 1; i++)
	{
		for (int j = 1; j < gameFieldHeight - 1; j++)
		{
			possibleApplePosition.min.x = i;
			possibleApplePosition.min.y = j;
			possibleApplePosition.max.x = i + 1;
			possibleApplePosition.max.y = j + 1;
			for (int k = 0; k < physics.size(); k++)
			{
				if (!NormalAABBvsAABB(possibleApplePosition, physics[k].borders) && physics[k].type != DEAD_SNAKE)
				{
					possibleApplePositions[numberOfPossibleApplePositions] = possibleApplePosition;
					numberOfPossibleApplePositions++;
					k = physics.size();
				}
			}
		}
	}
	physics[apple.GetPhysicalBodyID()].borders = possibleApplePositions[randomNumber(0, numberOfPossibleApplePositions - 1)];
}

/* Once i will write this function and rewrite renderer, but not now */
void GameLogic::AddSnake(int snakeLenght, AABB snakeHead, int headDir)
{
	std::vector<SnakeBlock> newSnake;

}

int GameLogic::HandleAIForSnake(short SnakeID)
{
	bool canTurnUp = true;
	bool canTurnDown = true;
	bool canTurnLeft = true;
	bool canTurnRight = true;
	AABB possibleFuturePos = physics[snakes[SnakeID][0].PhysicalBodyID].borders;
	possibleFuturePos.min.x = possibleFuturePos.max.x;
	possibleFuturePos.max.x = possibleFuturePos.max.x + 1;
	for (int i = 0; i < physics.size(); i++)
	{
		if (AABBvsAABB(physics[i].borders, possibleFuturePos) && physics[i].type != APPLE && physics[i].type != DEAD_SNAKE)
			canTurnRight = false;
	}

	possibleFuturePos.max.x = possibleFuturePos.max.x - 2;
	possibleFuturePos.min.x = possibleFuturePos.min.x - 2;
	for (int i = 0; i < physics.size(); i++)
	{
		if (AABBvsAABB(physics[i].borders, possibleFuturePos) && physics[i].type != APPLE && physics[i].type != DEAD_SNAKE)
			canTurnLeft = false;
	}

	possibleFuturePos = physics[snakes[SnakeID][0].PhysicalBodyID].borders;
	possibleFuturePos.min.y = possibleFuturePos.max.y;
	possibleFuturePos.max.y = possibleFuturePos.max.y + 1;
	for (int i = 0; i < physics.size(); i++)
	{
		if (AABBvsAABB(physics[i].borders, possibleFuturePos) && physics[i].type != APPLE && physics[i].type != DEAD_SNAKE)
			canTurnUp = false;
	}

	possibleFuturePos.min.y = possibleFuturePos.min.y - 2;
	possibleFuturePos.max.y = possibleFuturePos.max.y - 2;
	for (int i = 0; i < physics.size(); i++)
	{
		if (AABBvsAABB(physics[i].borders, possibleFuturePos) && physics[i].type != APPLE && physics[i].type != DEAD_SNAKE)
			canTurnDown = false;
	}

	if (physics[apple.GetPhysicalBodyID()].borders.min.y > physics[snakes[SnakeID][0].PhysicalBodyID].borders.min.y)
	{
		if (canTurnUp)
			return UP;
		if (canTurnLeft)
			return LEFT;
		if (canTurnRight)
			return RIGHT;
		if (canTurnDown)
			return DOWN;
	}
	if (physics[apple.GetPhysicalBodyID()].borders.min.y < physics[snakes[SnakeID][0].PhysicalBodyID].borders.min.y)
	{
		if (canTurnDown)
			return DOWN;
		if (canTurnLeft)
			return LEFT;
		if (canTurnRight)
			return RIGHT;
		if (canTurnUp)
			return UP;
	}
	if (physics[apple.GetPhysicalBodyID()].borders.min.x < physics[snakes[SnakeID][0].PhysicalBodyID].borders.min.x)
	{
		if (canTurnLeft)
			return LEFT;
		if (canTurnUp)
			return UP;
		if (canTurnDown)
			return DOWN;
		if (canTurnRight)
			return RIGHT;
	}
	if (physics[apple.GetPhysicalBodyID()].borders.min.x > physics[snakes[SnakeID][0].PhysicalBodyID].borders.min.x)
	{
		if (canTurnRight)
			return RIGHT;
		if (canTurnUp)
			return UP;
		if (canTurnDown)
			return DOWN;
		if (canTurnLeft)
			return LEFT;
	}
	if (canTurnUp)
		return UP;
	if (canTurnLeft)
		return LEFT;
	if (canTurnRight)
		return RIGHT;
	if (canTurnDown)
		return DOWN;
	return UP;
}