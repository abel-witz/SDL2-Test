#ifndef GAME_H
#define GAME_H

#include "level_screen.h"
#include "util.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <time.h>
#include <vector>

#include <SDL2/SDL_ttf.h>

class Level_Screen;

class Game {
public:
	SDL_Window* sdl_window;
	SDL_Renderer* sdl_renderer;

	std::vector<Level_Screen> level_screens;

	TTF_Font* font;
	SDL_Texture* brick_texture;
	SDL_Texture* mystery_box_texture;
	SDL_Texture* empty_mystery_box_texture;

	Vector2D player_position;
	int screen_left_position_x;
	int vertical_speed = 0;
	bool on_the_ground = true;
	bool can_jump = true;

	int score = 0;

	bool keyboard[128];
	clock_t next_move;
	SDL_Event sdl_event;

	void Initialize();
	void EnterMainMenu();
	void EnterGame();
	void Loop();
	void Destroy();
};

#endif
