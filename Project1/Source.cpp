#include <SDL.h>
#include <stdio.h>
#include <SDL_image.h>
#include <string>
#include <SDL_ttf.h>
#include <sstream>
#include "Bird.h"
#include "LTexture.h"

//** Based on Tutorials by Lazy Foo' production **//

//Screen dimension constants
const int SCREEN_WIDTH = 538;
const int SCREEN_HEIGHT = 480;


//Starts up SDL and creates window
bool init();

//Loads media
bool loadMedia();

//Frees media and shuts down SDL
void close();

//Box collision detector
bool checkCollision(SDL_Rect bird, SDL_Rect pipe);


#ifdef _SDL_TTF_H
//Creates image from font string
bool loadFromRenderedText(std::string textureText, SDL_Color textColor);
#endif


bool gameOver = false;
bool gameReset = false;

//The window we'll be rendering to
SDL_Window* gWindow = NULL;

//The window renderer
SDL_Renderer* gRenderer = NULL;

//Scene textures
LTexture gbirdTexture;
LTexture board;
LTexture endgame;

TTF_Font *gFont = NULL;
//Rendered texture
LTexture gTextTexture;

bool init()
{
	//Initialization flag
	bool success = true;

	//Initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
		success = false;
	}
	else
	{
		//Set texture filtering to linear
		if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1"))
		{
			printf("Warning: Linear texture filtering not enabled!");
		}

		//Create window
		gWindow = SDL_CreateWindow("Flappy Birdchu!", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
		if (gWindow == NULL)
		{
			printf("Window could not be created! SDL Error: %s\n", SDL_GetError());
			success = false;
		}
		else
		{
			//Create vsynced renderer for window
			gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

			if (gRenderer == NULL)
			{
				printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
				success = false;
			}
			else
			{
				//Initialize renderer color
				SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);

				//Initialize PNG loading
				int imgFlags = IMG_INIT_PNG;
				if (!(IMG_Init(imgFlags) & imgFlags))
				{
					printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
					success = false;
				}
			}
		}
	}

	return success;
}

