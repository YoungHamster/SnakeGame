#pragma once

#include <vector>

#include "Apple.h"

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
	bool snake1alive = false;
	bool snake2alive = false;
	bool snake3alive = false;
	bool snake4alive = false;

	Apple apple;
	AABB appleSpawnZone;

	int gameFieldWidth = 64;
	int gameFieldHeight = 36;

public:
	/* Created for server */
	char playerDirs[4];

	std::vector<PhysicalObject> physics;
	std::vector<SnakeBlock> snakes[4];
	std::vector<std::vector<SnakeBlock>> newsnakes;
	void Reset();
	bool Init(int gameFieldWidth, int gameFieldHeight, int snake1Length, int snake2Length, int snake3Length, int snake4Length);
	int OneTick(char snake1dir, char snake2dir, char snake3dir, char snake4dir);
	int ServerOneTick(char snake1dir, char snake2dir, char snake3dir, char snake4dir, bool *currentlyUsingPDs);
	bool EnlargeSnake(int Length, int SnakeID);
	bool MoveSnakeBlock(SnakeBlock* block);
	bool MoveSnake(char Direction, int SnakeID);
	void KillSnake(int SnakeID);
	void SpawnApple();
	void AddSnake(int snakeLenght, AABB snakeHead, int headDir);

	/* Created for using in client in multiplayer */
	void ChangeApplePostition(short minx, short miny);

	int HandleAIForSnake(int SnakeID);
	int CheckCollisionsForAI(short SnakeID);
	void AStar(AABB start, AABB goal);
};