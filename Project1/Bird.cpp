#include "Bird.h"
#include "LTexture.h"


Bird::Bird()
{
	//Initialize the offsets
	mPosX = 269;
	mPosY = 240;

	//Set collision box dimension
	mCollider.w = Bird_WIDTH;
	mCollider.h = Bird_HEIGHT;

	//Initialize the velocity
	mVelX = 0;
	mVelY = 0;
}

void Bird::handleEvent(SDL_Event& e)
{
	//If a key was pressed
	if (e.type == SDL_KEYDOWN)
	{
		//Adjust the velocity
		switch (e.key.keysym.sym) { case SDLK_UP: mVelY -= 20; break; }
	}

	if (e.type == SDL_KEYUP)
	{
		//Adjust the velocity
		switch (e.key.keysym.sym) { case SDLK_UP: mVelY += 20; break; }
	}
}
void Bird::reset() {
	mPosX = 269;
	mPosY = 240;
	//Set collision box dimension
	mCollider.w = Bird_WIDTH;
	mCollider.h = Bird_HEIGHT;

	//Initialize the velocity
	mVelX = 0;
	mVelY = 0;
}


void Bird::move()
{
	mPosY += 8;
	mPosY += mVelY;
	mCollider.y = mPosY;
	mPosX += mVelX;
	mCollider.x = mPosX;
}

