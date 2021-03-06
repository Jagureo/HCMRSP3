#include "enemy.h"

enemy::enemy(Vector3 pos, int eType, float getSta, float getSpd, float getStr)
{
	
	position = pos;
	newSpawn = 1;
	caught = 0;
	strength = 100;
	active = 1;
	running = 0;
	runLonger = 0;
	taken = 0;

	if (eType == 0)
	{
		type = EN_ZEBRA;

	}
	else if (eType == 1)
	{
		type = EN_RHINO;

	}
	else if (eType == 2)
	{
		type = EN_LION;

		
	}
	else if (eType == 3)
	{
		type = EN_HUMAN;


	}
	stamina = getSta;
	speed = getSpd * Math::RandFloatMinMax(0.7, 1.3);
	strength = getStr;
	drunk = 0;
	//strength = 99999;
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

bool enemy::getTaken()
{
	return taken;
}


int enemy::getRunLonger()
{
	return runLonger;
}

int enemy::getType()
{
	return type;
}

float enemy::getStrength()
{
	return strength;
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

void enemy::increaseRunLonger(int input)
{
	runLonger += input;
	if (runLonger < 0)
	{
		runLonger = 0;
	}
}

void enemy::addStrength(int input)
{
	strength += input;
}

void enemy::setCaught(bool input)
{
	caught = input;
}

void enemy::setTaken(bool input)
{
	taken = input;
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

void enemy::setActive(bool input)
{
	active = input;
}

void enemy::setLeader(bool input)
{
	leader = input;
}

void enemy::setDrunk(bool input)
{
	drunk = input;
}

void enemy::checkCollision(std::vector<enemy*> enemyVector)
{
	for (std::vector<enemy*>::iterator itE = enemyVector.begin(); itE != enemyVector.end(); ++itE)
	{
		enemy *goE = (enemy *)*itE;
		if (position.x > goE->getPos().x - 5 && position.x < goE->getPos().x + 5 && position.y > goE->getPos().y - 5 && position.y < goE->getPos().y + 5 && goE->active == 1)
		{
			float distanceSquared = (position - goE->getPos()).LengthSquared();
			float yDist = goE->getPos().y - position.y;
			float xDist = goE->getPos().x - position.x;
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
			goE->vel += dir.Normalized();
			vel += -(dir.Normalized());
		}
	}
}

void enemy::runOff(Vector3 playerPos, std::vector<enemy*> enemyVector, enemy* leader1)
{
	running = 1;
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
	if (stamina > 0)
	{
		if (drunk == 0)
		{
			if (caught == 1)
			{

				vel.x += -((dir.Normalized()).x * 2);
				vel.y += -((dir.Normalized()).y * 2);
			}
			else if (leader == 1)
			{
				vel.x += -((dir.Normalized()).x * 1.8);
				vel.y += -((dir.Normalized()).y * 1.8);
			}
			else
			{
				//vel += -(dir.Normalized()) * 3;
				vel += (cohesion(enemyVector, leader1) + alignment(enemyVector, leader1) + seperation(enemyVector, leader1) * 1.8);
				vel.z = 0;
			}
			stamina--;
		}
		else
		{
			if (caught == 1)
			{

				vel.x += -((dir.Normalized()).x * 1.8);
				vel.y += -((dir.Normalized()).y * 1.8);
				strength -= 1;
			}
			else if (leader == 1)
			{
				vel.x += -((dir.Normalized()).x * 1.5);
				vel.y += -((dir.Normalized()).y * 1.5);
			}
			else
			{
				//vel += -(dir.Normalized()) * 3;
				vel += (cohesion(enemyVector, leader1) + alignment(enemyVector, leader1) + seperation(enemyVector, leader1) * 1.5);
				vel.z = 0;
			}
			stamina -= 2;
		}
	}
	else
	{
		if (drunk == 0)
		{

			if (caught == 1)
			{

				vel.x += -((dir.Normalized()).x * 1.5);
				vel.y += -((dir.Normalized()).y  * 1.5);
			}
			if (leader == 1)
			{
				vel.x += -((dir.Normalized()).x * 1.2);
				vel.y += -((dir.Normalized()).y * 1.2);

			}
			else
			{
				vel += (cohesion(enemyVector, leader1) + alignment(enemyVector, leader1) + seperation(enemyVector, leader1) * 1.2);
				vel.z = 0;
			}
		}
		else
		{
			if (caught == 1)
			{
				vel.x += -((dir.Normalized()).x * 1.2);
				vel.y += -((dir.Normalized()).y  * 1.2);
				strength -= 1;
			}
			if (leader == 1)
			{
				vel.x += -((dir.Normalized()).x * 1.0);
				vel.y += -((dir.Normalized()).y * 1.0);

			}
			else
			{
				vel += (cohesion(enemyVector, leader1) + alignment(enemyVector, leader1) + seperation(enemyVector, leader1) * 1.0);
				vel.z = 0;
			}
		}
	}
	
	
	if (stamina < 0)
	{
		stamina = 0;
	}
	
	
}

void enemy::slowDown(std::vector<enemy*> enemyVector, Vector3 objective)
{
	running = 0;
	stamina+= 5;
	if (stamina > 300)
	{
		stamina = 300;
	}
			float distanceSquared = (position - objective).LengthSquared();
			float yDist = objective.y - position.y;
			float xDist = objective.x - position.x;
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
			//(dir.Normalized() * (speed));
			if (drunk == 0)
			{

				if (taken == 0)
				{
					vel += (dir.Normalized());
				}
				else
				{
					vel -= (dir.Normalized());
				}
			}
			else
			{
				if (taken == 0)
				{
					vel += (dir.Normalized() * 0.8);
				}
				else
				{
					vel -= (dir.Normalized() * 0.8);
				}
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
	if (drunk == 0)
	{

		if (running == 1 && stamina > 0)
		{
			if (vel.x > 80 * speed)
			{
				vel.x = 80 * speed;
			}
			else if (vel.x < -80 * speed)
			{
				vel.x = -80 * speed;
			}
			if (vel.y < -80 * speed)
			{
				vel.y = -80 * speed;
			}
			else if (vel.y > 80 * speed)
			{
				vel.y = 80 * speed;
			}
			if (vel.z != 0)
			{
				vel.z = 0;
			}
		}
		else
		{
			if (vel.x > 50 * speed)
			{
				vel.x = 50 * speed;
			}
			else if (vel.x < -50 * speed)
			{
				vel.x = -50 * speed;
			}
			if (vel.y < -50 * speed)
			{
				vel.y = -50 * speed;
			}
			else if (vel.y > 50 * speed)
			{
				vel.y = 50 * speed;
			}
			if (vel.z != 0)
			{
				vel.z = 0;
			}
		}
	}
	else
	{
		if (running == 1 && stamina > 0)
		{
			if (vel.x > 40 * speed)
			{
				vel.x = 40 * speed;
			}
			else if (vel.x < -40 * speed)
			{
				vel.x = -40 * speed;
			}
			if (vel.y < -40 * speed)
			{
				vel.y = -40 * speed;
			}
			else if (vel.y > 40 * speed)
			{
				vel.y = 40 * speed;
			}
			if (vel.z != 0)
			{
				vel.z = 0;
			}
		}
		else
		{
			if (vel.x > 25 * speed)
			{
				vel.x = 25 * speed;
			}
			else if (vel.x < -25 * speed)
			{
				vel.x = -25 * speed;
			}
			if (vel.y < -25 * speed)
			{
				vel.y = -25 * speed;
			}
			else if (vel.y > 25 * speed)
			{
				vel.y = 25 * speed;
			}
			if (vel.z != 0)
			{
				vel.z = 0;
			}
		}
	}
}

Vector3 enemy::cohesion(std::vector<enemy*> enemyVector, enemy* leaderI)
{
	Vector3 forceVector = (0,0,0);
	Vector3 centerOfMass = (0, 0, 0);
	int neighbour = 0;

	for (int i = 0; i < enemyVector.size(); ++i)
	{
		if (enemyVector[i]->getPos() != leaderI->position && enemyVector[i]->newSpawn == 0)
		{
			centerOfMass += enemyVector[i]->getPos();
			neighbour++;
		}
	}

	if (neighbour > 0)
	{
		centerOfMass.x /= neighbour;
		centerOfMass.y /= neighbour;
		//centerOfMass.z /= neighbour;

		forceVector = centerOfMass - leaderI->position;

		forceVector = forceVector.Normalize();
	}
	return forceVector;
}

Vector3 enemy::seperation(std::vector<enemy*> enemyVector, enemy* leaderI)
{
	Vector3 forceVector = (0, 0, 0);


	for (int i = 0; i < enemyVector.size(); ++i)
	{
		if (enemyVector[i]->getPos() != leaderI->position && enemyVector[i]->newSpawn == 0)
		{
			float distanceSquared = (leaderI->position - enemyVector[i]->getPos()).LengthSquared();
			if (distanceSquared < 300)
			{
				Vector3 headingVector = enemyVector[i]->getPos() - leaderI->position;
				float scale = headingVector.Length() / sqrt(300);
				forceVector = headingVector.Normalize();
				forceVector.x /= scale;
				forceVector.y /= scale;
			}
		}
	}
	return forceVector;
}
Vector3 enemy::alignment(std::vector<enemy*> enemyVector, enemy* leaderI)
{
	Vector3 forceVector = (0, 0, 0);
	int neighbour = 0;

	for (int i = 0; i < enemyVector.size(); ++i)
	{
		if (enemyVector[i]->getPos() != leaderI->position && vel != (0, 0, 0) && enemyVector[i]->getVel() != (0, 0, 0) && enemyVector[i]->newSpawn == 0)
		{
			if ((enemyVector[i]->getPos() - leaderI->position).Length() < 1000)
			{
				forceVector += enemyVector[i]->getVel();
				neighbour++;
			}
		}
	}

	if (neighbour > 0)
	{
		forceVector.x /= neighbour;
		forceVector.y /= neighbour;
		forceVector.Normalize();
	}
	return forceVector;
}


enemy* newEnemy(float x, float y, float z, int type, float getSta, float getSpd, float getStr)
{
	return new enemy(Vector3(x,y,z), type, getSta,getSpd,getStr);
}