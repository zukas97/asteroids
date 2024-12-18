#include <SDL2/SDL_render.h>
#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdbool.h>
#include <time.h>
#include "defs.h"
#include "actions.h"
#include "sprites.h"

//Globals

int running = false;
int last_frame_time;
float dtime;
bool gameover = false;
bool started = false;

int score;

Asteroid asteroid;
Background background;
Rocket rocket;
Bullet bullet[MAX_BULLETS];


SDL_Color white = {255, 255, 255};

void setup();



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
							for (int i=0; i <= MAX_BULLETS; i++) {
								if (!bullet[i].onscreen) {
									bullet[i].rect.x = rocket.rect.x + (rocket.rect.w/2);
									bullet[i].rect.y = rocket.rect.y;
									bullet[i].onscreen = true;
									break;
								}
							}
						case SDLK_RETURN:
							if (event.key.keysym.sym != SDLK_SPACE) {
								if (!started) {
									started = true;
								}
								else if (gameover) {
									gameover = false;
								}
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



void render(SDL_Renderer* rend, SDL_Surface* start_surface, SDL_Texture* start_texture) {

	if (started == true) {
		if (gameover == false) {
		
			if (SDL_HasIntersection(&rocket.rect, &asteroid.rect)) {
				gameover = true;
			}

			SDL_SetRenderDrawColor(rend, 0, 0, 0, 255);
			SDL_RenderClear(rend);
			


			
			SDL_SetRenderDrawColor(rend, 255, 255, 255, 255);
			
			for (int i = 0; i < MAX_BULLETS; i++) {
				if (bullet[i].onscreen) {
					SDL_RenderFillRect(rend, &bullet[i].rect);
				}
			}
			
			
			SDL_RenderCopy(rend, rocket.texture, NULL, &rocket.rect);
			SDL_RenderCopy(rend, asteroid.texture, NULL, &asteroid.rect);
			
			SDL_RenderPresent(rend); 
			SDL_RenderClear(rend);
			
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
			for (int i = 0; i < MAX_BULLETS; i++) {
				if (bullet[i].onscreen) {
					bullet[i].rect.y -= bullet[i].vel * dtime;
				}
				if (bullet[i].rect.y <= 0 - bullet[i].rect.h) {
					bullet[i].onscreen = false;
				}
				if (SDL_HasIntersection(&bullet[i].rect, &asteroid.rect) && bullet[i].onscreen) {
					bullet[i].onscreen = false;
					summon_asteroid();
					score += 1;
					break;
				}
			}

			if (asteroid.added && rocket.added && score % 5 != 0) {
				asteroid.vel += ASTEROID_INCRAMENT;
				rocket.added = false;
				asteroid.added = false;
			}

			if (!asteroid.added && score != 0 && score % 5 == 0) {
				asteroid.vel += ASTEROID_INCRAMENT;
				asteroid.added = true;
				
			}

			if (rocket.added == false && score != 0 && score % 10 == 0) {
				rocket.vel += 10;
				rocket.added = true;
			}

		
		
	}

	if (gameover) {
		setup();

	}

	}

	
}


void setup(SDL_Renderer* rend) {
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
	asteroid.vel = 130;
	rocket.vel = 420;
	score = 0;
	rocket.right = 0;
	rocket.left = 0;
	rocket.added = false;
	asteroid.added = false;
	
	//rocket
	SDL_Surface* surface = IMG_Load("./images/rocket.bmp");
	rocket.texture = SDL_CreateTextureFromSurface(rend, surface);
	SDL_FreeSurface(surface);
	SDL_QueryTexture(rocket.texture, NULL, NULL, &rocket.rect.w, &rocket.rect.h);
	rocket.rect.w /= 20;
	rocket.rect.h /= 20;
	
	//asteroid
	surface = IMG_Load("./images/asteroid.bmp");
	asteroid.texture = SDL_CreateTextureFromSurface(rend, surface);
	SDL_FreeSurface(surface);
	SDL_QueryTexture(asteroid.texture, NULL, NULL, &asteroid.rect.w, &asteroid.rect.h);
	asteroid.rect.w /= 12;
	asteroid.rect.h /= 12;



	for (int i = 0; i < MAX_BULLETS; i++) {
		bullet[i].rect.w = 5;
		bullet[i].rect.h= 10;
		bullet[i].rect.y = -10;
		bullet[i].vel = 800;
	}
	summon_asteroid();

}
int main() {
	SDL_Surface *start_surface;
	SDL_Texture *start_texture;
	SDL_Window *win = NULL;
	SDL_Renderer *rend = NULL;
	running = Init_Win(&win, &rend);
	
	setup(rend);

	while (running) {
		input();
		update();
		render(rend, start_surface, start_texture);
	}
	destroy(win, rend, asteroid.texture, rocket.texture);
	return 0;
}
