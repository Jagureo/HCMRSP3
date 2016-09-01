#include "lasso.h"

lasso::lasso(float range, float spd)
{
	currLassoState = READY;
	maxLassoRange = range;
	lassoSpd = spd;
}

lasso::~lasso()
{

}

void lasso::setLassoRange(float input)
{
	maxLassoRange = input;
}

void lasso::setLassoSpd(float input)
{
	lassoSpd = input;
}


bool lasso::throwLasso(Vector3 playerPos, Vector3 mousePos)
{
	if ((mousePos - playerPos).Length() > 3)
	{
		lassoPos = playerPos;
		targetPos = mousePos;
		currLassoRange = 0;
		currLassoState = THROWN;
		lassoVel = (targetPos - lassoPos);
		lassoVel.Normalize();
		lassoVel *= lassoSpd;
		return true;
	}
	return false;

}

bool lasso::updateLasso(Vector3 playerPos, float dt)
{
	
		if (currLassoState == 1)
		{
			if (currLassoRange > maxLassoRange || lassoPos.x > targetPos.x - 1 && lassoPos.x < targetPos.x + 1 && lassoPos.y > targetPos.y - 1 && lassoPos.y < targetPos.y + 1)
			{
				lassoVel = (0, 0, 0);
				currLassoState = MISS;
			}
			else
			{

				lassoVel = (targetPos - lassoPos);
				lassoVel.Normalize();
				lassoVel *= lassoSpd;
				lassoPos += lassoVel;
				currLassoRange = (lassoPos - playerPos).Length();

				//std::cout << currLassoRange << std::endl;
			}

		}
		else if (currLassoState == 3)
		{

			lassoVel += (lassoPos - playerPos);
			lassoVel.Normalize();
			lassoVel *= 0.5 * lassoSpd;
			lassoPos -= lassoVel;
			currLassoRange = (lassoPos - playerPos).Length();

			if (/*lassoPos.x > playerPos.x - 3 &&  lassoPos.x < playerPos.x + 3 && lassoPos.y > playerPos.y - 3 &&  lassoPos.y < playerPos.y + 3*/(lassoPos - playerPos).Length() < 5)
			{
				currLassoState = READY;
				lassoVel = (0, 0, 0);
			}
			//std::cout << "cumming bak" << std::endl;
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

bool lasso::caughtUpdate(Vector3 playerPos, Vector3 enemyPos, bool enemyActive)
{
	std::cout << (lassoPos - playerPos).Length() << std::endl;
	if ((lassoPos - playerPos).Length() > maxLassoRange && currLassoState == CAUGHT || enemyActive == 0)
	{
		currLassoState = MISS;
		lassoVel = (0, 0, 0);
		targetPos = enemyPos;
		return true;
	}
	lassoPos = enemyPos;
	return false;
}

Vector3 lasso::getLassoPos()
{
	return lassoPos;
}

Vector3 lasso::getTargetPos()
{
	return targetPos;
}

Vector3 lasso::getLassoVel()
{
	return lassoVel;
}

int lasso::getLassoState()
{
	return currLassoState;
}

lasso* newLasso(float range, float speed)
{
	return new lasso(range, speed);
}