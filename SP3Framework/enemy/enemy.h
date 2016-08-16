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
		Vector3 getVel();
		bool getNewSpawn();
		void setNewSpawn(bool input);
		void runOff(Vector3 player);
		void setVel(float x, float y, float z);
		void setPos(float x, float y, float z);
		void updatePos(float dt);
		enum ENEMY_TYPE
		{
			EN_NONE = 0,
			EN_ZEBRA,

			EN_TOTAL, //must be last
		};
		Vector3 position;
		Vector3 vel;
		bool newSpawn;
	};

	ENEMY_API enemy* newEnemy(float x, float y, float z);

