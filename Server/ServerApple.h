#pragma once

#include "ServerPhysics.h"
#include "ServerRandom.h"

class Apple
{
private:
	short PhysicalBodyID;
public:
	void Init(PhysicalObject* apple, short PhysicalBodyID)
	{
		this->PhysicalBodyID = PhysicalBodyID;
		apple->type = APPLE;
	}

	void Spawn(AABB spawnZone, PhysicalObject* physicalBody)
	{
		physicalBody->borders.min.x = randomNumber(spawnZone.min.x, spawnZone.max.x);
		physicalBody->borders.min.y = randomNumber(spawnZone.min.y, spawnZone.max.y);
		physicalBody->borders.max.x = physicalBody->borders.min.x + 1;
		physicalBody->borders.max.y = physicalBody->borders.min.y + 1;
	}

	short GetPhysicalBodyID()
	{
		return PhysicalBodyID;
	}
};