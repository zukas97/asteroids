#include <SDL2/SDL_events.h>
#include <SDL2/SDL_render.h>
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
float dtime;
bool gameover;

int score;

SDL_Surface *rocket_surface;
SDL_Texture *rocket_texture;

SDL_Surface *asteroid_surface;
SDL_Texture *asteroid_texture;

SDL_Surface *start_surface;
SDL_Texture *start_texture;

SDL_Color white = {255, 255, 255};

pthread_t input_thread;

void setup();

//Sprites
struct Rocket {
	int x;
	int y;
	int width;
	int height;
	int vel;
} rocket;

struct Asteroid {
	int x;
	int y;
	int width;
	int height;
	int vel;
} asteroid;

struct Background {
	int x;
	int y;
	int width;
	int height;
	
} gameover_screen;

struct Bullet {
	int x;
	int y;
	int width;
	int height;
} bullet;



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

	rend = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);

	if (!rend) {
		fprintf(stderr, "Error initalizing renderer\n");
		return false;
	}

	if (!IMG_Init(IMG_INIT_PNG)) {
		fprintf(stderr, "Error initalizing SDL_image\n");
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
							if (rocket.x >= 10) {
								rocket.x -= 500 * dtime;
							}
							break;
						case SDLK_RIGHT:
							if (rocket.x <= WIN_WIDTH - 55) {
								rocket.x += rocket.vel * dtime;
							}
						
							break;
						case SDLK_SPACE:
							bullet.x = rocket.x + (rocket.width/2);
							bullet.y = rocket.y;
							break;
						case SDLK_RETURN:
							if (gameover == true) {
								gameover = false;
							}

							


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


void render() {
	SDL_Rect Rocket = {
		rocket.x,
		rocket.y,
		rocket.width,
		rocket.height,
	};

	SDL_Rect Background = {
		gameover_screen.x,
		gameover_screen.y,
		gameover_screen.width,
		gameover_screen.height,
	};

	SDL_Rect Asteroid = {
		asteroid.x,
		asteroid.y,
		asteroid.width,
		asteroid.height,
	};

	SDL_Rect Bullet = {
		bullet.x,
		bullet.y,
		bullet.width,
		bullet.height,
	};



	if (gameover == false) {
	
		if (SDL_HasIntersection(&Rocket, &Asteroid)) {
			gameover = true;
		}

		SDL_SetRenderDrawColor(rend, 0, 0, 0, 255);
		SDL_RenderClear(rend);
		
		rocket_surface = IMG_Load("./images/rocket.bmp");
		rocket_texture = SDL_CreateTextureFromSurface(rend, rocket_surface);
		SDL_FreeSurface(rocket_surface);

		asteroid_surface = IMG_Load("./images/asteroid.bmp");
		asteroid_texture = SDL_CreateTextureFromSurface(rend, asteroid_surface);
		SDL_FreeSurface(asteroid_surface);
		
		SDL_SetRenderDrawColor(rend, 255, 255, 255, 255);
		
		SDL_RenderFillRect(rend, &Bullet);
		
		
		SDL_RenderCopy(rend, rocket_texture, NULL, &Rocket);
		SDL_RenderCopy(rend, asteroid_texture, NULL, &Asteroid);
		
		SDL_RenderPresent(rend); 
		SDL_RenderClear(rend);
		SDL_DestroyTexture(asteroid_texture);
		SDL_DestroyTexture(rocket_texture);
		
		if (SDL_HasIntersection(&Bullet, &Asteroid)) {
			asteroid.y = -50;
			bullet.y = -10;
			summon_asteroid();
			score += 1;
		}
	}
	else if (gameover == true) {
		//pthread_join(input_thread, NULL);
		
		SDL_RenderClear(rend);
		SDL_Surface * background_surface = IMG_Load("./images/gameover.png");
		SDL_Texture* background_texture = SDL_CreateTextureFromSurface(rend, background_surface);
		SDL_FreeSurface(background_surface);
		SDL_RenderCopy(rend, background_texture, NULL, &Background);
		SDL_RenderPresent(rend);
		SDL_DestroyTexture(background_texture);


	}
	
}

void update() {
	int wait_time = FRAME_TIME - (SDL_GetTicks() - last_frame_time);

	if (wait_time > 0 && wait_time <= FRAME_TIME) {
		SDL_Delay(wait_time);
	}

	dtime = (SDL_GetTicks() - last_frame_time) / 1000.0f;

	last_frame_time = SDL_GetTicks();
	if (gameover == false) {
		asteroid.y += asteroid.vel * dtime;

		if (asteroid.y >= WIN_HEIGHT) {
			gameover = true;
		}

		//bullet.x = rocket.x;
		bullet.y -= 500 * dtime;

		if (score < 5) {
			asteroid.vel = 100;
		}
		else if (score >= 5 && score < 10) {
			asteroid.vel = 200;
			rocket.vel = 915;
		}
		else if (score >= 10 && score < 15) {
			asteroid.vel = 250;
			rocket.vel = 925;
		}
		else if (score >= 15 && score < 20) {
			asteroid.vel = 275;
			rocket.vel = 945;
		}
		else if (score >= 20 && score < 25) {
			asteroid.vel = 300;
			rocket.vel = 965;
		}
		
	}

	if (gameover == true) {
		setup();

	}

	
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
	//asteroid.x = 100;
	//asteroid.y = -100;
	asteroid.width = 50;
	asteroid.height = 50;
	gameover_screen.x = 0;
	gameover_screen.y = 0;
	gameover_screen.width = WIN_WIDTH;
	gameover_screen.height = WIN_HEIGHT;
	bullet.width = 5;
	bullet.height = 10;
	bullet.y = -10;
	asteroid.vel = 100;
	rocket.vel = 900;
	score = 0;
	summon_asteroid();

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
