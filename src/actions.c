#include "actions.h"

void destroy(SDL_Window *win, SDL_Renderer *rend, SDL_Texture *asteroid_texture, SDL_Texture *rocket_texture) {
	SDL_DestroyTexture(asteroid_texture);
	SDL_DestroyTexture(rocket_texture);
	SDL_DestroyRenderer(rend);
	SDL_DestroyWindow(win);
	SDL_Quit();
}

