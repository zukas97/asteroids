#include <SDL2/SDL.h>

#define WIN_WIDTH 800
#define WIN_HEIGHT 700

#define FPS 60
#define FRAME_TIME (1000/FPS)

#define MAX_BULLETS 10

typedef struct {
	SDL_Rect Rect;
	int vel;
	bool onscreen;
} sprite_t;
