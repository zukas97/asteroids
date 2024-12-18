#ifndef SPRITES_H
#define SPRITES_H 
#include <SDL2/SDL.h>
typedef struct Rocket {
	SDL_Rect rect;
	int vel;
	int left;
	int right;
	bool added;
	SDL_Texture* texture;
} Rocket;

typedef struct Asteroid {
	SDL_Rect rect;
	int vel;
	bool added;
	SDL_Texture* texture;
} Asteroid;

typedef struct Background {
	SDL_Rect rect;
	
} Background;

typedef struct Bullet {
	SDL_Rect rect;
	int vel;
	bool onscreen;
} Bullet;
#endif
