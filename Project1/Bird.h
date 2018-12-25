#include <SDL.h>
#include <stdio.h>
#include <SDL_image.h>
#include <string>
#include <SDL_ttf.h>
#include <sstream>
#include "LTexture.h"

#pragma once
class Bird
{


public:
	//The dimensions of the Bird
	static const int Bird_WIDTH = 40;
	static const int Bird_HEIGHT = 33;

	//Maximum axis velocity of the Bird
	static const int Bird_VEL = 10;

	//Initializes the variables
	Bird();

	//Takes key presses and adjusts the Bird's velocity
	void handleEvent(SDL_Event& e);

	//Moves the Bird and checks collision
	void move(SDL_Rect& wall, SDL_Rect& wall2, bool gameOver, bool gameReset);

	void reset();

	int mPosX, mPosY;

	int mVelX, mVelY;

	//Bird's collision box
	SDL_Rect mCollider;

};