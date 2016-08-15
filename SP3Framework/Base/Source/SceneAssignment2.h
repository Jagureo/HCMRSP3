#ifndef SCENE_ASSIGNMENT2_H
#define SCENE_ASSIGNMENT2_H

#include "GameObject.h"
#include <vector>
#include "SceneBase.h"
#include <fstream>
#include <sstream>
using std::stoi;
using std::ifstream;


class SceneAssignment2 : public SceneBase
{
	static const int MAX_SPEED = 10;
	static const int BULLET_SPEED = 50;
	static const int MISSILE_SPEED = 20;
	static const int MISSILE_POWER = 1;

public:
	SceneAssignment2();
	~SceneAssignment2();

	virtual void Init();
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();

	void RenderGO(GameObject *go);

	GameObject* FetchGO();
	bool CheckCollision(GameObject *go, GameObject *other, double dt);
	float CheckCollision2(GameObject *go1, GameObject *go2);
	bool CheckCollision3(GameObject *go, GameObject *other, double dt);
	bool carballCollide(GameObject *go, GameObject *other, double dt);
	void CollisionResponse(GameObject *go, GameObject *other);
	bool spawnMap();
	void menuUpdate(double dt);
	void menuRender();
	void loadPlayer1();
	void loadPlayer2();
	void Reset();
protected:
	GameObject *player1;
	GameObject *player2;
	GameObject *player1goal;
	GameObject *player2goal;
	GameObject *playerAI;
	GameObject *soccerBall;

	//Physics
	std::vector<GameObject *> m_goList;
	float m_speed;
	float m_worldWidth;
	float m_worldHeight;
	float m_estimatedTime;
	GameObject *m_ghost;
	int m_objectCount;

	bool gameStart;
	float timer;
	float timer2;
	float timer3;
	int m_ballCount;
	Vector3 debug;
	Vector3 debugscale;
	bool debugactive;
	float debugcolor;
	float friction;
	bool loading;
	int internationalDT;
	
	int min3;
	int second3;

	int player1score;
	int player2score;

	int player1power;
	int player2power;

	float player1poweractive;
	float player2poweractive;
	bool player1poweractived;
	bool player2poweractived;

	bool mainMenu;
	int numOfPlayers;
	bool gameModeSelected;
	int player1preset;
	int player2preset;
	int maxCars;
	bool player1ready;
	bool player2ready;

	//Auditing
	float m1, m2;
	Vector3 u1, u2, v1, v2;
};

#endif