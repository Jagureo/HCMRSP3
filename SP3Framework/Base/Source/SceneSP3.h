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
		s_Upgrade_Cars,
		s_Upgrade_Tires,
		s_Upgrade_Lasso,
		s_Upgrade_Darts,
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

	void Reset();

	void RenderProps(playMap* map);

	void RenderLasso(lasso* lassoPos);

	bool RenderMapFile();
	bool MapExist(string FileName);

	void SetEvent(int eventtype);
	string FileName;
protected:

	GameObject* player1;

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

	//Auditing
	float m1, m2;
	Vector3 u1, u2, v1, v2;
};

#endif