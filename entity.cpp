#include "entity.h"
#include "game.h"

#include <iostream>

void Brick::Render(const Game& game, int index) const {
    if (deleted)
        return;

    int background_x = index*320 - game.screen_left_position_x;

    SDL_Rect rect;
    rect.w = 20;
    rect.h = 20;
    rect.x = background_x + position.x*20 - 20;;
    rect.y = 280 - position.y*20;

    SDL_RenderCopy(game.sdl_renderer, game.brick_texture, NULL, &rect);
}

void Brick::Destroy(Game& game) {
    deleted = true;
}

void Mystery_Box::Render(const Game& game, int index) const {
    int background_x = index*320 - game.screen_left_position_x;

    SDL_Rect rect;
    rect.w = 20;
    rect.h = 20;
    rect.x = background_x + position.x*20 - 20;;
    rect.y = 280 - position.y*20;

    if (deleted) {
        SDL_RenderCopy(game.sdl_renderer, game.empty_mystery_box_texture, NULL, &rect);
    } else {
        SDL_RenderCopy(game.sdl_renderer, game.mystery_box_texture, NULL, &rect);
    }
}

void Mystery_Box::Destroy(Game& game) {
    if (deleted)
        return;
        
    deleted = true;
    game.score += 1;
}