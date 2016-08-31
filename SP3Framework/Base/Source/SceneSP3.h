#ifndef SCENE_SP3_H
#define SCENE_SP3_H

#include "GameObject.h"
#include <vector>
#include "SceneBase.h"
#include <fstream>
#include <sstream>
#include "Map.h"
#include "TextFile.h"
#include "enemy.h"
#include "lasso.h"
#include <irrKlang.h>
using namespace irrklang;

using std::stoi;
using std::ifstream;

class SceneSP3 : public SceneBase
{
	enum class states
	{
		s_Menu,
		s_Instructions,
		s_MapEditor,
		s_Options,
		s_LevelSelect,
		s_CustomLevelSelect,
		s_Upgrade,
		s_Upgrade_Cars1,
		s_Upgrade_Cars2,
		s_Upgrade_Cars3,
		s_Upgrade_Tires1,
		s_Upgrade_Tires2,
		s_Upgrade_Tires3,
		s_Upgrade_Lasso1,
		s_Upgrade_Lasso2,
		s_Upgrade_Lasso3,
		s_Upgrade_Darts1,
		s_Upgrade_Darts2,
		s_Upgrade_Darts3,
		s_Tutorial,
		s_Level2,
		s_Level3,
		s_LevelBoss,
		s_Win,
		s_Lose,
		s_Highscore,
	};

	static const int MAX_SPEED = 10;
	static const int BULLET_SPEED = 50;
	static const int MISSILE_SPEED = 20;
	static const int MISSILE_POWER = 1;

	states gameStates;
	int arrowSelection;
	float rotateDisplayX;
	float rotateDisplayY;
	float fuelAmount;

	bool car1Bought;
	bool car2Bought;
	bool car3Bought;
	bool tire1Bought;
	bool tire2Bought;
	bool tire3Bought;
	bool lasso1Bought;
	bool lasso2Bought;
	bool lasso3Bought;
	bool dart1Bought;
	bool dart2Bought;
	bool dart3Bought;

	int currentLevel;

	void InitCarStat(string varname);

public:
	SceneSP3();
	~SceneSP3();

	virtual void Init();
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();

	void RenderGO(GameObject *go);
	void RenderEnemy(enemy *go);

	GameObject* FetchGO();
	bool CheckCollision(GameObject *go, GameObject *other, double dt);
	float CheckCollision2(GameObject *go1, GameObject *go2);
	bool CheckCollision3(GameObject *go, GameObject *other, double dt);
	void CollisionResponse(GameObject *go, GameObject *other);

	void CollisionMap(GameObject *go, GameObject *other, double dt);

	void playerControl();

	void mapEditorUpdate(double dt);
	void mapEditorRender();
	void renderSelection(float x1, float y1);
	void renderMinimap(playMap* map);
	void renderMenu();
	void UpgradeController();

	void Reset();

	void RenderProps(playMap* map);

	void RenderLasso(lasso* lassoPos);

	bool RenderMapFile();
	bool MapExist(string FileName);

	void engineSound();
	void throwSound();
	void snapSound();
	void dingSound();
	void bumpSound();
	void backSound();

	void eraseEnemy();

	string FileName;
	
protected:

	GameObject* player1;

	TextFile* animalStat;

	lasso* Dalasso = newLasso();

	float friction;

	Vector3 mapPosition;
	playMap *currentMap;
	playMap testMap;

	//enemy* zebra;

	//Physics
	std::vector<GameObject *> m_goList;
	std::vector<enemy *> enemyList;
	float m_speed;
	float m_worldWidth;
	float m_worldHeight;
	float m_estimatedTime;
	GameObject *m_ghost;
	int m_objectCount;
	bool driftMode;

	string mapName;
	bool nameType;

	bool dragObj;
	bool testMode;
	int deleteMode;
	bool panel2;
	int panel2x;

	float newMouseX;
	float newMouseY;
	float diffx;
	float diffy;
	float time;

	int points = 0;

	Vector3 objective;
	bool updateObjective;
	Vector3 center;

	enemy* leader;

	//Auditing
	float m1, m2;
	Vector3 u1, u2, v1, v2;

	bool highQ;
	bool sound;
	bool snapSet;
	ISoundEngine* theSoundEngine;
	ISound* Sound_Engine;
	ISound* Sound_Throw;
	ISound* Sound_Snap;
	ISound* Sound_Ding;
	ISound* Sound_Bump;
	ISound* Sound_Back;
};

#endif