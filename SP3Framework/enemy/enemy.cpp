#include "enemy.h"

enemy::enemy(Vector3 pos)
{
	position = pos;
}

enemy::~enemy()
{

}

Vector3 enemy::getPos()
{
	return position;
}

void enemy::setPos(Vector3 pos)
{
	position = pos;
}

enemy* newEnemy(float x, float y, float z)
{
	return new enemy(Vector3(x,y,z));
}