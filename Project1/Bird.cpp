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
}

void Bird::handleEvent(SDL_Event& e)
{
	//If a key was pressed
	/*if (e.type == SDL_KEYDOWN)
	{
		switch (e.key.keysym.sym)
		{
		case SDLK_SPACE:
			int i = 0;
			while (i < 14)
			{	
				i++;
				mPosY -= i;
			}
			break;
		}
	}*/
}

void Bird::reset()
{
	mPosX = 269;
	mPosY = 240;
	//Set collision box dimension
	mCollider.w = Bird_WIDTH;
	mCollider.h = Bird_HEIGHT;
}


void Bird::move()
{
	mPosY += 7;
	mCollider.y = mPosY;
	mCollider.x = mPosX;
}
