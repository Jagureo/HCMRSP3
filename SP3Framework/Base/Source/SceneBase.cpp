#include "SceneBase.h"
#include "GL\glew.h"

#include "shader.hpp"
#include "MeshBuilder.h"
#include "Application.h"
#include "Utility.h"
#include "LoadTGA.h"
#include <sstream>

SceneBase::SceneBase()
{
}

SceneBase::~SceneBase()
{
}

void SceneBase::Init()
{
	// Black background
	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);
	// Enable depth test
	glEnable(GL_DEPTH_TEST);
	// Accept fragment if it closer to the camera than the former one
	glDepthFunc(GL_LESS); 
	
	glEnable(GL_CULL_FACE);
	
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glGenVertexArrays(1, &m_vertexArrayID);
	glBindVertexArray(m_vertexArrayID);

	m_programID = LoadShaders( "Shader//comg.vertexshader", "Shader//comg.fragmentshader" );
	
	// Get a handle for our uniform
	m_parameters[U_MVP] = glGetUniformLocation(m_programID, "MVP");
	//m_parameters[U_MODEL] = glGetUniformLocation(m_programID, "M");
	//m_parameters[U_VIEW] = glGetUniformLocation(m_programID, "V");
	m_parameters[U_MODELVIEW] = glGetUniformLocation(m_programID, "MV");
	m_parameters[U_MODELVIEW_INVERSE_TRANSPOSE] = glGetUniformLocation(m_programID, "MV_inverse_transpose");
	m_parameters[U_MATERIAL_AMBIENT] = glGetUniformLocation(m_programID, "material.kAmbient");
	m_parameters[U_MATERIAL_DIFFUSE] = glGetUniformLocation(m_programID, "material.kDiffuse");
	m_parameters[U_MATERIAL_SPECULAR] = glGetUniformLocation(m_programID, "material.kSpecular");
	m_parameters[U_MATERIAL_SHININESS] = glGetUniformLocation(m_programID, "material.kShininess");
	m_parameters[U_LIGHTENABLED] = glGetUniformLocation(m_programID, "lightEnabled");
	m_parameters[U_NUMLIGHTS] = glGetUniformLocation(m_programID, "numLights");
	m_parameters[U_LIGHT0_TYPE] = glGetUniformLocation(m_programID, "lights[0].type");
	m_parameters[U_LIGHT0_POSITION] = glGetUniformLocation(m_programID, "lights[0].position_cameraspace");
	m_parameters[U_LIGHT0_COLOR] = glGetUniformLocation(m_programID, "lights[0].color");
	m_parameters[U_LIGHT0_POWER] = glGetUniformLocation(m_programID, "lights[0].power");
	m_parameters[U_LIGHT0_KC] = glGetUniformLocation(m_programID, "lights[0].kC");
	m_parameters[U_LIGHT0_KL] = glGetUniformLocation(m_programID, "lights[0].kL");
	m_parameters[U_LIGHT0_KQ] = glGetUniformLocation(m_programID, "lights[0].kQ");
	m_parameters[U_LIGHT0_SPOTDIRECTION] = glGetUniformLocation(m_programID, "lights[0].spotDirection");
	m_parameters[U_LIGHT0_COSCUTOFF] = glGetUniformLocation(m_programID, "lights[0].cosCutoff");
	m_parameters[U_LIGHT0_COSINNER] = glGetUniformLocation(m_programID, "lights[0].cosInner");
	m_parameters[U_LIGHT0_EXPONENT] = glGetUniformLocation(m_programID, "lights[0].exponent");
	// Get a handle for our "colorTexture" uniform
	m_parameters[U_COLOR_TEXTURE_ENABLED] = glGetUniformLocation(m_programID, "colorTextureEnabled");
	m_parameters[U_COLOR_TEXTURE] = glGetUniformLocation(m_programID, "colorTexture");
	// Get a handle for our "textColor" uniform
	m_parameters[U_TEXT_ENABLED] = glGetUniformLocation(m_programID, "textEnabled");
	m_parameters[U_TEXT_COLOR] = glGetUniformLocation(m_programID, "textColor");
	
	// Use our shader
	glUseProgram(m_programID);

	lights[0].type = Light::LIGHT_DIRECTIONAL;
	lights[0].position.Set(5, 20, 0);
	lights[0].color.Set(1, 1, 1);
	lights[0].power = 1;
	lights[0].kC = 1.f;
	lights[0].kL = 0.01f;
	lights[0].kQ = 0.001f;
	lights[0].cosCutoff = cos(Math::DegreeToRadian(45));
	lights[0].cosInner = cos(Math::DegreeToRadian(30));
	lights[0].exponent = 3.f;
	lights[0].spotDirection.Set(0.f, 1.f, 0.f);
	
	glUniform1i(m_parameters[U_NUMLIGHTS], 0);
	glUniform1i(m_parameters[U_TEXT_ENABLED], 0);

	glUniform1i(m_parameters[U_LIGHT0_TYPE], lights[0].type);
	glUniform3fv(m_parameters[U_LIGHT0_COLOR], 1, &lights[0].color.r);
	glUniform1f(m_parameters[U_LIGHT0_POWER], lights[0].power);
	glUniform1f(m_parameters[U_LIGHT0_KC], lights[0].kC);
	glUniform1f(m_parameters[U_LIGHT0_KL], lights[0].kL);
	glUniform1f(m_parameters[U_LIGHT0_KQ], lights[0].kQ);
	glUniform1f(m_parameters[U_LIGHT0_COSCUTOFF], lights[0].cosCutoff);
	glUniform1f(m_parameters[U_LIGHT0_COSINNER], lights[0].cosInner);
	glUniform1f(m_parameters[U_LIGHT0_EXPONENT], lights[0].exponent);

	camera.Init(Vector3(0, 0, 1), Vector3(0, 0, 0), Vector3(0, 1, 0));

	for(int i = 0; i < NUM_GEOMETRY; ++i)
	{
		meshList[i] = NULL;
	}
	meshList[GEO_AXES] = MeshBuilder::GenerateAxes("reference", 1000, 1000, 1000);
	meshList[GEO_BALL] = MeshBuilder::GenerateSphere("ball", Color(1, 1, 1), 10, 10, 1.f);
	meshList[GEO_ICE] = MeshBuilder::GenerateCircle("ice", Color(0.5f, 0.5f, 1));
	meshList[GEO_SOCC] = MeshBuilder::GenerateOBJ("GEO_BALL", "Object//ball1.obj");
	meshList[GEO_SOCC]->textureID = LoadTGA("Image//ballUV.tga");
	meshList[GEO_CUBE] = MeshBuilder::GenerateCube("cube", Color(1, 1, 1), 4.f);
	meshList[GEO_TEXT] = MeshBuilder::GenerateText("text", 16, 16);
	meshList[GEO_TEXT]->textureID = LoadTGA("Image//calibri.tga");
	meshList[GEO_TEXT]->material.kAmbient.Set(1, 0, 0);
	meshList[GEO_SHIP] = MeshBuilder::GenerateQuad("ship", Color(1, 1, 1), 2.f);
	meshList[GEO_SHIP]->textureID = LoadTGA("Image//12581269711028989317ElectronicRU_Rocket-Picture.svg.hi.tga");
	meshList[GEO_BLACKHOLE] = MeshBuilder::GenerateSphere("ball", Color(0, 0, 0), 10, 10, 1.f);
	meshList[GEO_ENEMY] = MeshBuilder::GenerateQuad("ship", Color(1, 1, 1), 2.f);
	meshList[GEO_ENEMY]->textureID = LoadTGA("Image//enemyship.tga");
	meshList[GEO_ENEMYBULLET] = MeshBuilder::GenerateSphere("ball", Color(1, 0, 0), 10, 10, 1.f);
	meshList[GEO_CAR1] = MeshBuilder::GenerateOBJ("GEO_CAR1", "Object//car1basicfix.obj");
	meshList[GEO_CAR1]->textureID = LoadTGA("Image//car1.tga");
	meshList[GEO_CAR2] = MeshBuilder::GenerateOBJ("GEO_CAR2", "Object//car1basicfix.obj");
	meshList[GEO_CAR2]->textureID = LoadTGA("Image//car2.tga");
	meshList[GEO_POST1] = MeshBuilder::GenerateSphere("ball1", Color(255, 144, 0), 10, 10, 1.f);
	meshList[GEO_POST2] = MeshBuilder::GenerateSphere("ball2", Color(255, 0, 228), 10, 10, 1.f);
	meshList[GEO_GOAL1] = MeshBuilder::GenerateCube("cube1", Color(255, 144, 0), 1.f);
	meshList[GEO_GOAL2] = MeshBuilder::GenerateCube("cube2", Color(255, 0, 228), 1.f);
	meshList[GEO_POWERUP] = MeshBuilder::GenerateQuad("powerup", Color(1, 1, 1), 2.f);
	meshList[GEO_POWERUP]->textureID = LoadTGA("Image//yellow-orange-bubble-clip-art.tga");
	meshList[GEO_SAWBLADE] = MeshBuilder::GenerateQuad("sawblade", Color(1, 1, 1), 2.f);
	meshList[GEO_SAWBLADE]->textureID = LoadTGA("Image//sawblade.tga");

	// SP 3 STUFF
	
	meshList[GEO_BACKGROUND] = MeshBuilder::GenerateQuad("TESTMAPBACKGROUND", Color(1, 1, 1), 1.f);
	meshList[GEO_BACKGROUND]->textureID = LoadTGA("Image//bg_calm_ocen_water.tga");
	meshList[GEO_TESTMAP] = MeshBuilder::GenerateQuad("TESTMAP", Color(1, 1, 1), 10.f);
	meshList[GEO_TESTMAP]->textureID = LoadTGA("Image//grass_lightblue.tga");
	meshList[GEO_TESTMAP2] = MeshBuilder::GenerateQuad("TESTMAP2", Color(1, 1, 1), 10.f);
	meshList[GEO_TESTMAP2]->textureID = LoadTGA("Image//grass_lightbrown.tga");
	meshList[GEO_TREETOP] = MeshBuilder::GenerateQuad("treetop", Color(1, 1, 1), 2.f);
	meshList[GEO_TREETOP]->textureID = LoadTGA("Image//treetop.tga");
	meshList[GEO_ROCK] = MeshBuilder::GenerateQuad("rock", Color(1, 1, 1), 1.f);
	meshList[GEO_ROCK]->textureID = LoadTGA("Image//rock.tga");
	meshList[GEO_MUD] = MeshBuilder::GenerateCircle("mud", Color(0.51f, 0.219f,0.101f));
	meshList[GEO_OBJECTIVE] = MeshBuilder::GenerateQuad("robjective", Color(1, 1, 1), 1.f);
	meshList[GEO_OBJECTIVE]->textureID = LoadTGA("Image//objective.tga");

	meshList[GEO_LION] = MeshBuilder::GenerateQuad("lion", Color(1, 1, 1), 1.f);
	meshList[GEO_LION]->textureID = LoadTGA("Image//animal_lion.tga");
	meshList[GEO_ZEBRA] = MeshBuilder::GenerateQuad("zebra", Color(1, 1, 1), 1.f);
	meshList[GEO_ZEBRA]->textureID = LoadTGA("Image//animal_zebra.tga");
	meshList[GEO_RHINO] = MeshBuilder::GenerateQuad("rhino", Color(1, 1, 1), 1.f);
	meshList[GEO_RHINO]->textureID = LoadTGA("Image//animal_rhino.tga");
	meshList[GEO_HUMAN] = MeshBuilder::GenerateQuad("human", Color(1, 1, 1), 1.f);
	meshList[GEO_HUMAN]->textureID = LoadTGA("Image//animal_human.tga");
	//meshList[GEO_JEEP] = MeshBuilder::GenerateOBJ("GEO_JEEP", "Object//jurrassiccar.obj");
	//meshList[GEO_JEEP]->textureID = LoadTGA("Image//jeep.tga");
	meshList[HUD_MAPEDITOR] = MeshBuilder::GenerateQuad("gui", Color(1, 1, 1), 1.f);
	meshList[HUD_MAPEDITOR]->textureID = LoadTGA("Image//editorui.tga");
	meshList[HUD_MAPEDITOR2] = MeshBuilder::GenerateQuad("gui2", Color(1, 1, 1), 1.f);
	meshList[HUD_MAPEDITOR2]->textureID = LoadTGA("Image//editorui2.tga");
	meshList[HUD_MAPEDITOR3] = MeshBuilder::GenerateQuad("gui3", Color(1, 1, 1), 1.f);
	meshList[HUD_MAPEDITOR3]->textureID = LoadTGA("Image//editorui3.tga");
	meshList[HUD_TESTMODE] = MeshBuilder::GenerateQuad("testmode", Color(1, 1, 1), 1.f);
	meshList[HUD_TESTMODE]->textureID = LoadTGA("Image//testmode.tga");
	meshList[HUD_DELETEICON] = MeshBuilder::GenerateQuad("del", Color(1, 1, 1), 1.f);
	meshList[HUD_DELETEICON]->textureID = LoadTGA("Image//deleteicon.tga");
	meshList[HUD_SELECTION] = MeshBuilder::GenerateCube("selection", Color(1, 0, 0), 1.f);
	meshList[HUD_GRIDLOCK] = MeshBuilder::GenerateQuad("grid", Color(1, 1, 1), 1.f);
	meshList[HUD_GRIDLOCK]->textureID = LoadTGA("Image//gridlock.tga");
	meshList[HUD_RADAR] = MeshBuilder::GenerateQuad("radar", Color(1, 1, 1), 1.f);
	meshList[HUD_RADAR]->textureID = LoadTGA("Image//radar2.tga");
	meshList[HUD_RADARLINE] = MeshBuilder::GenerateCube("radarline", Color(1, 118.f / 255.f, 247.f / 255.f), 4.f);
	meshList[HUD_RADARDETECT] = MeshBuilder::GenerateCircle("detect", Color(0, 1, 0));
	meshList[HUD_RADARENEMY] = MeshBuilder::GenerateCircle("detectene", Color(1, 0, 0));
	//meshList[HUD_RADARDETECT]->textureID = LoadTGA("Image//yellow-orange-bubble-clip-art.tga");

	meshList[GEO_TESTENEMY] = MeshBuilder::GenerateSphere("enemy", Color(255, 144, 0), 10, 10, 1.f);

	meshList[GEO_MENU_BACKGROUND] = MeshBuilder::GenerateQuad("Intro", Color(1, 1, 1), 1.f);
	meshList[GEO_MENU_BACKGROUND]->textureID = LoadTGA("Image//IntroScreen.tga");
	meshList[GEO_MENU_MAPEDITOR] = MeshBuilder::GenerateQuad("mapeditor", Color(1, 1, 1), 1.f);
	meshList[GEO_MENU_MAPEDITOR]->textureID = LoadTGA("Image//menu_MapEditor.tga");
	meshList[GEO_MENU_SIGNBOARD] = MeshBuilder::GenerateQuad("signboard", Color(1, 1, 1), 2.f);
	meshList[GEO_MENU_SIGNBOARD]->textureID = LoadTGA("Image//noticeboard.tga");
	meshList[GEO_MENU_ARROW] = MeshBuilder::GenerateQuad("arrow", Color(1, 1, 1), 2.f);
	meshList[GEO_MENU_ARROW]->textureID = LoadTGA("Image//arrow.tga");
	meshList[GEO_MENU_STARTGAME] = MeshBuilder::GenerateQuad("menu_startgame", Color(1, 1, 1), 2.f);
	meshList[GEO_MENU_STARTGAME]->textureID = LoadTGA("Image//menu_StartGame.tga");
	meshList[GEO_MENU_INSTRUCTIONS] = MeshBuilder::GenerateQuad("menu_instructions", Color(1, 1, 1), 2.f);
	meshList[GEO_MENU_INSTRUCTIONS]->textureID = LoadTGA("Image//menu_Instructions.tga");
	meshList[GEO_MENU_INSTRUCTIONS_PAGE] = MeshBuilder::GenerateQuad("menu_instructions_page", Color(1, 1, 1), 1.f);
	meshList[GEO_MENU_INSTRUCTIONS_PAGE]->textureID = LoadTGA("Image//menu_Instructions_page.tga");
	meshList[GEO_MENU_OPTIONS] = MeshBuilder::GenerateQuad("menu_options", Color(1, 1, 1), 2.f);
	meshList[GEO_MENU_OPTIONS]->textureID = LoadTGA("Image//menu_Options.tga");
	meshList[GEO_MENU_HIGHSCORE] = MeshBuilder::GenerateQuad("menu_highscore", Color(1, 1, 1), 2.f);
	meshList[GEO_MENU_HIGHSCORE]->textureID = LoadTGA("Image//menu_Highscore.tga");
	meshList[GEO_MENU_QUIT] = MeshBuilder::GenerateQuad("menu_quit", Color(1, 1, 1), 2.f);
	meshList[GEO_MENU_QUIT]->textureID = LoadTGA("Image//menu_Quit.tga");

	meshList[GEO_UPGRADE_BACKGROUND] = MeshBuilder::GenerateQuad("upgrade_background", Color(1, 1, 1), 1.f);
	meshList[GEO_UPGRADE_BACKGROUND]->textureID = LoadTGA("Image//UpgradeScene.tga");
	meshList[GEO_UPGRADE_UI_CARS] = MeshBuilder::GenerateQuad("upgrade_ui_cars", Color(1, 1, 1), 1.f);
	meshList[GEO_UPGRADE_UI_CARS]->textureID = LoadTGA("Image//Upgrade_Cars.tga");
	meshList[GEO_UPGRADE_UI_TIRES] = MeshBuilder::GenerateQuad("upgrade_ui_tires", Color(1, 1, 1), 1.f);
	meshList[GEO_UPGRADE_UI_TIRES]->textureID = LoadTGA("Image//Upgrade_Tires.tga");
	meshList[GEO_UPGRADE_UI_LASSO] = MeshBuilder::GenerateQuad("upgrade_ui_lasso", Color(1, 1, 1), 1.f);
	meshList[GEO_UPGRADE_UI_LASSO]->textureID = LoadTGA("Image//Upgrade_Lasso.tga");
	meshList[GEO_UPGRADE_UI_DARTS] = MeshBuilder::GenerateQuad("upgrade_ui_darts", Color(1, 1, 1), 1.f);
	meshList[GEO_UPGRADE_UI_DARTS]->textureID = LoadTGA("Image//Upgrade_Darts.tga");

	meshList[GEO_LEVELSELECT] = MeshBuilder::GenerateQuad("levelselect", Color(1, 1, 1), 1.f);
	meshList[GEO_LEVELSELECT]->textureID = LoadTGA("Image//LevelSelection.tga");
	meshList[GEO_LEVELSELECT_CUSTOM] = MeshBuilder::GenerateQuad("customlevelselect", Color(1, 1, 1), 1.f);
	meshList[GEO_LEVELSELECT_CUSTOM]->textureID = LoadTGA("Image//CustomLevelSelection.tga");

	meshList[GEO_DISPLAY_CAR1] = MeshBuilder::GenerateOBJ("display_car1", "Object//jurrassiccar.obj");
	meshList[GEO_DISPLAY_CAR1]->textureID = LoadTGA("Image//jeep.tga");
	meshList[GEO_DISPLAY_CAR2] = MeshBuilder::GenerateOBJ("display_car2", "Object//jeep2.obj");
	meshList[GEO_DISPLAY_CAR2]->textureID = LoadTGA("Image//jeep2.tga");
	meshList[GEO_DISPLAY_CAR3] = MeshBuilder::GenerateOBJ("display_car3", "Object//jeep3.obj");
	meshList[GEO_DISPLAY_CAR3]->textureID = LoadTGA("Image//jeep3.tga");
	meshList[GEO_DISPLAY_WHEEL] = MeshBuilder::GenerateOBJ("display_wheel", "Object//wheels.obj");
	meshList[GEO_DISPLAY_WHEEL]->textureID = LoadTGA("Image//wheels.tga");
	meshList[GEO_DISPLAY_ROPE] = MeshBuilder::GenerateOBJ("display_rope", "Object//wheels.obj");
	meshList[GEO_DISPLAY_ROPE]->textureID = LoadTGA("Image//wheels.tga");
	meshList[GEO_DISPLAY_DARTS] = MeshBuilder::GenerateOBJ("display_wheel", "Object//darts.obj");
	meshList[GEO_DISPLAY_DARTS]->textureID = LoadTGA("Image//darts.tga");

	meshList[GEO_BUY] = MeshBuilder::GenerateQuad("buyButton", Color(1, 1, 1), 1.f);
	meshList[GEO_BUY]->textureID = LoadTGA("Image//buyButton.tga");
	meshList[GEO_SOLD] = MeshBuilder::GenerateQuad("sold", Color(1, 1, 1), 1.f);
	meshList[GEO_SOLD]->textureID = LoadTGA("Image//SOLD.tga");

	meshList[GEO_LOSE_SCENE] = MeshBuilder::GenerateQuad("loseScreen", Color(1, 1, 1), 1.f);
	meshList[GEO_LOSE_SCENE]->textureID = LoadTGA("Image//loseScreen.tga");

	meshList[GEO_ROPE] = MeshBuilder::GenerateCube("rope", Color(1, 1, 0), 1.f);

	bLightEnabled = false;
}

