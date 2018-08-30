#pragma once

struct XY {
	short x;
	short y;
};

struct AABB
{
	XY max;
	XY min;
};

struct PhysicalObject
{
	AABB borders;
	char type;
};

// Types of objects
#define SNAKE_1_HEAD_UP 1
#define SNAKE_1_HEAD_DOWN 2
#define SNAKE_1_HEAD_LEFT 3
#define SNAKE_1_HEAD_RIGHT 4
#define SNAKE_1_BODY 5
#define SNAKE_1_TAIL_UP 6
#define SNAKE_1_TAIL_DOWN 7
#define SNAKE_1_TAIL_LEFT 8
#define SNAKE_1_TAIL_RIGHT 9
#define SNAKE_2_HEAD_UP 10
#define SNAKE_2_HEAD_DOWN 11
#define SNAKE_2_HEAD_LEFT 12
#define SNAKE_2_HEAD_RIGHT 13
#define SNAKE_2_BODY 14
#define SNAKE_2_TAIL_UP 15
#define SNAKE_2_TAIL_DOWN 16
#define SNAKE_2_TAIL_LEFT 17
#define SNAKE_2_TAIL_RIGHT 18
#define SNAKE_3_HEAD_UP 19
#define SNAKE_3_HEAD_DOWN 20
#define SNAKE_3_HEAD_LEFT 21
#define SNAKE_3_HEAD_RIGHT 22
#define SNAKE_3_BODY 23
#define SNAKE_3_TAIL_UP 24
#define SNAKE_3_TAIL_DOWN 25
#define SNAKE_3_TAIL_LEFT 26
#define SNAKE_3_TAIL_RIGHT 27
#define SNAKE_4_HEAD_UP 28
#define SNAKE_4_HEAD_DOWN 29
#define SNAKE_4_HEAD_LEFT 30
#define SNAKE_4_HEAD_RIGHT 31
#define SNAKE_4_BODY 32
#define SNAKE_4_TAIL_UP 33
#define SNAKE_4_TAIL_DOWN 34
#define SNAKE_4_TAIL_LEFT 35
#define SNAKE_4_TAIL_RIGHT 36
#define DEAD_SNAKE 37
#define APPLE 38
#define BORDER 39



// Checks collision of a and b
inline bool AABBvsAABB(AABB a, AABB b)
{
	// Exit with no intersection if found separated along an axis
	//if (a.max.x < b.min.x or a.min.x > b.max.x) return false;
	//if (a.max.y < b.min.y or a.min.y > b.max.y) return false;
	if (a.max.x <= b.min.x or a.min.x >= b.max.x) return false; // wrong implementation
	if (a.max.y <= b.min.y or a.min.y >= b.max.y) return false; // wrong implementation

																// No separating axis found, therefor there is at least one overlapping axis
	return true;
}
