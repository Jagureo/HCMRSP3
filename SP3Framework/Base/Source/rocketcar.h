#ifndef ROCKETCAR_H
#define ROCKETCAR_H

#include <string>
using std::string;

struct rocketCar
{
	float engine;
	float hp;
	float handling;
	float topSpeed;
	float turnSpeed;
	float mass;
	float acceleration;
	int powerUp;
	string name;
};

#endif