#pragma once

#ifndef ENEMY_EXPORTS
#define ENEMY_API __declspec(dllexport) 
#else
#define ENEMY_API __declspec(dllimport) 
#endif

#include "Vector3.h"
#include <vector>


	class ENEMY_API enemy
	{
	public:
		
		enemy(Vector3 pos, int type, float getSta, float getSpd, float getStr);
		~enemy();
		Vector3 getPos();
		Vector3 getVel();
		bool getActive();
		bool getNewSpawn();
		bool getCaught();
		int getRunLonger();
		int getType();
		void setNewSpawn(bool input);
		void setCaught(bool input);
		void runOff(Vector3 player, std::vector<enemy*> enemyVector, enemy* leaderI);
		void slowDown( std::vector<enemy*> enemyVector, Vector3 objective);
		void setVel(float x, float y, float z);
		void setPos(float x, float y, float z);
		void setActive(bool input);
		void setLeader(bool input);
		void increaseRunLonger(int input);
		void updatePos(float dt);
		void checkCollision(std::vector<enemy*> enemyVector);
		enum ENEMY_TYPE
		{
			EN_ZEBRA,
			EN_RHINO,
			EN_LION,
			EN_TOTAL, //must be last
		};
		ENEMY_TYPE type;
		bool caught;
		bool running;
		int strength;
		Vector3 position;
		Vector3 vel;
		bool active;
		bool newSpawn;
		bool leader;
		int stamina;
		int runLonger;
		int speed;
		
		Vector3 cohesion(std::vector<enemy*> enemyVector, enemy* leaderI);
		Vector3 seperation(std::vector<enemy*> enemyVector, enemy* leaderI);
		Vector3 alignment(std::vector<enemy*> enemyVector, enemy* leaderI);
	};

	ENEMY_API enemy* newEnemy(float x, float y, float z, int type, float getSta, float getSpd, float getStr);
