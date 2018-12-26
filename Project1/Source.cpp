#include <SDL.h>
#include <stdio.h>
#include <SDL_image.h>
#include <string>
#include <SDL_ttf.h>
#include <sstream>
#include "Bird.h"
#include "LTexture.h"

//Screen dimension constants
const int SCREEN_WIDTH = 538;
const int SCREEN_HEIGHT = 480;


//Starts up SDL and creates window
bool init();

//Loads media
bool loadMedia();

//Frees media and shuts down SDL
void close();

void generatePipes(Bird bird, SDL_Rect upperPipe, SDL_Rect downPipe);

//Box collision detector
bool checkCollision(SDL_Rect a, SDL_Rect b);


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
	free();

	//Render text surface
	SDL_Surface* textSurface = TTF_RenderText_Solid(NULL, textureText.c_str(), textColor);
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
	//Loading success flag
	bool success = true;
	//Load press texture
	board.loadFromFile("imgs/board.bmp", gRenderer);
	endgame.loadFromFile("imgs/endgame.bmp", gRenderer);
	if (!gbirdTexture.loadFromFile("imgs/birdchu.bmp", gRenderer))
	{
		printf("Failed to load bird texture!\n");
		success = false;
	}

	return success;
}

void close()
{
	//Free loaded images
	gbirdTexture.free();

	//Destroy window	
	SDL_DestroyRenderer(gRenderer);
	SDL_DestroyWindow(gWindow);
	gWindow = NULL;
	gRenderer = NULL;

	//Quit SDL subsystems
	IMG_Quit();
	SDL_Quit();
}

bool checkCollision(SDL_Rect a, SDL_Rect b)
{
	//The sides of the rectangles
	int leftA, leftB;
	int rightA, rightB;
	int topA, topB;
	int bottomA, bottomB;

	//Calculate the sides of rect A
	leftA = a.x;
	rightA = a.x + a.w;
	topA = a.y;
	bottomA = a.y + a.h;

	//Calculate the sides of rect B
	leftB = b.x;
	rightB = b.x + b.w;
	topB = b.y;
	bottomB = b.y + b.h;

	//If any of the sides from A are outside of B
	if (bottomA <= topB)
	{
		return false;
	}

	if (topA >= bottomB)
	{
		return false;
	}

	if (rightA <= leftB)
	{
		return false;
	}

	if (leftA >= rightB)
	{
		return false;
	}

	//If none of the sides from A are outside B
	return true;
}

void generatePipes(Bird bird, SDL_Rect upperPipe, SDL_Rect downPipe) {
	

	if (upperPipe.x == 269) {
		

		upperPipe.x = 538;
		upperPipe.y = 0;
		upperPipe.w = 40;
		downPipe.x = 538;
		downPipe.w = 40;


		upperPipe.h = 1 + (rand() % static_cast<int>(330 - 1 + 1));
		downPipe.y = upperPipe.h + 150;
		downPipe.h = 480 - downPipe.y;

	}
	upperPipe.x -= 3;
	downPipe.x -= 3;
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

			//Set the wall
			SDL_Rect wall1;
			SDL_Rect wall2;
			wall1.x = 538;
			wall1.y = 0;
			wall1.w = 40;
			wall1.h = 200;

			wall2.x = 538;
			wall2.y = 350;
			wall2.w = 40;
			wall2.h = 130;

			SDL_Rect upperPipe;
			SDL_Rect downPipe;
			upperPipe.x = 538;
			upperPipe.y = 0;
			upperPipe.w = 40;
			upperPipe.h = 200;

			downPipe.x = 538;
			downPipe.y = 350;
			downPipe.w = 40;
			downPipe.h = 130;

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
					//Move the bird and check collision
					bird.move(wall1, wall2, gameOver, gameReset);
					//generatePipes(bird, upperPipe, downPipe);

					if (upperPipe.x < 0) {


						upperPipe.x = 538;
						upperPipe.y = 0;
						upperPipe.w = 40;
						downPipe.x = 538;
						downPipe.w = 40;


						upperPipe.h = 1 + (rand() % static_cast<int>(330 - 1 + 1));
						downPipe.y = upperPipe.h + 150;
						downPipe.h = 480 - downPipe.y;

					}
					upperPipe.x -= 3;
					downPipe.x -= 3;

					if (bird.mPosY > 480) {
						gameOver = true;
					}
					if (gameReset) {
						bird.mPosY = 0;
						gameReset = false;
					}

					//mCollider.y = mPosY;

					//If the Bird collided or went too far up or down
					if (checkCollision(bird.mCollider, upperPipe))
					{
						/*	mPosY -= mVelY;
						mCollider.y = mPosY;*/
						gameOver = true;
					}
					if (checkCollision(bird.mCollider, downPipe))
					{
						/*	mPosY -= mVelY;
						mCollider.y = mPosY;*/
						gameOver = true;
					}


				}
				//If end game clear texture and show GameOver
				if (gameOver) {
					gbirdTexture.free();
					board.free();
					endgame.render(0, 0, NULL, NULL,NULL, gRenderer);
					upperPipe.w = 0;
					upperPipe.h = 0;
					downPipe.w = 0;
					downPipe.h = 0;
					upperPipe.x = -1;
					if (e.type == SDL_KEYDOWN)
					{
						//Adjust the velocity
						switch (e.key.keysym.sym)
						{
						case SDLK_RETURN:
							gameOver = false;
							endgame.free();
							bird.reset();
							gameReset = true;
							loadMedia();
							break;

						}
					}
				}

				//Render wall

				SDL_SetRenderDrawColor(gRenderer, 113, 54, 26, 0xFF);
				//Render bird
				board.render(0, 0, NULL, NULL, NULL, gRenderer);
				SDL_RenderDrawRect(gRenderer, &upperPipe);
				SDL_RenderDrawRect(gRenderer, &downPipe);

				if (bird.mPosY < 0) {
					bird.mPosY = 0;
					bird.mCollider.y = 0;
					bird.mVelY = 0;
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