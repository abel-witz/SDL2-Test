#include "util.h"
#include "level_screen.h"
#include "collision.h"
#include "game.h"
#include <vector>
#include <iostream>

std::vector<Collision_Record> Level_Screen::ground_collision_test(const Vector2D& player_position) {
    std::vector<Collision_Record> collision_records;

    for (int i=0; i<16; i++) {
        Collision_Record collision_record;

        int up_penetration = ground_level[i]*20 - player_position.y;
        int left_penetration = player_position.x + 20 - i*20 - index*320;
        int right_penetration = i*20 + index*320 + 20 - player_position.x;

        if (up_penetration > 0 && right_penetration > 0 && left_penetration > 0)  {

            collision_record.direction = DIRECTION_UP;
            int least_penetration = up_penetration;

            if (right_penetration < least_penetration) {
                collision_record.direction = DIRECTION_RIGHT;
                least_penetration = right_penetration;
            }

            if (left_penetration < least_penetration) {
                collision_record.direction = DIRECTION_LEFT;
            }

            if (collision_record.direction == DIRECTION_UP) {
                collision_record.position = ground_level[i]*20;
            } else if (collision_record.direction == DIRECTION_LEFT) {
                collision_record.position = i*20 + index*320;
            } else if (collision_record.direction == DIRECTION_RIGHT) {
                collision_record.position = i*20 + index*320 + 20;
            }

            collision_record.entity = nullptr;
            collision_records.push_back(collision_record);
        }
    }

    return collision_records;
}

std::vector<Collision_Record> Level_Screen::entity_collision_test(const Vector2D& player_position) {
    std::vector<Collision_Record> collision_records;

    Collision_Record down_collision_with_least_player_exceed;
    bool down_collision = false;

    for (Entity* entity : entities) {
        if (entity->deleted && entity->type == ENTITY_BRICK)
            continue;

        Collision_Record collision_record;

        int up_penetration = entity->position.y*20 - player_position.y;
        int down_penetration = player_position.y + 40 - entity->position.y*20 + 20;
        int left_penetration = player_position.x + 40 - entity->position.x*20 - index*320;
        int right_penetration = entity->position.x*20 - index*320 - player_position.x;

        if (up_penetration > 0 && down_penetration > 0 && right_penetration > 0 && left_penetration > 0) {
            collision_record.direction = DIRECTION_UP;
            int least_penetration = up_penetration;

            if (down_penetration < least_penetration) {
                collision_record.direction = DIRECTION_DOWN;
                least_penetration = down_penetration;
            }

            if (right_penetration < least_penetration) {
                collision_record.direction = DIRECTION_RIGHT;
                least_penetration = right_penetration;
            }

            if (left_penetration < least_penetration) {
                collision_record.direction = DIRECTION_LEFT;
            }

            if (collision_record.direction == DIRECTION_UP) {
                collision_record.position = entity->position.y*20;
            } else if (collision_record.direction == DIRECTION_DOWN) {
                collision_record.position = entity->position.y*20 - 20;
            } else if (collision_record.direction == DIRECTION_LEFT) {
                collision_record.position = entity->position.x*20 + index*320 - 20;
            } else if (collision_record.direction == DIRECTION_RIGHT) {
                collision_record.position = entity->position.x*20 + index*320;
            }

            collision_record.entity = entity;
            collision_record.player_bound_exceed = std::abs(left_penetration - 10);

            if (collision_record.direction == DIRECTION_DOWN) {
                if (collision_record.player_bound_exceed < down_collision_with_least_player_exceed.player_bound_exceed || !down_collision) {
                    down_collision_with_least_player_exceed = collision_record;
                    down_collision = true;
                }
            } else {
                collision_records.push_back(collision_record);
            }
        }
    }

    if (down_collision) {
        collision_records.push_back(down_collision_with_least_player_exceed);
    }

    return collision_records;
}


std::vector<Collision_Record> Level_Screen::collision_test(const Vector2D& player_position) {
    std::vector<Collision_Record> collision_records;

    auto ground_collision = ground_collision_test(player_position);
    auto entity_collision = entity_collision_test(player_position);

    collision_records.insert(collision_records.end(), ground_collision.begin(), ground_collision.end());
    collision_records.insert(collision_records.end(), entity_collision.begin(), entity_collision.end());

    return collision_records;
}

void Level_Screen::Render(Game& game) {
    SDL_Rect background;
    background.x = index*320 - game.screen_left_position_x;
    background.y = 0;
    background.w = 320;
    background.h = 280;

    SDL_SetRenderDrawColor(game.sdl_renderer, 255, 255, 255, 255);
    SDL_RenderFillRect(game.sdl_renderer, &background);

    for (int i=0; i<16; i++) {
        SDL_Rect ground_tile;

        ground_tile.w = 20;
        ground_tile.h = ground_level[i] * 20;
        ground_tile.x = background.x + i*20;
        ground_tile.y = 280 - ground_tile.h;

        SDL_SetRenderDrawColor(game.sdl_renderer, 69, 69, 69, 255);
        SDL_RenderFillRect(game.sdl_renderer, &ground_tile);
    }

    for (const Entity* entity : entities) {
        entity->Render(game, index);
    }
}