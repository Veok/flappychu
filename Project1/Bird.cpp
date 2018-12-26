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
		switch (e.key.keysym.sym)
		{
		case SDLK_UP: mVelY -= 20; break;

		}
	}
	if (e.type == SDL_KEYUP)
	{
		//Adjust the velocity
		switch (e.key.keysym.sym)
		{
		case SDLK_UP: mVelY += 20; break;

		}

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


void Bird::move(SDL_Rect& wall, SDL_Rect& wall2, bool gameOver, bool gameReset)
{
	//Move the Bird left or right
	/*mPosX += mVelX;
	mCollider.x = mPosX;*/
	//If the Bird collided or went too far to the left or right
	//if ((mPosX < 0) || (mPosX + Bird_WIDTH > SCREEN_WIDTH) || checkCollision(mCollider, wall))
	//{
	//	//Move back
	//	mPosX -= mVelX;
	//	mCollider.x = mPosX;
	//}

	//Move the Bird up or down



	//if (mPosY < 0) {
	//	mPosY = 0;
	//	mCollider.y = 0;
	//}
	mPosY += 8;
	mPosY += mVelY;
	mCollider.y = mPosY;
	mPosX += mVelX;
	mCollider.x = mPosX;


}

