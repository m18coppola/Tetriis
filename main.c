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
void handleInput(int* gameState);
void render();
void gameLoop();
void newTetromino(int selection);
void moveTetromino(int dir);

/* variables */
int currentSelection = 0;
SDL_Window* window;
SDL_Renderer* renderer;
int board[BOARD_HEIGHT][BOARD_WIDTH];

typedef struct {
	int x;
	int y;
} vec2;

const vec2 startingPos = {4, 0};

vec2 fallingTetromino[4];

/*
const int tetrominos[7][4] =
{
	{1,3,5,7}, //I
	{2,4,5,7}, //Z
	{3,5,4,6}, //S
	{3,5,4,7}, //T
	{2,3,5,7}, //L
	{3,5,7,6}, //J
	{2,3,4,5}  //O
};
*/

const vec2 tetrominos[7][4] = 
{
	{ {3, 0}, {4, 0}, {5, 0}, {6, 0} }, //I
	{ {4, 0}, {5, 0}, {5, 1}, {6, 1} }, //Z
	{ {4, 1}, {5, 1}, {5, 0}, {6, 0} }, //S
	{ {4, 1}, {5, 1}, {5, 0}, {6, 1} }, //T
	{ {4, 0}, {4, 1}, {5, 0}, {6, 0} }, //L
	{ {4, 0}, {4, 1}, {5, 1}, {6, 1} }, //J
	{ {4, 0}, {4, 1}, {5, 0}, {5, 1} }, //O
};

typedef struct {
	int r;
	int g;
	int b;
	int a;
} color;

/* 0 i l j o s t z */
const color colors[8] = {{0x00, 0x00, 0x00, 0xFF}, //empty
                   {0x00, 0xF2, 0xF5, 0xFF}, //I
                   {0xFF, 0x00, 0x00, 0xFF}, //Z
                   {0x00, 0xF4, 0x3A, 0xFF}, //S
                   {0xB1, 0x00, 0xBB, 0xFF}, //T
                   {0x00, 0x05, 0xEF, 0xFF}, //L
                   {0xFF, 0x9C, 0x00, 0xFF}, //J
                   {0xF1, 0xE6, 0x1F, 0xFF}, //O
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
	window = SDL_CreateWindow("The Game!", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
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

				case SDLK_0:
				currentSelection++;
				if (currentSelection > 6) {
					currentSelection = 0;
				}
				newTetromino(currentSelection);
				break;

				case SDLK_a:
				moveTetromino(-1);
				break;

				case SDLK_d:
				moveTetromino(1);
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

	SDL_Rect square = {0, 0, size, size};

	for(int i = 0; i < BOARD_WIDTH; i++){
		y = SCREEN_HEIGHT;
		for(int j = 0; j < BOARD_HEIGHT; j++){
			SDL_SetRenderDrawColor(renderer, colors[board[i][j]].r, colors[board[i][j]].g, colors[board[i][j]].b, colors[board[i][j]].a);
			square.x = x;
			square.y = y;
			SDL_RenderFillRect(renderer, &square);
			y -= size;
		}
		x += size;
	}

	/* draw tetromino */
	SDL_SetRenderDrawColor(renderer, colors[currentSelection + 1].r, colors[currentSelection + 1].g, colors[currentSelection + 1].b, colors[currentSelection + 1].a);
	for(int i = 0; i < 4; i++){
		square.x = (SCREEN_WIDTH/2 - (BOARD_WIDTH * size)/2) + size * fallingTetromino[i].x;
		square.y = SCREEN_HEIGHT - BOARD_HEIGHT * size + fallingTetromino[i].y * size;
		SDL_RenderFillRect(renderer, &square);
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

void
newTetromino(int selection)
{
	/*
	for(int i = 0; i < 4; i++){
		fallingTetromino[i].x = tetrominos[selection][i] % 2 + startingPos.x;
		fallingTetromino[i].y = tetrominos[selection][i] / 2 + startingPos.y;
	}
	*/
	for(int i = 0; i < 4; i++){
		fallingTetromino[i].x = tetrominos[selection][i].x;
		fallingTetromino[i].y = tetrominos[selection][i].y;
	}
}

void
moveTetromino(int dir)
{
	int outOfBounds = 0;
	for(int i = 0; i < 4; i++){
		fallingTetromino[i].x += dir;
		if (fallingTetromino[i].x >= BOARD_WIDTH || fallingTetromino[i].x < 0) {
			outOfBounds = 1;
		}
	}
	if (outOfBounds) {
		for(int i = 0; i < 4; i++){
			fallingTetromino[i].x -= dir;
		}
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
