#include "enemy.h"

enemy::enemy(Vector3 pos)
{
	position = pos;
	newSpawn = 1;
	caught = 0;
	strength = 100;
	active = 1;
}

enemy::~enemy()
{

}

Vector3 enemy::getPos()
{
	return position;
}

bool enemy::getCaught()
{
	return caught;
}

bool enemy::getActive()
{
	return active;
}

bool enemy::getNewSpawn()
{
	return newSpawn;
}

void enemy::setNewSpawn(bool input)
{
	newSpawn = input;
}

void enemy::setCaught(bool input)
{
	caught = input;
}

void enemy::setPos(float x, float y, float z)
{
	position = Vector3(x, y, z);
}

Vector3 enemy::getVel()
{
	return vel;
}

void enemy::setVel(float x, float y, float z)
{
	vel.Set(x, y, z);
}

void enemy::runOff(Vector3 playerPos)
{
	//vel = -(playerPos - position) * (playerPos - position);

	//vel.x += -(1 / (playerPos.x - position.x)) * 1;

	//vel.y += -(1 / (playerPos.y - position.y)) * 1;
	//
	///*if (vel.x > -3)
	//{
	//	vel.x -= (1 / -(playerPos.x - position.x)) * 1;
	//}
	//if (vel.y > -3)
	//{
	//	vel.y -= (1 / -(playerPos.y - position.y)) * 1;
	//}*/
	//if (vel.x > 100)
	//{
	//	vel.x = 100;
	//}
	//if (vel.x < -100)
	//{
	//	vel.x = -100;
	//}
	//if (vel.y > 100)
	//{
	//	vel.y = 100;
	//}
	//if (vel.y < -100)
	//{
	//	vel.y = -100;
	//}

	float distanceSquared = (position - playerPos).LengthSquared();
	float yDist = playerPos.y - position.y;
	float xDist = playerPos.x - position.x;
	float angleDiff = atan2(yDist, xDist);
	Vector3 dir;
	dir.x += (distanceSquared * cos(angleDiff));
	dir.y += (distanceSquared * sin(angleDiff));
	if (!dir.IsZero())
	{
		dir.Normalize();
	}
	else
	{
		dir.Set(1, 0, 0);
	}
	if (caught == 0)
	{

		vel += -(dir.Normalized()) * 1;
	}
	else
	{
		vel.x += -((dir.Normalized()).x + Math::RandFloatMinMax(-10, 10)) * 1.5;
		vel.y += -((dir.Normalized()).y + Math::RandFloatMinMax(-10, 10)) * 1.5;
	}

}

void enemy::slowDown()
{
	if (vel.x > 0)
	{
		vel.x -= 1;
	}
	else if (vel.x < 0)
	{
		vel.x += 1;
	}
	if (vel.y < 0)
	{
		vel.y += 1;
	}
	else if (vel.y > 0)
	{
		vel.y -= 1;
	}

}

void enemy::updatePos(float dt)
{
	position += vel * dt * .3;
	if (caught == 1)
	{
		strength--;
	}
	if (strength <= 0)
	{
		active = 0;
	}
}

enemy* newEnemy(float x, float y, float z)
{
	return new enemy(Vector3(x, y, z));
}