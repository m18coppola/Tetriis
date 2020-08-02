#include <stdio.h>
#include <SDL.h>

/* macros */
#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480
#define BOARD_WIDTH 10
#define BOARD_HEIGHT 20
#define BOARD_DIVISIONS 1
#define MONOMINO_SCALE 1.0/23.0

/* function declarations */
int init();
void quit();
void gameLoop();
void handleInput(int* gameState);

/* variables */
SDL_Window* window;
SDL_Renderer* renderer;
int board[BOARD_HEIGHT][BOARD_WIDTH];

typedef struct {
	int r;
	int g;
	int b;
	int a;
} color;

/* 0 i l j o s t z */
color colors[8] = {{0xFF, 0xFF, 0xFF, 0xFF},
                   {0x00, 0xF2, 0xF5, 0xFF},
                   {0x00, 0x05, 0xEF, 0xFF},
                   {0xFF, 0x9C, 0x00, 0xFF},
                   {0xF1, 0xE6, 0x1F, 0xFF},
                   {0x00, 0xF4, 0x3A, 0xFF},
                   {0xB1, 0x00, 0xBB, 0xFF},
                   {0xFF, 0x00, 0x00, 0xFF},
};

int
init()
{
	/* init SDL2 */
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
		return -1;
	}

	/* create SDL2 window */
	window = NULL;
	window = SDL_CreateWindow("Tetriis", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	if (window == NULL) {
		printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
		return -1;
	}
	/* create renderer */
	renderer = NULL;
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	if (renderer == NULL) {
		printf( "Renderer could not be created! SDL Error: %s\n", SDL_GetError() );
		return -1;
	}

	return 0;
}

void
quit()
{
	SDL_DestroyWindow(window);
	window = NULL;
	SDL_Quit();
}

void
handleInput(int* gameState)
{
	SDL_Event e;
	
	if (SDL_PollEvent(&e) != 0) {
		if (e.type == SDL_QUIT) {
			*gameState = 0;
		}

		if (e.type == SDL_KEYDOWN) {
			switch (e.key.keysym.sym){
				case SDLK_q:
				*gameState = 0;
				break;
			}
		}
	}
}

void
render()
{
	SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
	SDL_RenderClear(renderer);

	/* render grid */
	SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF);
	int size = (int)((float)SCREEN_HEIGHT*MONOMINO_SCALE);
	int x = SCREEN_WIDTH/2 - (BOARD_WIDTH * size)/2;
	int y = SCREEN_HEIGHT;
	for (int i = 0; i <= BOARD_WIDTH; i++) {
		//x,y,x,y
		SDL_RenderDrawLine(renderer, x, SCREEN_HEIGHT, x, SCREEN_HEIGHT - (size * BOARD_HEIGHT));
		x += size;
	}
	for(int i = 0; i <= BOARD_HEIGHT; i++){
		SDL_RenderDrawLine(renderer, SCREEN_WIDTH/2 - (BOARD_WIDTH * size)/2, y, SCREEN_WIDTH/2 + (BOARD_WIDTH * size)/2, y);
		y -= size;
	}



	SDL_RenderPresent(renderer);
}

void
gameLoop()
{
	int running = 1;

	while (running) {
		handleInput(&running);
		render();
	}
}

int
main (int argc, char* argv[])
{
	if (init() < 0) {
		printf("Tetriis has failed to load!\n");
		return -1;
	}

	gameLoop();

	quit();

	return 0;
}
