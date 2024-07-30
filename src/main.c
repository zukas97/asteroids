//#include <stdlib.h>
#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdbool.h>
#include <time.h>
#include "defs.h"


//Globals
SDL_Window *win = NULL;
SDL_Renderer *rend = NULL;

int running = false;
int last_frame_time;


SDL_Surface *rocket_surface;
SDL_Texture *rocket_texture;

SDL_Surface *asteroid_surface;
SDL_Texture *asteroid_texture;

//Sprites
struct Rocket {
	int x;
	int y;
	int width;
	int height;
	int vel;
	bool left;
	bool right;

} rocket;

struct Asteroid {
	int x;
	int y;
	int width;
	int height;
} asteroid;



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
	//int is_pressed = event.key.state == SDL_PRESSED;
	if (SDL_PollEvent(&event)){
		switch (event.type) {
			case SDL_QUIT:
				running = false;
				break;
			case SDL_KEYDOWN:
				switch (event.key.keysym.sym) {
					case SDLK_ESCAPE:
						running = false;
						break;
					
					case SDLK_LEFT:
						if (rocket.x >= 10){
							rocket.x -= 4;
						}
						break;
					case SDLK_RIGHT:
						if (rocket.x <= WIN_WIDTH - 55){
							rocket.x += 4;
						}
						break;
						


				break;
				}
		}
	}
}

void summon_asteroid() {
	int rand_y;
	srand(time(NULL));
	rand_y = rand() % 700;
	asteroid.x = rand_y;
	asteroid.y = -50;
}

void update() {
	int wait_time = FRAME_TIME - (SDL_GetTicks() - last_frame_time);

	if (wait_time > 0 && wait_time <= FRAME_TIME) {
		SDL_Delay(wait_time);
	}

	float dtime = (SDL_GetTicks() - last_frame_time) / 1000.0f;

	last_frame_time = SDL_GetTicks();

	asteroid.y += 5;

	if (asteroid.y >= WIN_HEIGHT) {
		summon_asteroid();
	}

}

void render() {


	
	SDL_SetRenderDrawColor(rend, 0, 0, 0, 255);
	SDL_RenderClear(rend);
	
	rocket_surface = IMG_Load("./images/rocket.bmp");
	rocket_texture = SDL_CreateTextureFromSurface(rend, rocket_surface);
	SDL_FreeSurface(rocket_surface);

	asteroid_surface = IMG_Load("./images/asteroid.bmp");
	asteroid_texture = SDL_CreateTextureFromSurface(rend, asteroid_surface);
	SDL_FreeSurface(asteroid_surface);
	
	
	SDL_Rect Rocket = {
		rocket.x,
		rocket.y,
		rocket.width,
		rocket.height,
	};

	SDL_Rect Asteroid = {
		asteroid.x,
		asteroid.y,
		asteroid.width,
		asteroid.height,
	};

	
	SDL_RenderCopy(rend, rocket_texture, NULL, &Rocket);
	SDL_RenderCopy(rend, asteroid_texture, NULL, &Asteroid);
	
	SDL_RenderPresent(rend); 
	SDL_RenderClear(rend);
	SDL_DestroyTexture(asteroid_texture);
	SDL_DestroyTexture(rocket_texture);

}

void destroy() {
	SDL_DestroyTexture(asteroid_texture);
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
	rocket.vel = 0;
	asteroid.x = 100;
	asteroid.y = 100;
	asteroid.width = 50;
	asteroid.height = 50;
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

