#ifndef SCENE_ASSIGNMENT1_H
#define SCENE_ASSIGNMENT1_H

#include "GameObject.h"
#include <vector>
#include "SceneBase.h"

class SceneAssignment1 : public SceneBase
{
	static const int MAX_SPEED = 25;
	static const int BULLET_SPEED = 50;
	static const int MISSILE_SPEED = 20;
	static const int MISSILE_POWER = 1;

public:
	SceneAssignment1();
	~SceneAssignment1();

	virtual void Init();
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();

	void RenderGO(GameObject *go);

	GameObject* FetchGO();
protected:

	//Physics
	std::vector<GameObject *> m_goList;
	float m_speed;
	float m_worldWidth;
	float m_worldHeight;
	GameObject *m_ship;
	GameObject *m_blackhole;
	GameObject *m_enemy;
	Vector3 m_force;
	Vector3 faceDirection;
	float faceAngle;
	float jetFuel;
	float spinner;
	bool reignite;
	bool enemyActive;
	int randomtimer;
	int m_objectCount;
	int m_lives;
	int m_score;
	static const int firerate = 5;
	float fireDebounceTimer;
};

#endif