void SceneBase::Update(double dt)
{
	//Keyboard Section
	/*if(Application::IsKeyPressed('1'))
		glEnable(GL_CULL_FACE);
	if(Application::IsKeyPressed('2'))
		glDisable(GL_CULL_FACE);
	if(Application::IsKeyPressed('3'))
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	if(Application::IsKeyPressed('4'))
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);*/
	
	fps = (float)(1.f / dt);
}

void SceneBase::RenderText(Mesh* mesh, std::string text, Color color)
{
	if(!mesh || mesh->textureID <= 0)
		return;
	
	glDisable(GL_DEPTH_TEST);
	glUniform1i(m_parameters[U_TEXT_ENABLED], 1);
	glUniform3fv(m_parameters[U_TEXT_COLOR], 1, &color.r);
	glUniform1i(m_parameters[U_LIGHTENABLED], 0);
	glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 1);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, mesh->textureID);
	glUniform1i(m_parameters[U_COLOR_TEXTURE], 0);
	for(unsigned i = 0; i < text.length(); ++i)
	{
		Mtx44 characterSpacing;
		characterSpacing.SetToTranslation(i * 1.0f, 0, 0); //1.0f is the spacing of each character, you may change this value
		Mtx44 MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top() * characterSpacing;
		glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
	
		mesh->Render((unsigned)text[i] * 6, 6);
	}
	glBindTexture(GL_TEXTURE_2D, 0);
	glUniform1i(m_parameters[U_TEXT_ENABLED], 0);
	glEnable(GL_DEPTH_TEST);
}

