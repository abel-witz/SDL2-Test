#ifndef LEVEL_SCREEN_H
#define LEVEL_SCREEN_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <vector>

#include "util.h"
#include "entity.h"
#include "game.h"
#include "collision.h"

class Entity;
class Game;

class Level_Screen {
public:
	int index;
	int ground_level[16] = {1};
	std::vector<Entity*> entities;

	std::vector<Collision_Record> collision_test(const Vector2D& player_position);
	void Render(Game& game);

private:
	std::vector<Collision_Record> ground_collision_test(const Vector2D& player_position);
	std::vector<Collision_Record> entity_collision_test(const Vector2D& player_position);
};

#endif