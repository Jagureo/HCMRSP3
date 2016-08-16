#pragma once

#ifndef ENEMY_EXPORTS
#define ENEMY_API __declspec(dllexport) 
#else
#define ENEMY_API __declspec(dllimport) 
#endif

#include "Vector3.h"


	class ENEMY_API enemy
	{
	public:
		enemy(Vector3 pos);
		~enemy();
		Vector3 getPos();
		void setPos(Vector3 pos);

		Vector3 position;
	};

	ENEMY_API enemy* newEnemy(float x, float y, float z);

