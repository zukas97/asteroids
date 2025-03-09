#ifndef ACTIONS_H
#define ACTIONS_H
#include <SDL2/SDL.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include "defs.h"
#include "sprites.h"

void destroy(SDL_Window *win, SDL_Renderer *rend, SDL_Texture *asteroid_texture, SDL_Texture *rocket_texture);
int Init_Win(SDL_Window** win, SDL_Renderer** rend, TTF_Font** font);
#endif
