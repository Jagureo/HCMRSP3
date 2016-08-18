#include "enemy.h"

enemy::enemy(Vector3 pos)
{
	position = pos;
	newSpawn = 1;
	caught = 0;
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
	position = Vector3(x,y,z);
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
	if (vel.x < 5)
	{
		vel.x += (1 / -(playerPos.x - position.x)) * 10;
	}
	if (vel.y < 5)
	{
		vel.y += (1 / -(playerPos.y - position.y)) * 10;
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

}

enemy* newEnemy(float x, float y, float z)
{
	return new enemy(Vector3(x,y,z));
}