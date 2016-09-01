#ifndef SCENE_BASE_H
#define SCENE_BASE_H

#include "Scene.h"
#include "Mtx44.h"
#include "Camera.h"
#include "Mesh.h"
#include "MatrixStack.h"
#include "Light.h"
#include "GameObject.h"
#include <vector>

class SceneBase : public Scene
{
	enum UNIFORM_TYPE
	{
		U_MVP = 0,
		U_MODELVIEW,
		U_MODELVIEW_INVERSE_TRANSPOSE,
		U_MATERIAL_AMBIENT,
		U_MATERIAL_DIFFUSE,
		U_MATERIAL_SPECULAR,
		U_MATERIAL_SHININESS,
		U_LIGHTENABLED,
		U_NUMLIGHTS,
		U_LIGHT0_TYPE,
		U_LIGHT0_POSITION,
		U_LIGHT0_COLOR,
		U_LIGHT0_POWER,
		U_LIGHT0_KC,
		U_LIGHT0_KL,
		U_LIGHT0_KQ,
		U_LIGHT0_SPOTDIRECTION,
		U_LIGHT0_COSCUTOFF,
		U_LIGHT0_COSINNER,
		U_LIGHT0_EXPONENT,
		U_COLOR_TEXTURE_ENABLED,
		U_COLOR_TEXTURE,
		U_TEXT_ENABLED,
		U_TEXT_COLOR,
		U_TOTAL,
	};
public:
	enum GEOMETRY_TYPE
	{
		GEO_AXES,
		GEO_TEXT,
		GEO_BALL,
		GEO_CUBE,
		GEO_SHIP,
		GEO_ENEMY,
		GEO_ENEMYBULLET,
		GEO_BLACKHOLE,
		GEO_CAR1,
		GEO_CAR2,
		GEO_POST1,
		GEO_POST2,
		GEO_GOAL1,
		GEO_GOAL2,
		GEO_SOCC,
		GEO_POWERUP,
		GEO_SAWBLADE,
		GEO_ICE,

		//SP3
		GEO_BACKGROUND,
		GEO_TESTMAP,
		GEO_TESTMAP2,
		GEO_TREETOP,
		GEO_ROCK,
		GEO_MUD,
		GEO_LION,
		GEO_ZEBRA,
		GEO_RHINO,
		GEO_HUMAN,
		GEO_OBJECTIVE,
		//GEO_JEEP,

		GEO_TESTENEMY,

		GEO_ROPE,

		HUD_MAPEDITOR,
		HUD_MAPEDITOR2,
		HUD_MAPEDITOR3,
		HUD_TESTMODE,
		HUD_DELETEICON,
		HUD_SELECTION,
		HUD_GRIDLOCK,
		HUD_RADAR,
		HUD_RADARLINE,
		HUD_RADARDETECT,
		HUD_RADARENEMY,

		GEO_MENU_BACKGROUND,
		GEO_MENU_MAPEDITOR,
		GEO_MENU_SIGNBOARD,
		GEO_MENU_ARROW,
		GEO_MENU_STARTGAME,
		GEO_MENU_INSTRUCTIONS,
		GEO_MENU_INSTRUCTIONS_PAGE,
		GEO_MENU_OPTIONS,
		GEO_MENU_HIGHSCORE,
		GEO_MENU_HIGHSCORE_PAGE,
		GEO_MENU_PAUSE,
		GEO_MENU_QUIT,
		GEO_MENU_HIGHSCORE_PAGE2,

		GEO_UPGRADE_BACKGROUND,
		GEO_UPGRADE_UI_CARS,
		GEO_UPGRADE_UI_TIRES,
		GEO_UPGRADE_UI_LASSO,
		GEO_UPGRADE_UI_DARTS,
		GEO_NEXTLEVEL,

		GEO_DISPLAY_CAR1,
		GEO_DISPLAY_CAR2,
		GEO_DISPLAY_CAR3,
		GEO_DISPLAY_WHEEL,
		GEO_DISPLAY_ROPE,
		GEO_DISPLAY_DARTS,

		GEO_BUY,
		GEO_SOLD,

		GEO_LEVELSELECT,
		GEO_LEVELSELECT_CUSTOM,

		GEO_OPTIONS,

		GEO_LOSE_SCENE,
		GEO_WIN_SCENE,

		NUM_GEOMETRY,
	};
public:
	SceneBase();
	~SceneBase();

	virtual void Init();
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();

	void RenderText(Mesh* mesh, std::string text, Color color);
	void RenderTextOnScreen(Mesh* mesh, std::string text, Color color, float size, float x, float y);
	void RenderMesh(Mesh *mesh, bool enableLight);
	void RenderGO(GameObject *go);

	GameObject* FetchGO();
protected:
	unsigned m_vertexArrayID;
	Mesh* meshList[NUM_GEOMETRY];
	unsigned m_programID;
	unsigned m_parameters[U_TOTAL];

	Camera camera;

	MS modelStack;
	MS viewStack;
	MS projectionStack;

	Light lights[1];

	bool bLightEnabled;

	float fps;
};

#endif