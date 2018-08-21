#pragma once

#include <vector>

#include "Physics.h"
#include "Apple.h"

#define UP 1
#define DOWN 2
#define LEFT 3
#define RIGHT 4

struct SnakeBlock
{
	int PhysicalBodyID;
	int Direction;
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

public:
	std::vector<PhysicalObject> physics;
	std::vector<SnakeBlock> snakes[4];
	void Reset();
	bool Init(unsigned int gameFieldWidth, unsigned int gameFieldHeight, int snake1Length, int snake2Length, int snake3Length, int snake4Length);
	int OneTick(int snake1dir, int snake2dir, int snake3dir, int snake4dir);
	bool EnlargeSnake(int Length, int SnakeID);
	bool MoveSnakeBlock(SnakeBlock* block);
	bool MoveSnake(int Direction, int SnakeID);
	void KillSnake(int SnakeID);

	int HandleAIForSnake(int SnakeID);
	int CheckCollisionsForAI(int SnakeID);
	void AStar(AABB start, AABB goal);
};