#ifdef _SDL_TTF_H
bool LTexture::loadFromRenderedText(std::string textureText, SDL_Color textColor, SDL_Renderer* gRenderer)
{
	//Get rid of preexisting texture
	//free();
	//Render text surface
	SDL_Surface* textSurface = TTF_RenderText_Solid(gFont, textureText.c_str(), textColor);
	if (textSurface != NULL)
	{
		//Create texture from surface pixels
		mTexture = SDL_CreateTextureFromSurface(gRenderer, textSurface);
		if (mTexture == NULL)
		{
			printf("Unable to create texture from rendered text! SDL Error: %s\n", SDL_GetError());
		}
		else
		{
			//Get image dimensions
			mWidth = textSurface->w;
			mHeight = textSurface->h;
		}

		//Get rid of old surface
		SDL_FreeSurface(textSurface);
	}
	else
	{
		printf("Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError());
	}

	//Return success
	return mTexture != NULL;
}
#endif

bool loadMedia()
{
	bool success = true;

	if (!endgame.loadFromFile("imgs/endgame.bmp", gRenderer))
	{
		printf("Failed to load endgame texture!\n");
		success = false;
	}

	if (!board.loadFromFile("imgs/board.bmp", gRenderer))
	{
		printf("Failed to load board texture!\n");
		success = false;
	}

	if (!gbirdTexture.loadFromFile("imgs/birdchu2.png", gRenderer))
	{
		printf("Failed to load bird texture!\n");
		success = false;
	}
	TTF_Init();

	gFont = TTF_OpenFont("lazy.ttf", 28);
	if (gFont == NULL)
	{
		printf("Failed to load lazy font! SDL_ttf Error: %s\n", TTF_GetError());
		success = false;
	}


	return success;
}

void close()
{
	//Free loaded images
	gbirdTexture.free();
	endgame.free();
	board.free();

	//Destroy window	
	SDL_DestroyRenderer(gRenderer);
	SDL_DestroyWindow(gWindow);
	gWindow = NULL;
	gRenderer = NULL;

	//Quit SDL subsystems
	IMG_Quit();
	SDL_Quit();
}

bool checkCollision(SDL_Rect bird, SDL_Rect pipe)
{
	int leftBirdSide = bird.x;
	int rightBirdSide = bird.x + bird.w;
	int topBirdSide = bird.y;
	int bottomBirdSide = bird.y + bird.h;
	int	leftPipeSide = pipe.x;
	int rightPipeSide = pipe.x + pipe.w;
	int	topPipeSide = pipe.y;
	int bottomPipeSide = pipe.y + pipe.h;

	return !(bottomBirdSide <= topPipeSide
		|| topBirdSide >= bottomPipeSide
		|| rightBirdSide <= leftPipeSide
		|| leftBirdSide >= rightPipeSide);
}


int main(int argc, char* args[])
{
	//Start up SDL and create window
	if (!init())
	{
		printf("Failed to initialize!\n");
	}
	else
	{
		//Load media
		if (!loadMedia())
		{
			printf("Failed to load media!\n");
		}
		else
		{
			//Main loop flag
			bool quit = false;

			//Event handler
			SDL_Event e;

			//The bird that will be moving around on the screen
			Bird bird;

			SDL_Rect upperPipe;
			SDL_Rect downPipe;
			//Init pipes
			upperPipe.x = 0;
			upperPipe.y = 0;
			upperPipe.w = 0;
			upperPipe.h = 0;

			downPipe.x = 0;
			downPipe.y = 0;
			downPipe.w = 0;
			downPipe.h = 0;
			int pipeCounter = 1;
			bool canCount = true;
			//While application is running
			while (!quit)
			{
				//Handle events on queue
				while (SDL_PollEvent(&e) != 0)
				{
					//User requests quit
					if (e.type == SDL_QUIT)
					{
						quit = true;
					}
					//Handle input for the bird
					bird.handleEvent(e);

				}

				if (!gameOver) {
					bird.move();

					if (upperPipe.x < bird.mPosX) {
						upperPipe.x -= 11;
						downPipe.x -= 11;
					}
					else {
						upperPipe.x -= 3;
						downPipe.x -= 3;
					}
					if (upperPipe.x < -60) {
						upperPipe.x = 538;
						upperPipe.y = 0;
						upperPipe.w = 60;
						downPipe.x = 538;
						downPipe.w = 60;
						upperPipe.h = 1 + (rand() % static_cast<int>(300 - 1 + 1));
						downPipe.y = upperPipe.h + 180;
						downPipe.h = 480 - downPipe.y;
						pipeCounter -= 1;
					}

					if (pipeCounter != 0 && pipeCounter % 3 == 0) {
 						upperPipe.x -= 3 - pipeCounter;
						downPipe.x -= 3 - pipeCounter;

					}

					if (bird.mPosY > 480) {
						gameOver = true;
					}
					if (gameReset) {
						bird.mPosY = 0;
						gameReset = false;
					}

					//If the Bird collided or went too far up or down
					if (checkCollision(bird.mCollider, upperPipe))
					{
						gameOver = true;
					}
					if (checkCollision(bird.mCollider, downPipe))
					{
						gameOver = true;
					}

				}
				//If end game clear texture and show GameOver
				if (gameOver) {
					gbirdTexture.free();
					board.free();
					endgame.render(0, 0, NULL, NULL, NULL, gRenderer);
					upperPipe.w = 0;
					upperPipe.h = 0;
					downPipe.w = 0;
					downPipe.h = 0;
					upperPipe.x = -1;

					//Render score text
					SDL_Color textColor = { 100, 25, 100 };
					if (!gTextTexture.loadFromRenderedText("Your score: " + std::to_string(abs(pipeCounter)), textColor, gRenderer))
					{
						printf("Failed to render text texture!\n");
					}


					gTextTexture.render(180, 300, NULL, NULL, NULL, gRenderer);

					//Update screen
					SDL_RenderPresent(gRenderer);

					if (e.type == SDL_KEYDOWN)
					{
						switch (e.key.keysym.sym)
						{
						case SDLK_RETURN:
							//Restart game
							gameOver = false;
							endgame.free();
							bird.reset();
							gTextTexture.free();
							gameReset = true;
							loadMedia();
							pipeCounter = 1;
							break;
						}
					}
				}

				//Render pipe color
				SDL_SetRenderDrawColor(gRenderer, 113, 54, 26, 0xFF);
				//Render board
				board.render(0, 0, NULL, NULL, NULL, gRenderer);
				//Render pipes
				SDL_RenderDrawRect(gRenderer, &upperPipe);
				SDL_RenderDrawRect(gRenderer, &downPipe);
				//If Bird Y position is above screen move bird to 0 point
				if (bird.mPosY < 0) {
					bird.mPosY = 0;
					bird.mCollider.y = 0;
				}
				gbirdTexture.render(bird.mPosX, bird.mPosY, NULL, NULL, NULL, gRenderer);
				SDL_RenderFillRect(gRenderer, &upperPipe);
				SDL_RenderFillRect(gRenderer, &downPipe);

				//Update screen
				SDL_RenderPresent(gRenderer);

			}
		}
	}

	//Free resources and close SDL
	close();

	return 0;
}