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
		switch (e.key.keysym.sym) { case SDLK_SPACE: mPosY -= 120; break; }
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
	mCollider.y = mPosY;
	mPosX += mVelX;
	mCollider.x = mPosX;
}

