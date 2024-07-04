//#include <stdlib.h>
#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdbool.h>
#include "defs.h"
//#include "sprites.h"

SDL_Window *win = NULL;
SDL_Renderer *rend = NULL;

int running = false;
int last_frame_time;


SDL_Surface *rocket_surface;
SDL_Texture *rocket_texture;


struct Rocket {
	int x;
	int y;
	int width;
	int height;

} rocket;

int w,h;

//extern Rocket;

int Init_Win(void) {
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		fprintf(stderr, "Error initalizing SDL\n");
		return false;
	}

	win = SDL_CreateWindow(
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

	rend = SDL_CreateRenderer(win, -1, 0);

	if (!rend) {
		fprintf(stderr, "Error initalizing renderer\n");
		return false;
	}

	return true;

}

void input() {
	SDL_Event event;
	if (SDL_WaitEvent(&event)){
		switch (event.type) {
			case SDL_QUIT:
				running = false;
				break;
			case SDL_KEYDOWN:
				switch (event.key.keysym.sym) {
					case SDLK_ESCAPE:
						running = false;
						break;
				break;
				}
		}
	}
}

void update() {
	int wait_time = FRAME_TIME - (SDL_GetTicks() - last_frame_time);

	if (wait_time > 0 && wait_time <= FRAME_TIME) {
		SDL_Delay(wait_time);
	}

	//last_frame_time = SDL_GetTicks();
}

void render() {


	SDL_SetRenderDrawColor(rend, 0, 0, 0, 255);
	SDL_RenderClear(rend);
	
	rocket_surface = IMG_Load("/home/zukas/C/sdl/asteroids//images/rocket.bmp");
	rocket_texture = SDL_CreateTextureFromSurface(rend, rocket_surface);
	SDL_FreeSurface(rocket_surface);
	

	SDL_Rect Rocket = {
		rocket.x,
		rocket.y,
		rocket.width,
		rocket.height,
	};

	SDL_QueryTexture(rocket_texture, NULL, NULL, &w, &h);
	SDL_RenderCopy(rend, rocket_texture, NULL, &Rocket);
	SDL_RenderPresent(rend); 

}

void destroy() {
	SDL_DestroyTexture(rocket_texture);
	SDL_DestroyRenderer(rend);
	SDL_DestroyWindow(win);
	SDL_Quit();
}

void setup() {
	rocket.x = (WIN_WIDTH / 2) - 20;
	rocket.y = 620;
	rocket.width = 46;
	rocket.height = 82;
}

int main() {
	running = Init_Win();
	
	setup();

	while (running) {
		input();
		update();
		render();
	}
	destroy();
	return 0;
}

