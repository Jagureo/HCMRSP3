#include "lasso.h"

lasso::lasso()
{
	currLassoState = READY;
}

lasso::~lasso()
{

}

void lasso::throwLasso(Vector3 playerPos, Vector3 mousePos)
{
	
	lassoPos = playerPos;
	targetPos = mousePos;
	currLassoRange = 0;
	currLassoState = THROWN;

}

bool lasso::updateLasso(Vector3 playerPos, float dt)
{
	if (currLassoState == 1)
	{
		if (currLassoRange > maxLassoRange || lassoPos.x > targetPos.x - 1 &&  lassoPos.x < targetPos.x + 1 && lassoPos.y > targetPos.y - 1 &&  lassoPos.y < targetPos.y + 1)
		{
			currLassoState = MISS;
		}
		else
		{
			lassoVel += (targetPos - lassoPos) * 0.1;
			lassoPos += lassoVel * dt * 0.1;
			currLassoRange = (lassoPos - playerPos).Length();
			std::cout << currLassoRange << std::endl;
		}
		
	}
	else if (currLassoState == 3)
	{
		lassoVel += (targetPos - lassoPos) * 0.1;
		lassoPos -= lassoVel * dt * 0.1;
		if (lassoPos.x > playerPos.x - 1 &&  lassoPos.x < playerPos.x + 1 && lassoPos.y > playerPos.y - 1 &&  lassoPos.y < playerPos.y + 1)
		{
			currLassoState = READY;
			lassoVel = (0,0,0);
		}
		std::cout << "cumming bak" << std::endl;
	}

	return false;
}
bool lasso::checkCaught(Vector3 playerPos, Vector3 enemyPos, float animalSize)
{	
	if (lassoPos.x >= enemyPos.x - animalSize && lassoPos.x <= enemyPos.x + animalSize && lassoPos.y >= enemyPos.y - animalSize && lassoPos.y <= enemyPos.y + animalSize && currLassoState == THROWN)
	{
		currLassoState = CAUGHT;
		lassoPos = enemyPos;
		return true;
	}
	
}

bool lasso::caughtUpdate(Vector3 playerPos, Vector3 enemyPos)
{
	std::cout << (lassoPos - playerPos).Length() << std::endl;
	if ((lassoPos - playerPos).Length() > maxLassoRange && currLassoState == CAUGHT)
	{
		currLassoState = MISS;
		targetPos = enemyPos;
		lassoPos = playerPos;
		return true;
	}
	lassoPos = enemyPos;
	return false;
}

Vector3 lasso::getLassoPos()
{
	return lassoPos;
}

int lasso::getLassoState()
{
	return currLassoState;
}

lasso* newLasso()
{
	return new lasso();
}