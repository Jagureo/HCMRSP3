#pragma once

#ifndef LASSO_EXPORTS
#define LASSO_API __declspec(dllexport) 
#else
#define LASSO_API __declspec(dllimport) 
#endif

#include "Vector3.h"



class LASSO_API lasso
{
public:
	lasso();
	~lasso();

	
	
	
	void throwLasso(Vector3 playerPos, Vector3 mousePos);
	bool checkCaught(Vector3 playerPos, Vector3 enemyPos, float animalSize);
	bool caughtUpdate(Vector3 playerPos, Vector3 enemyPos, bool enemyActive);
	bool updateLasso(Vector3 playerPos,float dt);
	void setLassoRange(float range);
	Vector3 getLassoPos();
	int getLassoState();
	Vector3 getLassoVel();
	Vector3 getTargetPos();

private:
	enum lassoStates
	{
		READY,
		THROWN,
		CAUGHT,
		MISS,
	};
	float maxLassoRange = 50;
	float currLassoRange;
	Vector3 lassoPos;
	lassoStates currLassoState;
	Vector3 targetPos;
	Vector3 lassoVel;


};

LASSO_API lasso* newLasso();