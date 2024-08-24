#ifndef ENTITY_H
#define ENTITY_H

#include "util.h"
#include "game.h"

#include <SDL2/SDL.h>

enum Entity_Type {ENTITY_BRICK, ENTITY_MYSTERY};

class Game;
class Vector2D;

class Entity {
public:
	bool deleted = false;
	Entity_Type type;
	Vector2D position;

	virtual void Render(const Game& game, int index) const = 0;
	virtual void Destroy(Game& game) = 0;
};

class Brick : public Entity {
public:
	Brick() {
		type = ENTITY_BRICK;
	}
	
	void Render(const Game& game, int index) const override;
	void Destroy(Game& game) override;
};

enum Mystery_Type {MYSTERY_COIN};

class Mystery_Box : public Entity {
public:
	Mystery_Type mystery_type;

	Mystery_Box() {
		type = ENTITY_MYSTERY;
	}
	
	void Render(const Game& game, int index) const override;
	void Destroy(Game& game) override;
};

#endif