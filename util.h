#ifndef UTIL_H
#define UTIL_H

#include <SDL2/SDL.h>

class Vector2D {
public:
	int x;
	int y;
};

enum Key {KEY_RETURN, KEY_UP, KEY_DOWN, KEY_RIGHT, KEY_LEFT};

inline void Handle_Key_Down(SDL_Keycode key, bool (&key_array)[128]) {
    if (key == SDLK_RETURN)
        key_array[KEY_RETURN] = true;
    else if (key == SDLK_UP)
        key_array[KEY_UP] = true;
    else if (key == SDLK_DOWN)
        key_array[KEY_DOWN] = true;
    else if (key == SDLK_RIGHT)
        key_array[KEY_RIGHT] = true;
    else if (key == SDLK_LEFT)
        key_array[KEY_LEFT] = true;
}

inline void Handle_Key_Up(SDL_Keycode key, bool (&key_array)[128]) {
    if (key == SDLK_RETURN)
        key_array[KEY_RETURN] = false;
    else if (key == SDLK_UP)
        key_array[KEY_UP] = false;
    else if (key == SDLK_DOWN)
        key_array[KEY_DOWN] = false;
    else if (key == SDLK_RIGHT)
        key_array[KEY_RIGHT] = false;
    else if (key == SDLK_LEFT)
        key_array[KEY_LEFT] = false;
}

#endif