#include "game.h"
#include "util.h"
#include "collision.h"
#include "entity.h"

#include <SDL2/SDL_ttf.h>
#include <string> 
#include <time.h>

void Game::Initialize()
{
    SDL_Init(SDL_INIT_VIDEO);
    TTF_Init();
    sdl_window = SDL_CreateWindow("SDL2 Demo", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 320, 280, 0 );
    sdl_renderer = SDL_CreateRenderer(sdl_window, -1, 0);

    font = TTF_OpenFont("pixeloid_sans.ttf", 16);
    brick_texture = IMG_LoadTexture(sdl_renderer, "brick_full.png");
    mystery_box_texture = IMG_LoadTexture(sdl_renderer, "mystery_box.png");
    empty_mystery_box_texture = IMG_LoadTexture(sdl_renderer, "empty_mystery_box.png");

    EnterMainMenu();
    Loop();
}
	
void Game::EnterMainMenu() {
    in_main_menu = true;
}

void Game::EnterGame()
{
    score = 0;

    Level_Screen screen0;
    screen0.index = 0;
    int ground_level[16] = {1, 1, 1, 1, 1, 2, 3, 4, 4, 4, 4, 4, 4, 4, 4, 4};
    Brick* brick = new Brick();
    brick->position.x = 11;
    brick->position.y = 8;
    screen0.entities.push_back(brick);

    Mystery_Box* mystery_box = new Mystery_Box();
    mystery_box->position.x = 12;
    mystery_box->position.y = 8;
    screen0.entities.push_back(mystery_box);

    for(int i=0; i<16; i++) {
        screen0.ground_level[i] = ground_level[i];
    }

    level_screens.push_back(screen0);
    
    Level_Screen screen1;
    screen1.index = 1;

    for(int i=0; i<16; i++) {
        screen1.ground_level[i] = ground_level[i];
    }
    screen1.ground_level[15] = 14;
    level_screens.push_back(screen1);

    in_main_menu = false;
    screen_left_position_x = 0;
    player_position.x = 0;
    player_position.y = 20;
}

void Game::Loop()
{
    while(true) {
        clock_t begin = clock();

        SDL_RenderClear(sdl_renderer);

        if (SDL_PollEvent(&sdl_event)) {
            if (sdl_event.type == SDL_QUIT) {
                break;
            } else if (sdl_event.type == SDL_KEYDOWN) {
                Handle_Key_Down(sdl_event.key.keysym.sym, keyboard);
            } else if (sdl_event.type == SDL_KEYUP) {
                Handle_Key_Up(sdl_event.key.keysym.sym, keyboard);
            }
        }

        int screen_index = screen_left_position_x / 160;
        screen_index = std::min(screen_index, 0);


        if (keyboard[KEY_RETURN]) {
            if (in_main_menu) {
                EnterGame();
            }
        }

        if (begin > next_move) {
            if (keyboard[KEY_UP]) {
                if (on_the_ground && can_jump) {
                    vertical_speed = 35;
                    on_the_ground = false;
                    can_jump = false;
                }
            } else {
                can_jump = true;
            }

            vertical_speed -= 1;
            player_position.y += vertical_speed / 6;

            if (keyboard[KEY_RIGHT]) {
                player_position.x += 1;
            } else if (keyboard[KEY_LEFT]) {
                player_position.x -= 1;
            }

            next_move = begin + CLOCKS_PER_SEC / 200;
        }

        if (player_position.x - screen_left_position_x > 100) {
            screen_left_position_x += player_position.x - screen_left_position_x - 100;
        }

        if (screen_left_position_x - player_position.x > 0) {
            player_position.x += screen_left_position_x - player_position.x;
        }

        if (!in_main_menu) {
            for (int i=0; i<2; i++) {
                std::vector<Collision_Record> collision_records = level_screens[screen_index + i].collision_test(player_position);

                for (const Collision_Record& collision_record : collision_records) {
                    if (collision_record.direction == DIRECTION_DOWN) {
                        player_position.y -= player_position.y + 40 - collision_record.position;
                        vertical_speed = 0;
                        collision_record.entity->Destroy(*this);
                    } else if (collision_record.direction == DIRECTION_UP) {
                        player_position.y += collision_record.position - player_position.y;
                        on_the_ground = true;
                        vertical_speed = 0;
                    } else if (collision_record.direction == DIRECTION_LEFT) {
                        player_position.x -= player_position.x + 20 - collision_record.position;
                    } else if (collision_record.direction == DIRECTION_RIGHT) {
                        player_position.x += collision_record.position - player_position.x;
                    }
                }
            }

            level_screens[screen_index].Render(*this);
            level_screens[screen_index + 1].Render(*this);

            SDL_Rect player;
            player.w = 20;
            player.h = 40;
            player.x = player_position.x - screen_left_position_x;
            player.y = 280 - 40 - player_position.y;

            SDL_SetRenderDrawColor(sdl_renderer, 0, 0, 0, 255);
            SDL_RenderFillRect(sdl_renderer, &player);

            SDL_Color black_color = {0, 0, 0};
            SDL_Surface* score_surface = TTF_RenderText_Solid(font, (std::string("Score: ") + std::to_string(score)).c_str(),  black_color);
            SDL_Texture* score_texture = SDL_CreateTextureFromSurface(sdl_renderer, score_surface);
            SDL_Rect score_rect;
            score_rect.w = score_surface->w;
            score_rect.h = score_surface->h;
            score_rect.x = 5;
            score_rect.y = 5;
            SDL_RenderCopy(sdl_renderer, score_texture, NULL, &score_rect);

            SDL_FreeSurface(score_surface);
            SDL_DestroyTexture(score_texture);
        }

        SDL_RenderPresent(sdl_renderer);
    }
}

void Game::Destroy() {
    SDL_DestroyRenderer(sdl_renderer);
    SDL_DestroyWindow(sdl_window);
    SDL_Quit();
    TTF_CloseFont(font);
    TTF_Quit();
}