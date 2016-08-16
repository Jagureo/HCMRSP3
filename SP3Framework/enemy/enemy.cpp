#include "enemy.h"

enemy::enemy(Vector3 pos)
{
	position = pos;
	newSpawn = 1;
}

enemy::~enemy()
{

}

Vector3 enemy::getPos()
{
	return position;
}

bool enemy::getNewSpawn()
{
	return newSpawn;
}

void enemy::setNewSpawn(bool input)
{
	newSpawn = input;
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
	vel = -(playerPos - position) * 3;
}

void enemy::updatePos(float dt)
{
	position += vel * dt * .2;
}

enemy* newEnemy(float x, float y, float z)
{
	return new enemy(Vector3(x,y,z));
}