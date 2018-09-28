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
	/*std::vector<PhysicalObject> physics;
	std::vector<SnakeBlock> snakes[4];*/
	bool snake1alive = false;
	bool snake2alive = false;
	bool snake3alive = false;
	bool snake4alive = false;

	Apple apple;
	AABB appleSpawnZone;

	short GameFieldWidth = 64;
	short GameFieldHeight = 36;

public:
	std::vector<PhysicalObject> physics;
	std::vector<SnakeBlock> snakes[4];
	void Reset();
	bool Init(short gameFieldWidth, short gameFieldHeight, short snake1Length, short snake2Length, short snake3Length, short snake4Length);
	int OneTick(char snake1dir, char snake2dir, char snake3dir, char snake4dir);
	bool EnlargeSnake(int Length, int SnakeID);
	bool MoveSnakeBlock(SnakeBlock* block);
	bool MoveSnake(char Direction, int SnakeID);
	void KillSnake(int SnakeID);

	int HandleAIForSnake(int SnakeID);
	int CheckCollisionsForAI(int SnakeID);
	void AStar(AABB start, AABB goal);
};