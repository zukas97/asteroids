#include "actions.h"

void destroy(SDL_Window *win, SDL_Renderer *rend, SDL_Texture *asteroid_texture, SDL_Texture *rocket_texture) {
	SDL_DestroyTexture(asteroid_texture);
	SDL_DestroyTexture(rocket_texture);
	SDL_DestroyRenderer(rend);
	SDL_DestroyWindow(win);
	SDL_Quit();
}
int Init_Win(SDL_Window **win, SDL_Renderer** rend) {
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		fprintf(stderr, "Error initalizing SDL\n");
		return false;
	}

	*win = SDL_CreateWindow(
		"Asteriods",
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		WIN_WIDTH,
		WIN_HEIGHT,
		0
			);

	if (!win) {
		fprintf(stderr, "Error initalizing window\n");
		return false;
	}

	*rend = SDL_CreateRenderer(*win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (!rend) {
		fprintf(stderr, "Error initalizing renderer\n");
		printf("Error initalizing renderer\n");
		return false;
	}

	if (!IMG_Init(IMG_INIT_PNG)) {
		fprintf(stderr, "Error initalizing SDL_image\n");
		return false;
	}
	return true;
}
