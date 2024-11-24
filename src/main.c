#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdbool.h>
#include <time.h>
#include "defs.h"
#include "actions.h"


//Globals
SDL_Renderer *rend = NULL;

int running = false;
int last_frame_time;
float dtime;
bool gameover = false;
bool started = false;

int score;


SDL_Color white = {255, 255, 255};


void setup();

//Sprites
struct Rocket {
	SDL_Rect rect;
	int vel;
	int left;
	int right;
	bool added;
} rocket;

struct Asteroid {
	SDL_Rect rect;
	int vel;
	bool added;
} asteroid;

struct Background {
	SDL_Rect rect;
	
} background;

typedef struct {
	SDL_Rect rect;
	int vel;
} Bullet;

Bullet bullet;



int Init_Win(SDL_Window *win) {
	//win = NULL;
	//rend = NULL;
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
		printf("Error initalizing renderer\n");
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
		if (SDL_PollEvent(&event)){
			switch (event.type) {
				case SDL_QUIT:
					running = false;
					break;
				case SDL_KEYUP:
					if (event.key.keysym.sym == SDLK_LEFT) {
						rocket.left = 0;
					}
					else if (event.key.keysym.sym == SDLK_RIGHT){
						rocket.right = 0;
					}
					break;


				case SDL_KEYDOWN:
					switch (event.key.keysym.sym) {
						case SDLK_ESCAPE:
							running = false;
							break;
						
						case SDLK_LEFT:
							rocket.left = 1;
							rocket.right = 0;
							break;
						case SDLK_RIGHT:
							rocket.right = 1;
							rocket.left = 0;
							break;
						case SDLK_SPACE:
							bullet.rect.x = rocket.rect.x + (rocket.rect.w/2);
							bullet.rect.y = rocket.rect.y;
							break;
						case SDLK_RETURN:
							if (!started) {
								started = true;
							}
							else if (gameover == true) {
								gameover = false;
							}
					break;		break;
											


					
					}
			}
		}
}

void summon_asteroid() {
	int rand_x;
	srand(time(NULL));
	rand_x = rand() % 700;
	asteroid.rect.x = rand_x;
	asteroid.rect.y = ASTEROID_START;
}


void render(SDL_Surface *rocket_surface, SDL_Texture *rocket_texture, SDL_Surface *asteroid_surface, SDL_Texture *asteroid_texture, SDL_Surface* start_surface, SDL_Texture* start_texture) {

	if (started == true) {
		if (gameover == false) {
		
			if (SDL_HasIntersection(&rocket.rect, &asteroid.rect)) {
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
			
			SDL_RenderFillRect(rend, &bullet.rect);
			
			
			SDL_RenderCopy(rend, rocket_texture, NULL, &rocket.rect);
			SDL_RenderCopy(rend, asteroid_texture, NULL, &asteroid.rect);
			
			SDL_RenderPresent(rend); 
			SDL_RenderClear(rend);
			SDL_DestroyTexture(asteroid_texture);
			SDL_DestroyTexture(rocket_texture);
			
			if (SDL_HasIntersection(&bullet.rect, &asteroid.rect)) {
				bullet.rect.y = -10;
				asteroid.rect.y = ASTEROID_START;
				summon_asteroid();
				score += 1;
			}
		}
		else if (gameover) {
			SDL_RenderClear(rend);
			SDL_Surface * background_surface = IMG_Load("./images/gameover.png");
			SDL_Texture* background_texture = SDL_CreateTextureFromSurface(rend, background_surface);
			SDL_FreeSurface(background_surface);
			SDL_RenderCopy(rend, background_texture, NULL, &background.rect);
			SDL_RenderPresent(rend);
			SDL_DestroyTexture(background_texture);


		}
	}
	else if (!started) {
		SDL_Surface * background_surface = IMG_Load("./images/home.png");
		SDL_Texture* background_texture = SDL_CreateTextureFromSurface(rend, background_surface);
		SDL_FreeSurface(background_surface);
		SDL_RenderCopy(rend, background_texture, NULL, &background.rect);
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
	if (started == true) {
		if (gameover == false) {
			asteroid.rect.y += asteroid.vel * dtime;

			if (asteroid.rect.y >= WIN_HEIGHT) {
				gameover = true;
			}

			

			if (rocket.left == 1) {
				
				if (rocket.rect.x >= 10) {
					rocket.rect.x -= rocket.vel * dtime;
				}
			}
			if (rocket.right == 1) {
				if (rocket.rect.x <= WIN_WIDTH - 55) {
					rocket.rect.x += rocket.vel * dtime;
				}
				
			}

			//bullet.x = rocket.x;
			bullet.rect.y -= bullet.vel * dtime;

			if (asteroid.added == true && rocket.added == true && score % 5 != 0) {
				rocket.added = false;
				asteroid.added = false;
			}

			if (asteroid.added == false && score != 0 && score % 5 == 0) {
				asteroid.vel += 30;
				asteroid.added = true;
				
			}

			if (rocket.added == false && score != 0 && score % 10 == 0) {
				rocket.vel += 5;
				rocket.added = true;
			}

		
		
	}

	if (gameover) {
		setup();

	}

	}

	
}


void setup() {
	rocket.rect.x = (WIN_WIDTH / 2) - 20;
	rocket.rect.y = 620;
	rocket.rect.w = 46;
	rocket.rect.h = 82;
	asteroid.rect.w= 50;
	asteroid.rect.h= 50;
	background.rect.x = 0;
	background.rect.y = 0;
	background.rect.w= WIN_WIDTH;
	background.rect.h= WIN_HEIGHT;
	bullet.rect.w = 5;
	bullet.rect.h= 10;
	bullet.rect.y = -10;
	asteroid.vel = 130;
	rocket.vel = 400;
	score = 0;
	rocket.right = 0;
	rocket.left = 0;
	bullet.vel = 700;
	rocket.added = false;
	asteroid.added = false;
	summon_asteroid();

}
int main() {
	SDL_Surface *rocket_surface;
	SDL_Texture *rocket_texture;
	SDL_Surface *asteroid_surface;
	SDL_Texture *asteroid_texture;
	SDL_Surface *start_surface;
	SDL_Texture *start_texture;
	SDL_Window *win = NULL;
	running = Init_Win(win);
	
	setup();

	while (running) {
		input();
		update();
		render(rocket_surface, rocket_texture, asteroid_surface, asteroid_texture, start_surface, start_texture);
	}
	destroy(win, rend, asteroid_texture, rocket_texture);
	return 0;
}
