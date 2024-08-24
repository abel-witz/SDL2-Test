#ifndef COLLISION_H
#define COLLISION_H

#include "entity.h"

enum Direction {DIRECTION_UP, DIRECTION_DOWN, DIRECTION_RIGHT, DIRECTION_LEFT};

class Entity;

struct Collision_Record {
	Direction direction;
	int position;
    Entity* entity;
	int player_bound_exceed;
};

#endif