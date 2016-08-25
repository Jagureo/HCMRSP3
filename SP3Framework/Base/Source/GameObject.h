#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

#include "Vector3.h"
#include "Mtx44.h"
#include "rocketcar.h"

struct GameObject
{
	enum GAMEOBJECT_TYPE
	{
		GO_NONE = 0,
		GO_BALL,
		GO_CUBE,
		GO_ASTEROID,
		GO_SHIP,
		GO_ENEMY,
		GO_BULLET,
		GO_ENEMYBULLET,
		GO_MISSILE,
		GO_BLACKHOLE,
		GO_WALL,
		GO_CAR,
		GO_GOAL,
		GO_PILLAR,
		GO_POWERUP,
		GO_SAWBLADE,
		GO_PARTICLE,

		MAP_TREE,
		MAP_ROCK,
		MAP_WATER,
		MAP_MUD,
		MAP_LION,
		MAP_ZEBRA,
		MAP_RHINO,
		MAP_HUMAN,
		MAP_OBJECTIVE,

		GO_TOTAL, //must be last
	};
	GAMEOBJECT_TYPE type;
	Vector3 pos;
	Vector3 vel;
	Vector3 scale;
	Vector3 normal;
	rocketCar playerCar;
	float ballrotated;
	float engine;
	float turnPower;
	float rotationAngle;
	float color;
	Mtx44 rotatedVel;
	bool active;
	bool dead;
	float mass;
	float ticker;
	bool fresh;
	bool radarVisible;

	GameObject(GAMEOBJECT_TYPE typeValue = GO_BALL);
	~GameObject();
};

#endif