void SceneBase::RenderTextOnScreen(Mesh* mesh, std::string text, Color color, float size, float x, float y)
{
	if(!mesh || mesh->textureID <= 0)
		return;

	glDisable(GL_DEPTH_TEST);
	Mtx44 ortho;
	ortho.SetToOrtho(0, 80, 0, 60, -10, 10);
	projectionStack.PushMatrix();
	projectionStack.LoadMatrix(ortho);
	viewStack.PushMatrix();
	viewStack.LoadIdentity();
	modelStack.PushMatrix();
	modelStack.LoadIdentity();
	modelStack.Translate(x, y, 0);
	modelStack.Scale(size, size, size);
	glUniform1i(m_parameters[U_TEXT_ENABLED], 1);
	glUniform3fv(m_parameters[U_TEXT_COLOR], 1, &color.r);
	glUniform1i(m_parameters[U_LIGHTENABLED], 0);
	glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 1);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, mesh->textureID);
	glUniform1i(m_parameters[U_COLOR_TEXTURE], 0);
	for(unsigned i = 0; i < text.length(); ++i)
	{
		Mtx44 characterSpacing;
		characterSpacing.SetToTranslation(i * 1.0f + 0.5f, 0.5f, 0); //1.0f is the spacing of each character, you may change this value
		Mtx44 MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top() * characterSpacing;
		glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);

		mesh->Render((unsigned)text[i] * 6, 6);
	}
	glBindTexture(GL_TEXTURE_2D, 0);
	glUniform1i(m_parameters[U_TEXT_ENABLED], 0);
	modelStack.PopMatrix();
	viewStack.PopMatrix();
	projectionStack.PopMatrix();
	glEnable(GL_DEPTH_TEST);
}

