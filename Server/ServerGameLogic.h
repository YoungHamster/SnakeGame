#pragma once

#include <vector>

#include "ServerApple.h"

#define UP 1
#define DOWN 2
#define LEFT 3
#define RIGHT 4

struct SnakeBlock
{
	short PhysicalBodyID;
	char Direction;
};

class GameLogic
{
private:
	/*std::vector<PhysicalObject> physics;
	std::vector<SnakeBlock> snakes[4];*/
	bool snake1alive = false;
	bool snake2alive = false;
	bool snake3alive = false;
	bool snake4alive = false;

	Apple apple;
	AABB appleSpawnZone;

	int gameFieldWidth = 0;
	int gameFieldHeight = 0;

public:
	char playerDirs[4];

	std::vector<PhysicalObject> physics;
	std::vector<SnakeBlock> snakes[4];
	std::vector<std::vector<SnakeBlock>> newsnakes;
	void Reset();
	bool Init(short gameFieldWidth, short gameFieldHeight, short snake1Length, short snake2Length, short snake3Length, short snake4Length);
	int OneTick(char snake1dir, char snake2dir, char snake3dir, char snake4dir, bool *currentlyUsingPDs);
	bool EnlargeSnake(short Length, short SnakeID);
	bool MoveSnakeBlock(SnakeBlock* block);
	bool MoveSnake(char Direction, short SnakeID);
	void KillSnake(short SnakeID);
	void SpawnApple();
	void AddSnake(int snakeLenght, AABB snakeHead, int headDir);

	int HandleAIForSnake(short SnakeID);
	int CheckCollisionsForAI(short SnakeID);
	void AStar(AABB start, AABB goal);
};