void SceneBase::RenderMesh(Mesh *mesh, bool enableLight)
{
	Mtx44 MVP, modelView, modelView_inverse_transpose;
	
	MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top();
	glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
	if(enableLight && bLightEnabled)
	{
		glUniform1i(m_parameters[U_LIGHTENABLED], 1);
		modelView = viewStack.Top() * modelStack.Top();
		glUniformMatrix4fv(m_parameters[U_MODELVIEW], 1, GL_FALSE, &modelView.a[0]);
		modelView_inverse_transpose = modelView.GetInverse().GetTranspose();
		glUniformMatrix4fv(m_parameters[U_MODELVIEW_INVERSE_TRANSPOSE], 1, GL_FALSE, &modelView.a[0]);
		
		//load material
		glUniform3fv(m_parameters[U_MATERIAL_AMBIENT], 1, &mesh->material.kAmbient.r);
		glUniform3fv(m_parameters[U_MATERIAL_DIFFUSE], 1, &mesh->material.kDiffuse.r);
		glUniform3fv(m_parameters[U_MATERIAL_SPECULAR], 1, &mesh->material.kSpecular.r);
		glUniform1f(m_parameters[U_MATERIAL_SHININESS], mesh->material.kShininess);
	}
	else
	{	
		glUniform1i(m_parameters[U_LIGHTENABLED], 0);
	}
	if(mesh->textureID > 0)
	{
		glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 1);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, mesh->textureID);
		glUniform1i(m_parameters[U_COLOR_TEXTURE], 0);
	}
	else
	{
		glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 0);
	}
	mesh->Render();
	if(mesh->textureID > 0)
	{
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}

void SceneBase::Render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void SceneBase::Exit()
{
	// Cleanup VBO
	for(int i = 0; i < NUM_GEOMETRY; ++i)
	{
		if(meshList[i])
			delete meshList[i];
	}
	glDeleteProgram(m_programID);
	glDeleteVertexArrays(1, &m_vertexArrayID);
}
