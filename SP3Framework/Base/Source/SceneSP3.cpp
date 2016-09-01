#include "SceneSP3.h"
#include "GL\glew.h"
#include "Application.h"
#include <sstream>

SceneSP3::SceneSP3() : theSoundEngine(NULL), Sound_Engine(NULL), Sound_Throw(NULL), Sound_Snap(NULL), Sound_Ding(NULL), Sound_Bump(NULL), Sound_Back(NULL)
{
}

SceneSP3::~SceneSP3()
{
	
}
string NameofMap;
void SceneSP3::Init()
{
	SceneBase::Init();

	//Physics code here
	m_speed = 1.f;

	Math::InitRNG();

	//Exercise 1: initialize m_objectCount
	m_objectCount = 0;

	m_ghost = new GameObject(GameObject::GO_BALL);

	player1 = new GameObject(GameObject::GO_CAR);
	player1->pos.Set(25, 50, 0);
	player1->mass = 5;
	player1->vel.Set(0, 0, 0);
	player1->rotationAngle = 0;
	player1->normal = Vector3(cos(Math::DegreeToRadian(player1->rotationAngle)), sin(Math::DegreeToRadian(player1->rotationAngle)), 0);
	player1->rotatedVel.SetToRotation(player1->rotationAngle, 0, 0, 1);
	player1->engine = 0;
	player1->turnPower = 3;
	player1->active = true;
	player1->scale.Set(10, 5, 1);

	player1->playerCar.engine = 4;
	player1->playerCar.mass = 5;
	player1->playerCar.topSpeed = 40;
	player1->playerCar.turnSpeed = 3;
	player1->playerCar.acceleration = 0.5;
	player1->playerCar.powerUp = 2;
	m_goList.push_back(player1);

	car1Bought = true;
	car2Bought = false;
	car3Bought = false;
	tire1Bought = true;
	tire2Bought = false;
	tire3Bought = false;
	lasso1Bought = true;
	lasso2Bought = false;
	lasso3Bought = false;
	dart1Bought = true;
	dart2Bought = false;
	dart3Bought = false;

	friction = 0.95f;
	diffx = 0;
	diffy = 0;
	testMode = false;
	panel2 = false;
	panel2x = 15;
	deleteMode = 0;
	time = 0;
	arrowSelection = 0;
	rotateDisplayX = 0;
	rotateDisplayY = 0;
	objective = NULL;
	center = (0, 0, 0);
	updateObjective = 0;
	fuelAmount = 100.0f;
	leader = NULL;
	snapSet = 0;
	money = 100;
	cost[0] = 100; //car2
	cost[1] = 200; //car3
	cost[2] = 300; //tire2
	cost[3] = 400; //tire3
	cost[4] = 500; //lasso2
	cost[5] = 600; //lasso3
	cost[6] = 700; //darts2
	cost[7] = 800; //darts3
	sound = 1;
	highQ = 0;
	dartMax = 5;
	dartCount = 5;
	dartROF = 1;
	EventType = 0;

	paused = false;
	scroll = 0;
	scrollimit = 275;

	loadMapStuff = 0;


	mapPosition = Vector3(m_worldWidth / 2, m_worldHeight / 2, 0);
	testMap.setBackground(meshList[GEO_TESTMAP2]);
	testMap.setMapSize(30, 20);

	gameStates = states::s_Upgrade;
	

	animalStat = new TextFile(TextFile::ANIMAL);

	theSoundEngine = createIrrKlangDevice();
	if (!theSoundEngine)
	{
	}
	engineSound();
	backSound();
	if (sound == 0 && Sound_Back != NULL)
	{
		Sound_Back->setIsPaused(true);
	}
	/*for (int i = 0; i < 7; ++i)
	{
		animalStat->GetAnimalStat("Zebra");
		enemy* animal = newEnemy(Math::RandFloatMinMax(-100, 100), Math::RandFloatMinMax(-100, 100), 0, 0, animalStat->get_stamina(), animalStat->get_speed(), animalStat->get_strength());
		animal->setVel(Math::RandFloatMinMax(-10, 10), Math::RandFloatMinMax(-10, 10), 0);
		animal->setLeader(0);
		enemyList.push_back(animal);
	}

	for (int i = 0; i < 3; ++i)
	{
		animalStat->GetAnimalStat("Rhino");
		enemy* animal = newEnemy(Math::RandFloatMinMax(-100, 100), Math::RandFloatMinMax(-100, 100), 0, 1, animalStat->get_stamina(), animalStat->get_speed(), animalStat->get_strength());
		animal->setVel(Math::RandFloatMinMax(-10, 10), Math::RandFloatMinMax(-10, 10), 0);
		animal->setLeader(0);
		enemyList.push_back(animal);
	}
	animalStat->GetAnimalStat("Lion");
	enemy* animal = newEnemy(Math::RandFloatMinMax(-100, 100), Math::RandFloatMinMax(-100, 100), 0, 2, animalStat->get_stamina(), animalStat->get_speed(), animalStat->get_strength());
	animal->setVel(Math::RandFloatMinMax(-10, 10), Math::RandFloatMinMax(-10, 10), 0);
	animal->setLeader(0);
	enemyList.push_back(animal);*/
	InitCarStat("Car1", "CarStats.txt");

	TextFile *car = new TextFile();
	car->SaveFile("CarStats.txt", "tempsave.txt");
	car->SaveFile("tempsave.txt", "SavedCarStats.txt");

	fogActive = false;
	EventActive = false;
	Cooldown = 0;
	Timer = 0.f;
	seconds = 0;
	EventSeconds = 0;
}
void SceneSP3::InitCarStat(string varname, string fileName)
{
	TextFile* var = new TextFile();
	var->GetCarStat(varname, fileName);
	player1->playerCar.engine = var->get_engine();
	player1->playerCar.hp= var->get_hp();
	player1->playerCar.handling = var->get_handling();
	player1->playerCar.lassoLength = var->get_lassolength();
	player1->playerCar.lassoSpeed = var->get_lassostrength();
	player1->playerCar.tranqCount = var->get_tranqcount();
	player1->playerCar.tranqDuration = var->get_tranqduration();
	cout << "bought" << endl;
}
void SceneSP3::Reset()
{
	//Physics code here
	m_speed = 1.f;

	Math::InitRNG();

	//Exercise 1: initialize m_objectCount
	m_objectCount = 0;

	m_ghost = new GameObject(GameObject::GO_BALL);
}

void SceneSP3::engineSound()
{
	if (Sound_Engine == NULL)
	{
		Sound_Engine = theSoundEngine->play2D("Sound/vroom.mp3", true, true);
	}
	//if (Sound_Engine->getIsPaused() == true)
	//{
	//	Sound_Engine->setIsPaused(false);
	//}
	//else if (Sound_Engine->isFinished())
	//{
	//	Sound_Engine = NULL;
	//}
}

void SceneSP3::throwSound()
{
	if (Sound_Throw == NULL)
	{
		Sound_Throw = theSoundEngine->play2D("Sound/throw.mp3", false, false);
	}
}

void SceneSP3::snapSound()
{
	if (Sound_Snap == NULL)
	{
		Sound_Snap = theSoundEngine->play2D("Sound/snap.mp3", false, false);
	}
}

void SceneSP3::dingSound()
{
	if (Sound_Ding == NULL)
	{
		Sound_Ding = theSoundEngine->play2D("Sound/ding.mp3", false, false);
	}
}

void SceneSP3::bumpSound()
{
	if (Sound_Bump == NULL)
	{
		Sound_Bump = theSoundEngine->play2D("Sound/bump.mp3", false, false);
	}
}

void SceneSP3::backSound()
{
	if (Sound_Back == NULL)
	{
		Sound_Back = theSoundEngine->play2D("Sound/song.mp3", true, true);
	}
	/*if (Sound_Back->getIsPaused() == true)
	{
		Sound_Back->setIsPaused(false);
	}
	else if (Sound_Back->isFinished())
	{
		Sound_Back = NULL;
	}*/
}

void SceneSP3::eraseEnemy()
{
	for (std::vector<enemy *>::iterator it = enemyList.begin(); it != enemyList.end();)
	{
		enemy *go = (enemy *)*it;
		it = enemyList.erase(it++);
	}
}


GameObject* SceneSP3::FetchGO()
{
	//Exercise 2a: implement FetchGO()

	for (std::vector<GameObject*>::iterator iter = m_goList.begin(); iter != m_goList.end(); ++iter)
	{
		GameObject *go = *iter;
		if (go->active == false && (go->type == GameObject::GO_BALL || go->type == GameObject::GO_TRANQ))
		{
			m_objectCount++;
			go->active = true;
			return go;
		}
	}
	for (unsigned int i = 0; i < 10; ++i)
	{
		m_goList.push_back(new GameObject(GameObject::GO_BALL));
	}
	GameObject *go = m_goList.back();
	go->active = true;

	//Exercise 2b: increase object count every time an object is set to active
	m_objectCount++;
	return go;
}

bool SceneSP3::CheckCollision(GameObject *go, GameObject *other, double dt)
{
	float distanceSquared = ((other->pos + other->vel * dt) - (go->pos + go->vel * dt)).LengthSquared();
	float combinedRadiusSquared = (go->scale.x + other->scale.x) * (go->scale.x + other->scale.x);
	Vector3 relativeVelocity = go->vel - other->vel;
	Vector3 relativeDisplacement = other->pos - go->pos;
	return (distanceSquared < combinedRadiusSquared && relativeVelocity.Dot(relativeDisplacement) > 0);
}

float SceneSP3::CheckCollision2(GameObject *go1, GameObject *go2)
{
	Vector3 rel = go1->vel - go2->vel;
	Vector3 dir = go1->pos = go2->pos;
	float r = go1->scale.x + go2->scale.x;
	float dot = rel.Dot(dir);
	if (dot > 0)
		return -1;
	float a = rel.LengthSquared();
	float b = 2 * rel.Dot(dir);
	float c = dir.LengthSquared() - r * r;
	float d = b * b - 4 * a * c;
	if (d < 0)
		return -1;
	float t = (-b - sqrt(d)) / (2 * a);
	if (t < 0)
		float t = (-b + sqrt(d)) / (2 * a);
	return t;
}

bool SceneSP3::CheckCollision3(GameObject *go, GameObject *other, double dt)
{
	switch (other->type)
	{
	case GameObject::GO_BALL:
	{
								float distanceSquared = ((go->pos + go->vel * dt) - (other->pos - other->vel * dt)).LengthSquared();
								float combinedRadiusSquared = (go->scale.x + other->scale.x) * (go->scale.x + other->scale.x);
								Vector3 relativeVelocity = go->vel - other->vel;
								Vector3 relativeDisplacement = other->pos - go->pos;
								return (distanceSquared < combinedRadiusSquared && relativeVelocity.Dot(relativeDisplacement) > 0);
								break;
	}
	case GameObject::GO_SAWBLADE:
	{
									float distanceSquared = ((go->pos + go->vel * dt) - (other->pos - other->vel * dt)).LengthSquared();
									float combinedRadiusSquared = (go->scale.x + other->scale.x) * (go->scale.x + other->scale.x);
									Vector3 relativeVelocity = go->vel - other->vel;
									Vector3 relativeDisplacement = other->pos - go->pos;
									return (distanceSquared < combinedRadiusSquared && relativeVelocity.Dot(relativeDisplacement) > 0);
									break;
	}
	case GameObject::GO_WALL:
	{
								Vector3 w0 = other->pos;
								Vector3 b1 = go->pos + go->vel * dt;
								Vector3 N = other->normal;
								//Vector3 NP = N.Cross(Vector3(0, 0, 1));
								Vector3 NP(-N.y, N.x);
								float r = go->scale.x;
								float h = other->scale.x;
								float l = other->scale.y;
	
								Vector3 relativeVelocity = go->vel - other->vel;
								Vector3 relativeDisplacement = w0 - b1;
	
								if (relativeDisplacement.Dot(N) < 0)
									N = -N;
	
								if (relativeVelocity.Dot(N) > 0)
									return ((abs((w0 - b1).Dot(N)) < (r + h / 2)) && ((abs(relativeDisplacement.Dot(NP)) < (l / 2))));
								break;
	}
	//case GameObject::GO_GOAL:
	//{
	//							if (go->pos.x - go->scale.x > other->pos.x - other->scale.x / 2 && go->pos.x + go->scale.x < other->pos.x + other->scale.x / 2)
	//							{
	//								if (go->pos.y - go->scale.y > other->pos.y - other->scale.y / 2 && go->pos.y + go->scale.y < other->pos.y + other->scale.y / 2)
	//								{
	//									return true;
	//								}
	//							}
	//							return false;
	//							break;
	//}
	case GameObject::GO_PILLAR:
	{
								  float distanceSquared = ((go->pos + go->vel * dt) - (other->pos - other->vel * dt)).LengthSquared();
								  float combinedRadiusSquared = (go->scale.x + other->scale.x) * (go->scale.x + other->scale.x);
								  Vector3 relativeDisplacement = other->pos - go->pos;
								  return (distanceSquared < combinedRadiusSquared && go->vel.Dot(relativeDisplacement) > 0);
								  break;
	}
	case GameObject::GO_CAR:
	{
							   //x’ = cos(theta) * (cx – originX) – sin(theta) * (cy – originY) + originX
							   float b1 = cos(Math::DegreeToRadian(-other->rotationAngle)) * (go->pos.x - other->pos.x) - sin(Math::DegreeToRadian(-other->rotationAngle)) * (go->pos.y - other->pos.y) + other->pos.x;
							   //y’ = sin(theta) * (cx – originX) + cos(theta) * (cy – originY) + originY
							   float b2 = sin(Math::DegreeToRadian(-other->rotationAngle)) * (go->pos.x - other->pos.x) + cos(Math::DegreeToRadian(-other->rotationAngle)) * (go->pos.y - other->pos.y) + other->pos.y;

							   float leftRight = 0;
							   float downUp = 0;
							   if (b1 < other->pos.x - (other->scale.x / 2))
							   {
								   leftRight = -1;
							   }
							   else if (b1 > other->pos.x + (other->scale.x / 2))
							   {
								   leftRight = 1;
							   }
							   else if (b1 < other->pos.x + (other->scale.x / 2) || go->pos.x > other->pos.x - (other->scale.x / 2))
							   {
								   leftRight = 3;
							   }
							   if (b2 > other->pos.y + (other->scale.y / 2))
							   {
								   downUp = 1;
							   }
							   else if (b2 < other->pos.y - (other->scale.y / 2))
							   {
								   downUp = -1;
							   }
							   else if (b2 < other->pos.y + (other->scale.y / 2) || go->pos.y > other->pos.y - (other->scale.y / 2))
							   {
								   downUp = 3;
							   }

							   bool pointCollide = false;
							   if (leftRight != 0 && leftRight != 3 && downUp != 0 && downUp != 3)
							   {
								   pointCollide = true;
							   }

							   if (pointCollide)
							   {
								   Vector3 NP = Vector3(-other->normal.y, other->normal.x, 0);
								   Vector3 w0 = other->pos + (leftRight * (other->scale.x * 0.5f) * other->normal.Normalized()) + (downUp * (other->scale.y * 0.5f) * NP.Normalized());
								   float distanceSquared = ((go->pos + go->vel * dt) - (w0 - other->vel * dt)).LengthSquared();
								   float combinedRadiusSquared = (go->scale.x + 0.5f) * (go->scale.x + 0.5f);
								   Vector3 relativeVelocity = go->vel - other->vel;
								   Vector3 relativeDisplacement = w0 - go->pos;
								   if (distanceSquared < combinedRadiusSquared && relativeVelocity.Dot(relativeDisplacement) > 0)
								   {
									   u1 = go->vel;

									   Vector3 N = (other->pos - go->pos).Normalized();

									   go->vel = u1 - 2 * u1.Dot(N) * N;
									   go->vel.z = 0;
								   }
							   }
							   else
							   {
								   if (leftRight == 0 || downUp == 0)
								   {
									   std::cout << "LOST BALL" << std::endl;
								   }
								   if (leftRight == 3 && downUp == 3)
								   {
									   //std::cout << "BALL INSIDE" << std::endl;
									   go->vel += other->vel;
									   return false;
								   }
								   if (downUp == 3)
								   {

									   Vector3 w0 = other->pos;
									   Vector3 b1 = go->pos + go->vel * dt;
									   Vector3 N = other->normal;
									   //Vector3 NP = N.Cross(Vector3(0, 0, 1));
									   Vector3 NP(-N.y, N.x);
									   float r = go->scale.x;
									   float h = other->scale.x;
									   float l = other->scale.y;

									   Vector3 relativeVelocity = go->vel - other->vel;
									   Vector3 relativeDisplacement = w0 - b1;

									   if (relativeDisplacement.Dot(N) < 0)
										   N = -N;

									   if (relativeVelocity.Dot(N) > 0)
									   {
										   if ((abs((w0 - b1).Dot(N)) < (r + h / 2)) && ((abs(relativeDisplacement.Dot(NP)) < (l / 2))))
										   {
											   Vector3 u = go->vel;
											   Vector3 N = other->normal;
											   Vector3 uN = u.Dot(N) * N;
											   go->vel = u - 2 * uN;
											   go->vel += other->vel;
										   }
									   }
								   }
								   else if (leftRight == 3)
								   {

									   Vector3 w0 = other->pos;
									   Vector3 b1 = go->pos + go->vel * dt;
									   Vector3 NP = other->normal;
									   //Vector3 NP = N.Cross(Vector3(0, 0, 1));
									   Vector3 N(-NP.y, NP.x);
									   float r = go->scale.x;
									   float h = other->scale.y;
									   float l = other->scale.x;

									   Vector3 relativeVelocity = go->vel - other->vel;
									   Vector3 relativeDisplacement = w0 - b1;

									   if (relativeDisplacement.Dot(N) < 0)
										   N = -N;

									   if (relativeVelocity.Dot(N) > 0)
									   {
										   if ((abs((w0 - b1).Dot(N)) < (r + h / 2)) && ((abs(relativeDisplacement.Dot(NP)) < (l / 2))))
										   {
											   Vector3 u = go->vel;
											   Vector3 NP = other->normal;
											   Vector3 N(-NP.y, NP.x);
											   Vector3 uN = u.Dot(N) * N;
											   go->vel = u - 2 * uN;
											   go->vel += other->vel;
										   }
									   }
								   }
							   }
							   return false;
							   break;
	}
	}
	return false;
}

void SceneSP3::CollisionResponse(GameObject *go, GameObject *other)
{
	switch (other->type)
	{
	case GameObject::GO_BALL:
	{
								m1 = go->mass;
								m2 = other->mass;
								u1 = go->vel;
								u2 = other->vel;

								Vector3 u1N, u2N, N;
								N = (other->pos - go->pos).Normalized();

								u1N = u1.Dot(N)  * N;
								u2N = u2.Dot(N)  * N;

								go->vel = u1 + (2 * m2) / (m1 + m2) * (u2N - u1N);
								other->vel = u2 + (2 * m1) / (m1 + m2) * (u1N - u2N);
								go->vel.z = 0;
								other->vel.z = 0;

								v1 = go->vel;
								v2 = other->vel;
								break;
	}
	case GameObject::GO_SAWBLADE:
	{
									m1 = go->mass;
									m2 = other->mass;
									u1 = go->vel;
									u2 = other->vel;

									Vector3 u1N, u2N, N;
									N = (other->pos - go->pos).Normalized();

									u1N = u1.Dot(N)  * N;
									u2N = u2.Dot(N)  * N;

									go->vel = u1 + (2 * m2) / (m1 + m2) * (u2N - u1N);
									other->vel = u2 + (2 * m1) / (m1 + m2) * (u1N - u2N);
									other->vel = other->vel.Normalized() * 150.f;
									go->vel.z = 0;
									other->vel.z = 0;

									v1 = go->vel;
									v2 = other->vel;
									break;
	}
	case GameObject::GO_WALL:
	{
								Vector3 u = go->vel;
								Vector3 N = other->normal;
								Vector3 uN = u.Dot(N) * N;
								go->vel = u - 2 * uN;
								break;
	}
	case GameObject::GO_PILLAR:
	{
								  u1 = go->vel;

								  Vector3 N = (other->pos - go->pos).Normalized();

								  go->vel = u1 - 2 * u1.Dot(N) * N;
								  go->vel.z = 0;
								  break;
	}
	case GameObject::GO_GOAL:
	{
								go->vel = go->pos - other->pos;
								go->scale *= 0.9f;
								if (go->scale.x < 0.2f)
								{
									go->active = false;
									//m_objectCount--;
								}
	}
	}

}

void SceneSP3::CollisionMap(GameObject *go, GameObject *other, double dt)
{
	switch (other->type)
	{
	case GameObject::MAP_TREE:
	{
		float distanceSquared = ((go->pos + go->vel * dt) - (other->pos - other->vel * dt)).LengthSquared();
		float combinedRadiusSquared = (go->scale.x / 3 + other->scale.x) * (go->scale.x / 3 + other->scale.x);
		Vector3 relativeDisplacement = other->pos - go->pos;
		if (distanceSquared < combinedRadiusSquared && go->vel.Dot(relativeDisplacement) > 0)
		{
			 u1 = go->vel;

			 Vector3 N = (other->pos - go->pos).Normalized();

			 go->vel = u1 - u1.Dot(N) * N;
			 go->vel.z = 0;

			 other->active = false;
			 break;
		}
		break;
	}
	case GameObject::MAP_ROCK:
	{
		float distanceSquared = ((go->pos + go->vel * dt) - (other->pos - other->vel * dt)).LengthSquared();
		float combinedRadiusSquared = (go->scale.x / 17 + other->scale.x) * (go->scale.x / 17 + other->scale.x);
		Vector3 relativeDisplacement = other->pos - go->pos;
		if (distanceSquared < combinedRadiusSquared && go->vel.Dot(relativeDisplacement) > 0)
		{
			u1 = go->vel;

			Vector3 N = (other->pos - go->pos).Normalized();

			go->vel = u1 - 2 * u1.Dot(N) * N;
			go->vel.z = 0;
			break;
		}
		break;
	}
	case GameObject::MAP_WATER:
	{
			float distanceSquared = ((go->pos + go->vel * dt) - (other->pos - other->vel * dt)).LengthSquared();
			float combinedRadiusSquared = (go->scale.x + other->scale.x) * (go->scale.x + other->scale.x);
			Vector3 relativeDisplacement = other->pos - go->pos;
			if (distanceSquared < combinedRadiusSquared && go->vel.Dot(relativeDisplacement) > 0)
			{
				friction = 0.99f;
				go->vel *= 0.95f;
			}
			break;
	}
	case GameObject::MAP_MUD:
	{
			float distanceSquared = ((go->pos + go->vel * dt) - (other->pos - other->vel * dt)).LengthSquared();
			float combinedRadiusSquared = (go->scale.x + other->scale.x) * (go->scale.x + other->scale.x);
			Vector3 relativeDisplacement = other->pos - go->pos;
			if (distanceSquared < combinedRadiusSquared && go->vel.Dot(relativeDisplacement) > 0)
			{
				friction = 0.99f;
				go->vel *= 0.97f;
			}
			break;
	}
	}
}

void SceneSP3::playerControl(double dt)
{
	if (Application::IsKeyPressed('W'))
	{
		player1->engine += player1->playerCar.acceleration;
	}
	else if (Application::IsKeyPressed('S'))
	{
		player1->engine = -1;
	}
	else
	{
		player1->engine = 0;
	}
	if (player1->engine > player1->playerCar.engine)
	{
		player1->engine = player1->playerCar.engine;
	}
	if (Application::IsKeyPressed(VK_LSHIFT))
	{
		driftMode = true;
	}
	else
	{
		driftMode = false;
	}
	if (Application::IsKeyPressed('D'))
	{
		player1->pos += Vector3(cos(Math::DegreeToRadian(player1->rotationAngle)) * player1->engine / 4, sin(Math::DegreeToRadian(player1->rotationAngle)) * player1->engine / 4, 0);
		player1->rotationAngle += player1->playerCar.turnSpeed * 3;
		player1->pos -= Vector3(cos(Math::DegreeToRadian(player1->rotationAngle)) * player1->engine / 4, sin(Math::DegreeToRadian(player1->rotationAngle)) * player1->engine / 4, 0);
		player1->rotationAngle -= player1->playerCar.turnSpeed * 3;

		player1->rotationAngle -= player1->playerCar.turnSpeed;
		if (player1->vel.Length() < 5)
		{
			if (9 / player1->vel.Length() < player1->playerCar.turnSpeed)
			{
				player1->rotationAngle += 9 / player1->vel.Length();
			}
			else
			{
				player1->rotationAngle += player1->playerCar.turnSpeed;
			}
				
		}

		player1->normal = Vector3(cos(Math::DegreeToRadian(player1->rotationAngle)), sin(Math::DegreeToRadian(player1->rotationAngle)), 0);
	}
	if (Application::IsKeyPressed('A'))
	{
		player1->pos += Vector3(cos(Math::DegreeToRadian(player1->rotationAngle)) * player1->engine / 4, sin(Math::DegreeToRadian(player1->rotationAngle)) * player1->engine / 4, 0);
		player1->rotationAngle -= player1->playerCar.turnSpeed * 3;
		player1->pos -= Vector3(cos(Math::DegreeToRadian(player1->rotationAngle)) * player1->engine / 4, sin(Math::DegreeToRadian(player1->rotationAngle)) * player1->engine / 4, 0);
		player1->rotationAngle += player1->playerCar.turnSpeed * 3;

		player1->rotationAngle += player1->playerCar.turnSpeed;
		if (player1->vel.Length() < 5)
		{
			if (9 / player1->vel.Length() < player1->playerCar.turnSpeed)
				player1->rotationAngle -= 9 / player1->vel.Length();
			else
				player1->rotationAngle -= player1->playerCar.turnSpeed;
		}

		player1->normal = Vector3(cos(Math::DegreeToRadian(player1->rotationAngle)), sin(Math::DegreeToRadian(player1->rotationAngle)), 0);
	}
	if (player1->vel.Length() < player1->playerCar.topSpeed)
	{
		player1->vel += Vector3(cos(Math::DegreeToRadian(player1->rotationAngle)) * player1->engine, sin(Math::DegreeToRadian(player1->rotationAngle)) * player1->engine, 0);
	}

	if (friction > 0.99f)
	{
		friction = 0.99f;
	}
	else if (friction < 0.95f - player1->mass * 0.01f)
	{
		friction += 0.01f;
	}
	if (friction > 0.96f - player1->mass * 0.01f)
	{
		friction -= 0.01f;
	}

	if (player1->vel.x != 0 || player1->vel.y != 0)
	{
		player1->vel = player1->vel * friction;
		if (driftMode)
		{
			player1->vel = player1->vel * 0.98f;
		}
	}
	Vector3 m_worldBorder = mapPosition - Vector3(m_worldWidth / 2, m_worldHeight / 2, 0);
	if (m_worldBorder.x > testMap.getMapSize().x * 5 && m_worldBorder.x < 100000 && m_worldBorder.x > -100000)
	{
		//cout << "LEFT" << endl;
		player1->pos.x += m_worldBorder.x - testMap.getMapSize().x * 5;
	}
	else if (m_worldBorder.x < -testMap.getMapSize().x * 5 && m_worldBorder.x < 100000 && m_worldBorder.x > -100000)
	{
		//cout << "RIGHT" << endl;
		player1->pos.x -= -m_worldBorder.x - testMap.getMapSize().x * 5;
	}
	if (m_worldBorder.y > testMap.getMapSize().y * 5 && m_worldBorder.y < 100000 && m_worldBorder.y > -100000)
	{
		//cout << "DOWN" << endl;
		player1->pos.y += m_worldBorder.y - testMap.getMapSize().y * 5;
	}
	else if (m_worldBorder.y < -testMap.getMapSize().y * 5 && m_worldBorder.y < 100000 && m_worldBorder.y > -100000)
	{
		//cout << "UP" << endl;
		player1->pos.y += testMap.getMapSize().y * 5 + m_worldBorder.y;
	}
	//cout << player1->pos << " mappos " << mapPosition - Vector3(m_worldWidth / 2, m_worldHeight / 2, 0) << endl;
	static bool shotsFired = false;
	if (Application::IsKeyPressed(VK_SPACE) && shotsFired == false)
	{
		shotsFired = true;
		if (dartCount > 0 && dartROF <= 0)
		{
			GameObject* tranq = FetchGO();
			tranq->type = GameObject::GO_TRANQ;
			//GameObject* tranq = new GameObject(GameObject::GO_TRANQ);
			tranq->pos.Set(player1->pos.x, player1->pos.y, 1);
			tranq->pos += Vector3(cos(Math::DegreeToRadian(player1->rotationAngle)) * player1->scale.y, sin(Math::DegreeToRadian(player1->rotationAngle)) * player1->scale.y, 0);
			tranq->vel = Vector3(cos(Math::DegreeToRadian(player1->rotationAngle)), sin(Math::DegreeToRadian(player1->rotationAngle)), 0).Normalized() * 75;
			tranq->active = true;
			//m_goList.push_back(tranq);
			dartROF = 1 / (3 * dt);
			dartCount--;
		}
	}
	else if (!Application::IsKeyPressed(VK_SPACE) && shotsFired == true)
	{
		shotsFired = false;
	}
}

void SceneSP3::Update(double dt)
{
	SceneBase::Update(dt);
	time--;
	if (Application::IsKeyPressed('5'))
	{
		testMap.setBackground(meshList[GEO_TESTMAP]);
	}
	else if (Application::IsKeyPressed('6'))
	{
		testMap.setBackground(meshList[GEO_TESTMAP2]);
	}
	if (Application::IsKeyPressed('V'))
	{
		testMap.optimize();
	}

	diffx = (m_worldWidth / 2) - player1->pos.x;
	diffy = (m_worldHeight / 2) - player1->pos.y;
	mapPosition = Vector3(mapPosition.x + diffx, mapPosition.y + diffy, 1);
	player1->pos.x = (m_worldWidth / 2);
	player1->pos.y = (m_worldHeight / 2);

	static bool WPressed = false;
	static bool UpPressed = false;

	//std::cout<<zebra->getPos()<<std::endl;


	if (updateObjective == 0)
	{
		center += mapPosition;
		updateObjective = 1;
	}
	else
	{
		center.x += diffx;
		center.y += diffy;
	}

	if (Application::IsKeyPressed('R'))
	{
		GameObject* testTree = new GameObject(GameObject::MAP_TREE);
		testTree->pos.Set(0, 0, 1);
		testTree->fresh = true;
		testTree->active = true;
		testMap.addClusterProp(testTree);

		GameObject* testWater = new GameObject(GameObject::MAP_WATER);
		testWater->pos.Set(25, 25, 1);
		testWater->fresh = true;
		testWater->active = true;
		testMap.addSingleProp(testWater);

		GameObject* testMud = new GameObject(GameObject::MAP_MUD);
		testMud->pos.Set(-25, -25, 1);
		testMud->scale.Set(4, 4, 4);
		testMud->fresh = true;
		testMud->active = true;
		testMap.addSingleProp(testMud);

		GameObject* testRock = new GameObject(GameObject::MAP_ROCK);
		testRock->pos.Set(-25, 25, 1);
		testRock->scale.Set(6, 6, 6);
		testRock->fresh = true;
		testRock->active = true;
		testMap.addClusterProp(testRock);

		testMap.addBorder();

		animalStat->SetScore(Math::RandInt(), "levelwat");
		animalStat->ListHighScore("levelwat");
	}
	if (Application::IsKeyPressed('T'))
	{
		player1->vel.SetZero();
		gameStates = states::s_MapEditor;
	}
	if (Application::IsKeyPressed('Y'))
	{
		player1->vel.SetZero();
		//gameStates = states::s_MapEditor;
	}

	if (Application::IsKeyPressed('9'))
	{
		m_speed = Math::Max(0.f, m_speed - 0.1f);
	}
	if (Application::IsKeyPressed('0'))
	{
		m_speed += 0.1f;
	}
	static bool hi = false;
	if (Application::IsKeyPressed('7') && hi == false)
	{
		hi = true;
		/*TextFile* animal = new TextFile(TextFile::ANIMAL);
		animal->GetAnimalStat("Zebra");*/
		TextFile* car = new TextFile(TextFile::CAR);
		car->GetCarStat("Car2", "CarStats.txt");
		car->SetCarStat("Car2", "hp", "8");
		car->GetCarStat("Car2", "tempsave.txt");
	}
	if (hi && !Application::IsKeyPressed('7'))
	{
		hi = false;
	}
	if (gameStates == states::s_Tutorial ||
		gameStates == states::s_Level2 ||
		gameStates == states::s_Level3 ||
		gameStates == states::s_LevelBoss ||
		gameStates == states::s_MapEditor && testMode == 1)
	{
		playerControl(dt);
		if (Application::IsKeyPressed('W') || (Application::IsKeyPressed('S')))
		{
			if (gameStates != states::s_MapEditor)
			{
				fuelAmount -= dt;
			}
		}
	}
	if (gameStates == states::s_Tutorial ||
		gameStates == states::s_Level2 ||
		gameStates == states::s_Level3 ||
		gameStates == states::s_LevelBoss ||
		gameStates == states::s_MapEditor && testMode == 0)
	{
		if (Application::IsKeyPressed(VK_ESCAPE))
		{
			gameStates = states::s_Menu;
			eraseEnemy();
		}
	}
	/*else if (gameStates == states::s_MapEditor && testMode == 1)
	{
		testMode = 0;
	}*/

	if (gameStates == states::s_MapEditor)
	{
		mapEditorUpdate(dt);
	}
	if (fuelAmount <= 0.0f)
	{
		gameStates = states::s_Lose;
	}
	if (sound == 1 && Sound_Engine != NULL)
	{
		if (player1->engine != 0)
		{
			Sound_Engine->setIsPaused(false);
		}
		else
		{
			Sound_Engine->setIsPaused(true);
		}
	}
	

	//Mouse Section
	static bool bLButtonState = false;
	
	if (gameStates != states::s_Tutorial ||
		gameStates != states::s_Level2 ||
		gameStates != states::s_Level3 ||
		gameStates != states::s_LevelBoss)
	{
		/*if (sound == 1)
		{
			backSound();
		}
		else
		{
			if (Sound_Back != NULL)
			{
				Sound_Back->stop();
			}
		}*/
		
		if (bLButtonState && !Application::IsMousePressed(0))
		{
			bLButtonState = false;
			std::cout << "LBUTTON UP" << std::endl;

			//Exercise 6: spawn small GO_BALL
			double x, y;
			Application::GetCursorPos(&x, &y);
			int w = Application::GetWindowWidth();
			int h = Application::GetWindowHeight();
			float worldX = x * m_worldWidth / w;
			float worldY = (h - y) * m_worldHeight / h;
			m_ghost->active = false;

			
			if (gameStates == states::s_Menu)
			{
		
				if (worldX > 0.863f * m_worldWidth && worldX < 0.947f * m_worldWidth)
				{
					if (worldY > 84.11 && worldY < 95.4f)
					{
						gameStates = states::s_MapEditor;
					}
				}
				if (worldX > 0.695f * m_worldWidth && worldX < 0.805f * m_worldWidth)
				{
					if (worldY > 36.111 && worldY < 39.444f)
					{
						gameStates = states::s_LevelSelect;
					}
					if (worldY > 29.2f && worldY <  32.35f)
					{
						gameStates = states::s_Instructions;
					}
					if (worldY > 22 && worldY < 25.27f)
					{
						gameStates = states::s_Options;
					}
					if (worldY > 15.438f && worldY < 18.6f)
					{
						gameStates = states::s_Highscore;
					}
					if (worldY > 8 && worldY < 11.406f)
					{
						Exit();
						exit(0);
					}
				}
			}
			else if (gameStates == states::s_Instructions)
			{
				if (worldX > 0.035f * m_worldWidth && worldX < 0.14141f * m_worldWidth)
				{
					if (worldY > 3.0 && worldY < 9.0f)
					{
						gameStates = states::s_Menu;
					}
				}
			}
			else if (gameStates == states::s_LevelSelect)
			{
				if (worldY > 47.8 && worldY < 52.3f)
				{
					if (worldX > 0.33f * m_worldWidth && worldX < 0.36f * m_worldWidth)
					{
						loadMapStuff = 1;
						gameStates = states::s_Tutorial;
						
					}
					if (worldX > 0.428f * m_worldWidth && worldX < 0.461f * m_worldWidth)
					{
						loadMapStuff = 1;
						gameStates = states::s_Level2;
					}
					if (worldX > 0.5322f * m_worldWidth && worldX < 0.563f * m_worldWidth)
					{
						loadMapStuff = 1;
						gameStates = states::s_Level3;
					}
					if (worldX > 0.631f * m_worldWidth && worldX < 0.669f * m_worldWidth)
					{
						loadMapStuff = 1;
						gameStates = states::s_LevelBoss;
					}
				}
				if (worldX > 0.78854f * m_worldWidth && worldX < 0.99219f * m_worldWidth)
				{
					if (worldY > 1 && worldY < 11.6f)
					{
						gameStates = states::s_CustomLevelSelect;
					}
				}
				if (worldX > 0.1f * m_worldWidth && worldX < 0.203f * m_worldWidth)
				{
					if (worldY > 1 && worldY < 11.6f)
					{
						gameStates = states::s_Menu;
					}
				}
			}
			else if (gameStates == states::s_CustomLevelSelect)
			{
				if (worldX > 0.1f * m_worldWidth && worldX < 0.203f * m_worldWidth)
				{
					if (worldY > 1 && worldY < 11.6f)
					{
						gameStates = states::s_LevelSelect;
					}
				}

			}
			else if (gameStates == states::s_Options)
			{
				if (worldX > 0.3f * m_worldWidth && worldX < 0.7f * m_worldWidth)
				{
					if (worldY > 72.f && worldY < 78.f)
					{
						if (sound == 1)
						{
							sound = 0;
							Sound_Back->setIsPaused(true);
						}
						else
						{
							sound = 1;
							Sound_Back->setIsPaused(false);
						}
					}
				}
				if (worldX > 0.3f * m_worldWidth && worldX < 0.7f * m_worldWidth)
				{
					if (worldY > 53.f && worldY < 59.f)
					{
						if (highQ == 1)
						{
							highQ = 0;
						}
						else
						{
							highQ = 1;
						}
					}
				}
				if (worldX > 0.448f * m_worldWidth && worldX < 0.550f * m_worldWidth)
				{
					if (worldY > 34.f && worldY < 39.f)
					{
						gameStates = states::s_Menu;
					}
				}
			}
			if (gameStates == states::s_Upgrade_Cars1 || gameStates == states::s_Upgrade_Cars3)
			{
				if (worldX > 0.232f * m_worldWidth && worldX < 0.3777f * m_worldWidth)
				{
					if (worldY > 13.16f && worldY < 24.5f)
					{
						gameStates = states::s_Upgrade_Cars2;
					}
				}
			}
			if (gameStates == states::s_Upgrade_Cars2 || gameStates == states::s_Upgrade_Cars3)
			{
				if (worldX > 0.054f * m_worldWidth && worldX < 0.20f * m_worldWidth)
				{
					if (worldY > 13.16f && worldY < 24.5f)
					{
						gameStates = states::s_Upgrade_Cars1;
					}
				}
			}
			if (gameStates == states::s_Upgrade_Cars1 || gameStates == states::s_Upgrade_Cars2)
			{
				if (worldX > 0.408f * m_worldWidth && worldX < 0.554f * m_worldWidth)
				{
					if (worldY > 13.16f && worldY < 24.5f)
					{
						gameStates = states::s_Upgrade_Cars3;
					}
				}
			}

			if (gameStates == states::s_Upgrade_Tires1 || gameStates == states::s_Upgrade_Tires3)
			{
				if (worldX > 0.232f * m_worldWidth && worldX < 0.3777f * m_worldWidth)
				{
					if (worldY > 13.16f && worldY < 24.5f)
					{
						gameStates = states::s_Upgrade_Tires2;
					}
				}
			}
			if (gameStates == states::s_Upgrade_Tires2 || gameStates == states::s_Upgrade_Tires3)
			{
				if (worldX > 0.054f * m_worldWidth && worldX < 0.20f * m_worldWidth)
				{
					if (worldY > 13.16f && worldY < 24.5f)
					{
						gameStates = states::s_Upgrade_Tires1;
					}
				}
			}
			if (gameStates == states::s_Upgrade_Tires1 || gameStates == states::s_Upgrade_Tires2)
			{
				if (worldX > 0.408f * m_worldWidth && worldX < 0.554f * m_worldWidth)
				{
					if (worldY > 13.16f && worldY < 24.5f)
					{
						gameStates = states::s_Upgrade_Tires3;
					}
				}
			}

			if (gameStates == states::s_Upgrade_Lasso1 || gameStates == states::s_Upgrade_Lasso3)
			{
				if (worldX > 0.232f * m_worldWidth && worldX < 0.3777f * m_worldWidth)
				{
					if (worldY > 13.16f && worldY < 24.5f)
					{
						gameStates = states::s_Upgrade_Lasso2;
					}
				}
			}
			if (gameStates == states::s_Upgrade_Lasso2 || gameStates == states::s_Upgrade_Lasso3)
			{
				if (worldX > 0.054f * m_worldWidth && worldX < 0.20f * m_worldWidth)
				{
					if (worldY > 13.16f && worldY < 24.5f)
					{
						gameStates = states::s_Upgrade_Lasso1;
					}
				}
			}
			if (gameStates == states::s_Upgrade_Lasso1 || gameStates == states::s_Upgrade_Lasso2)
			{
				if (worldX > 0.408f * m_worldWidth && worldX < 0.554f * m_worldWidth)
				{
					if (worldY > 13.16f && worldY < 24.5f)
					{
						gameStates = states::s_Upgrade_Lasso3;
					}
				}
			}

			if (gameStates == states::s_Upgrade_Darts1 || gameStates == states::s_Upgrade_Darts3)
			{
				if (worldX > 0.232f * m_worldWidth && worldX < 0.3777f * m_worldWidth)
				{
					if (worldY > 13.16f && worldY < 24.5f)
					{
						gameStates = states::s_Upgrade_Darts2;
					}
				}
			}
			if (gameStates == states::s_Upgrade_Darts2 || gameStates == states::s_Upgrade_Darts3)
			{
				if (worldX > 0.054f * m_worldWidth && worldX < 0.20f * m_worldWidth)
				{
					if (worldY > 13.16f && worldY < 24.5f)
					{
						gameStates = states::s_Upgrade_Darts1;
					}
				}
			}
			if (gameStates == states::s_Upgrade_Darts1 || gameStates == states::s_Upgrade_Darts2)
			{
				if (worldX > 0.408f * m_worldWidth && worldX < 0.554f * m_worldWidth)
				{
					if (worldY > 13.16f && worldY < 24.5f)
					{
						gameStates = states::s_Upgrade_Darts3;
					}
				}
			}
		}
		if (!bLButtonState && Application::IsMousePressed(0))
		{

			bLButtonState = true;
		}
		
	}
	if (gameStates == states::s_Level2)
	{
		if (loadMapStuff == 1)
		{
		TextFile* map = new TextFile(TextFile::MAP);
		//call LevelJW textfile
		NameofMap = map->CreateMapFile("LEVELJW", true);
		RenderMapFile();
		
			for (std::vector<GameObject *>::iterator it = testMap.mapProps.begin(); it != testMap.mapProps.end(); ++it)
			{
				GameObject *go = (GameObject *)*it;
				if (go->active == false)
				{
					go->dead = true;
				}
				if (go->dead == true)
				{
					continue;
				}
				if (go->type == GameObject::MAP_LION)
				{
					animalStat->GetAnimalStat("Lion");
					enemy* animal = newEnemy(go->pos.x - mapPosition.x, go->pos.y - mapPosition.y, 0, 2, animalStat->get_stamina(), animalStat->get_speed(), animalStat->get_strength());
					animal->setVel(Math::RandFloatMinMax(-10, 10), Math::RandFloatMinMax(-10, 10), 0);
					animal->setLeader(0);
					enemyList.push_back(animal);
					go->active = false;
				}
				else if (go->type == GameObject::MAP_ZEBRA)
				{
					animalStat->GetAnimalStat("Zebra");
					enemy* animal = newEnemy(go->pos.x - mapPosition.x, go->pos.y - mapPosition.y, 0, 0, animalStat->get_stamina(), animalStat->get_speed(), animalStat->get_strength());
					animal->setVel(Math::RandFloatMinMax(-10, 10), Math::RandFloatMinMax(-10, 10), 0);
					animal->setLeader(0);
					enemyList.push_back(animal);
					go->active = false;
				}
				else if (go->type == GameObject::MAP_RHINO)
				{
					animalStat->GetAnimalStat("Rhino");
					enemy* animal = newEnemy(go->pos.x - mapPosition.x, go->pos.y - mapPosition.y, 0, 1, animalStat->get_stamina(), animalStat->get_speed(), animalStat->get_strength());
					animal->setVel(Math::RandFloatMinMax(-10, 10), Math::RandFloatMinMax(-10, 10), 0);
					animal->setLeader(0);
					enemyList.push_back(animal);
					go->active = false;
				}
				else if (go->type == GameObject::MAP_HUMAN)
				{
					animalStat->GetAnimalStat("Human");
					enemy* animal = newEnemy(go->pos.x - mapPosition.x, go->pos.y - mapPosition.y, 0, 3, animalStat->get_stamina(), animalStat->get_speed(), animalStat->get_strength());
					animal->setVel(Math::RandFloatMinMax(-10, 10), Math::RandFloatMinMax(-10, 10), 0);
					animal->setLeader(0);
					enemyList.push_back(animal);
					go->active = false;
				}
			}
			loadMapStuff = 0;
		}
	}
	if (gameStates == states::s_LevelBoss)
	{
		if (loadMapStuff == 1)
		{
			TextFile* map = new TextFile(TextFile::MAP);
			//call LevelJW textfile
			NameofMap = map->CreateMapFile("LEVELFOU", true);
			RenderMapFile();

			for (std::vector<GameObject *>::iterator it = testMap.mapProps.begin(); it != testMap.mapProps.end(); ++it)
			{
				GameObject *go = (GameObject *)*it;
				if (go->active == false)
				{
					go->dead = true;
				}
				if (go->dead == true)
				{
					continue;
				}
				if (go->type == GameObject::MAP_LION)
				{
					animalStat->GetAnimalStat("Lion");
					enemy* animal = newEnemy(go->pos.x - mapPosition.x, go->pos.y - mapPosition.y, 0, 2, animalStat->get_stamina(), animalStat->get_speed(), animalStat->get_strength());
					animal->setVel(Math::RandFloatMinMax(-10, 10), Math::RandFloatMinMax(-10, 10), 0);
					animal->setLeader(0);
					enemyList.push_back(animal);
					go->active = false;
				}
				else if (go->type == GameObject::MAP_ZEBRA)
				{
					animalStat->GetAnimalStat("Zebra");
					enemy* animal = newEnemy(go->pos.x - mapPosition.x, go->pos.y - mapPosition.y, 0, 0, animalStat->get_stamina(), animalStat->get_speed(), animalStat->get_strength());
					animal->setVel(Math::RandFloatMinMax(-10, 10), Math::RandFloatMinMax(-10, 10), 0);
					animal->setLeader(0);
					enemyList.push_back(animal);
					go->active = false;
				}
				else if (go->type == GameObject::MAP_RHINO)
				{
					animalStat->GetAnimalStat("Rhino");
					enemy* animal = newEnemy(go->pos.x - mapPosition.x, go->pos.y - mapPosition.y, 0, 1, animalStat->get_stamina(), animalStat->get_speed(), animalStat->get_strength());
					animal->setVel(Math::RandFloatMinMax(-10, 10), Math::RandFloatMinMax(-10, 10), 0);
					animal->setLeader(0);
					enemyList.push_back(animal);
					go->active = false;
				}
				else if (go->type == GameObject::MAP_HUMAN)
				{
					animalStat->GetAnimalStat("Human");
					enemy* animal = newEnemy(go->pos.x - mapPosition.x, go->pos.y - mapPosition.y, 0, 3, animalStat->get_stamina(), animalStat->get_speed(), animalStat->get_strength());
					animal->setVel(Math::RandFloatMinMax(-10, 10), Math::RandFloatMinMax(-10, 10), 0);
					animal->setLeader(0);
					enemyList.push_back(animal);
					go->active = false;
				}
			}
			loadMapStuff = 0;
		}
	}

	/*if (Application::IsKeyPressed(VK_ESCAPE))
	{
		if (gameStates == states::s_Tutorial ||
			gameStates == states::s_Level2 ||
			gameStates == states::s_Level3 ||
			gameStates == states::s_LevelBoss ||
			gameStates == states::s_MapEditor && testMode == 1)
		{
			paused = true;
		}
	}
	if (gameStates == states::s_Highscore)
	{
		if (Application::IsKeyPressed(VK_UP) || Application::IsKeyPressed('W'))
		{
			if (scroll <= scrollimit)
			scroll++;
		}
		if (Application::IsKeyPressed(VK_DOWN) || Application::IsKeyPressed('S'))
		{
			if (scroll >= 0)
			scroll--;
		}
	}
	if (paused == true)
	{
		if (bLButtonState && !Application::IsMousePressed(0))
		{
			bLButtonState = false;
			std::cout << "LBUTTON UP" << std::endl;

			double x, y;
			Application::GetCursorPos(&x, &y);
			int w = Application::GetWindowWidth();
			int h = Application::GetWindowHeight();
			float worldX = x * m_worldWidth / w;
			float worldY = (h - y) * m_worldHeight / h;
			m_ghost->active = false;

			if (worldY > 29.0f && worldY < 33.33f)
			{
				if (worldX > 0.27f * m_worldWidth && worldX < 0.366f * m_worldWidth)
				{
					gameStates = states::s_LevelSelect;
				}
				if (worldX > 0.656f * m_worldWidth && worldX < 0.735f * m_worldWidth)
				{
					paused = false;
				}
			}
		}
		if (!bLButtonState && Application::IsMousePressed(0))
		{
			bLButtonState = true;
		}
	}*/
		static bool pressedBack = false;
		if (gameStates == states::s_CustomLevelSelect)
		{
			if (Application::IsKeyPressed(VK_ESCAPE) && pressedBack == false)
			{
				pressedBack = true;
				gameStates = states::s_LevelSelect;
			}
			else if (!Application::IsKeyPressed(VK_ESCAPE) && pressedBack == true)
			{
				pressedBack = false;
			}
		}
		if (gameStates == states::s_LevelSelect)
		{
			if (Application::IsKeyPressed(VK_ESCAPE) && pressedBack == false)
			{
				pressedBack = true;
				gameStates = states::s_Menu;
			}
			else if (!Application::IsKeyPressed(VK_ESCAPE) && pressedBack == true)
			{
				pressedBack = false;
			}
		}
		if (gameStates == states::s_Options)
		{
			if (Application::IsKeyPressed(VK_ESCAPE) && pressedBack == false)
			{
				pressedBack = true;
				gameStates = states::s_Menu;
			}
			else if (!Application::IsKeyPressed(VK_ESCAPE) && pressedBack == true)
			{
				pressedBack = false;
			}
		}
		if (gameStates == states::s_Lose)
		{
			if (Application::IsKeyPressed(VK_ESCAPE))
			{
				gameStates = states::s_Menu;
				fuelAmount = 100.0f;
				points = 0;
			}
		}
		if (gameStates == states::s_Win)
		{
			if (Application::IsKeyPressed(VK_ESCAPE))
			{
				gameStates = states::s_Menu;
				fuelAmount = 100.0f;
				points = 0;
			}
		}
		if (gameStates == states::s_LevelBoss || gameStates == states::s_Level3 || gameStates == states::s_Level2 || gameStates == states::s_Tutorial)
		{
			bool winCondition = true;
			for (std::vector<enemy*>::iterator itE3 = enemyList.begin(); itE3 != enemyList.end(); ++itE3)
			{
				enemy *goE3 = (enemy *)*itE3;
				if (goE3 -> getActive() == true && winCondition == true && goE3->getType() != 3)
				{
					winCondition = false;
				}

			}
			if (winCondition == true)
			{
				if (gameStates == states::s_LevelBoss)
				{
					gameStates = states::s_Win;
				}
				else if (gameStates == states::s_Level3)
				{
					gameStates = states::s_Upgrade;
				}
				else if (gameStates == states::s_Level2)
				{
					gameStates = states::s_Upgrade;
				}
				else if (gameStates == states::s_Tutorial)
				{
					gameStates = states::s_Upgrade;
				}
				eraseEnemy();
				winCondition = false;
			}

		}
		if (gameStates == states::s_Upgrade_Cars1 || gameStates == states::s_Upgrade_Cars2 || gameStates == states::s_Upgrade_Cars3 ||
			gameStates == states::s_Upgrade_Tires1 || gameStates == states::s_Upgrade_Tires2 || gameStates == states::s_Upgrade_Tires3 ||
			gameStates == states::s_Upgrade_Lasso1 || gameStates == states::s_Upgrade_Lasso2 || gameStates == states::s_Upgrade_Lasso3 ||
			gameStates == states::s_Upgrade_Darts1 || gameStates == states::s_Upgrade_Darts2 || gameStates == states::s_Upgrade_Darts3)
		{
			if (Application::IsKeyPressed(VK_LEFT))
			{
				rotateDisplayX -= 5 * dt;
			}
			if (Application::IsKeyPressed(VK_RIGHT))
			{
				rotateDisplayX += 5 * dt;
			}
			if (Application::IsKeyPressed(VK_UP))
			{
				rotateDisplayY -= 5 * dt;
			}
			if (Application::IsKeyPressed(VK_DOWN))
			{
				rotateDisplayY += 5 * dt;
			}
		}
		if (gameStates == states::s_Menu)
		{
			static bool arrowkeyUp = false;
			static bool arrowkeyDown = false;

			if (arrowSelection > 0)
			{
				if (Application::IsKeyPressed('W') && arrowkeyUp == false)
				{
					arrowkeyUp = true;
					arrowSelection--;
				}
				else if (!Application::IsKeyPressed('W') && arrowkeyUp == true)
					arrowkeyUp = false;
			}
			else if (arrowSelection == 0)
			{
				if (Application::IsKeyPressed('W') && arrowkeyUp == false)
				{
					arrowkeyUp = true;
					arrowSelection = 4;
				}
				else if (!Application::IsKeyPressed('W')  && arrowkeyUp == true)
					arrowkeyUp = false;

			}

			if (arrowSelection < 4)
			{
				if (Application::IsKeyPressed('S') && arrowkeyDown == false)
				{
					arrowkeyDown = true;
					arrowSelection++;
				}
				else if (!Application::IsKeyPressed('S')  && arrowkeyDown == true)
					arrowkeyDown = false;
			}
			else if (arrowSelection == 4)
			{
				if (Application::IsKeyPressed('S') && arrowkeyDown == false)
				{
					arrowkeyDown = true;
					arrowSelection = 0;
				}
				else if (!Application::IsKeyPressed('S') && arrowkeyDown == true)
					arrowkeyDown = false;
			}
			if (Application::IsKeyPressed(VK_RETURN))
			{
				switch (arrowSelection)
				{
				case 0:
				{
						  gameStates = states::s_LevelSelect;
						  break;
				}
				case 1:
				{
						  gameStates = states::s_Instructions;
						  break;
				}
				case 2:
				{
						  gameStates = states::s_Options;
						  break;
				}
				case 3:
				{
						  gameStates = states::s_Highscore;
						  break;
				}
				case 4:
				{
						  exit(0);
						  break;
				}
				}
			}
		}
		//Physics Simulation Section

		for (std::vector<GameObject *>::iterator it = m_goList.begin(); it != m_goList.end(); ++it)
		{
			GameObject *go = (GameObject *)*it;
			if (go->active)
			{
				go->pos += go->vel * m_speed * dt;

				//Exercise 7: handle out of bound game objects

				if (go->type == GameObject::GO_BALL)
				{
					if (go->pos.x > m_worldWidth - go->scale.x)
					{
						if (go->vel.x > 0)
							go->vel.x = -go->vel.x;
					}
					else if (go->pos.x < 0 + go->scale.x)
					{
						if (go->vel.x < 0)
							go->vel.x = -go->vel.x;
					}
					if (go->pos.x > m_worldWidth + go->scale.x || go->pos.x < 0 - go->scale.x)
					{
						go->active = false;
						m_objectCount--;
					}
					if (go->pos.y > m_worldHeight - go->scale.y)
					{
						if (go->vel.y > 0)
							go->vel.y = -go->vel.y;
					}
					else if (go->pos.y < 0 + go->scale.y)
					{
						if (go->vel.y < 0)
							go->vel.y = -go->vel.y;
					}
					if (go->pos.y > m_worldHeight + go->scale.y || go->pos.y < 0 - go->scale.y)
					{
						go->active = false;
						m_objectCount--;

					}
					if (go->vel.x != 0 || go->vel.y != 0)
					{
						go->vel *= 0.995;
					}
					if (go->vel.Length() > 110)
					{
						go->vel = go->vel.Normalized() * 40;
					}
					go->rotationAngle = Math::RadianToDegree(atan2(go->vel.y, go->vel.x));
					go->ballrotated += go->vel.Length() * (2 / go->scale.x);


				}
				else if (go->type == GameObject::GO_TRANQ)
				{
					go->pos.x += diffx;
					go->pos.y += diffy;
					if ((go->pos - player1->pos).LengthSquared() > 14400)
					{
						go->active = false;
						continue;
					}
					for (std::vector<enemy *>::iterator it2 = enemyList.begin(); it2 != enemyList.end(); ++it2)
					{
						enemy *other = (enemy *)*it2;
						if (other->getActive() == false)
							continue;
						if ((go->pos - other->getPos()).LengthSquared() < 50)
						{
							go->active = false;
							other->setDrunk(true);
							break;
						}
					}
				}
				//Exercise 8a: handle collision between GO_BALL and GO_BALL using velocity swap

				for (std::vector<GameObject *>::iterator it2 = it; it2 != m_goList.end(); ++it2)
				{
					GameObject *other = (GameObject*)*it2;
					//if (other->type == GameObject::GO_BALL)
					//	continue;
					if (!other->active)
						continue;

					GameObject *goA = go, *goB = other;
					if (go->type != GameObject::GO_BALL)
					{
						if (other->type != GameObject::GO_BALL)
							continue;
						goA = other;
						goB = go;
					}

					if (CheckCollision3(goA, goB, dt))
					{
						//Exercise 8b: store values in auditing variables

						CollisionResponse(goA, goB);

						break;
					}
				}



				if (go->type == GameObject::GO_CAR)
				{
					if (!go->active)
						continue;
					if (go->pos.x > m_worldWidth)
					{
						//std::cout << go->pos << std::endl;
						go->pos.x -= go->pos.x - m_worldWidth;
					}
					else if (go->pos.x < 0)
					{
						go->pos.x += 0 - go->pos.x;
					}
					if (go->pos.y > m_worldHeight)
					{
						go->pos.y -= go->pos.y - m_worldHeight;
					}
					else if (go->pos.y < 0)
					{
						go->pos.y += 0 - go->pos.y;
					}
					for (std::vector<GameObject *>::iterator it2 = m_goList.begin(); it2 != m_goList.end(); ++it2)
					{
						GameObject *other = (GameObject*)*it2;
						if (other->type == GameObject::GO_POWERUP)
						{
							if (!other->active)
								continue;

						}
					}
				}

				if (go->type == GameObject::GO_PARTICLE)
				{
					go->scale *= 0.9f;
					if (go->scale.x < 0.1f)
					{
						go->active = false;
					}
				}
				UpgradeController();
				//Exercise 10: handle collision using momentum swap instead

				//Exercise 12: improve inner loop to prevent double collision

				//Exercise 13: improve collision detection algorithm [solution to be given later] 
			}
		}
		
		for (std::vector<GameObject *>::iterator it = testMap.mapProps.begin(); it != testMap.mapProps.end(); ++it)
		{
			GameObject *go = (GameObject *)*it;
			if (go->dead == true)
				continue;

			if (go->fresh)
			{
				go->pos += mapPosition;
				go->fresh = false;
			}
			else
			{
				go->pos = Vector3(go->pos.x + diffx, go->pos.y + diffy, 1);
			}

			if ((go->pos - player1->pos).Length() < 10)
			{
				if (go->active == false)
					continue;
				CollisionMap(player1, go, dt);
			}
		}
		for (std::vector<GameObject *>::iterator it = testMap.mapBorder.begin(); it != testMap.mapBorder.end(); ++it)
		{
			GameObject *go = (GameObject *)*it;
			if (go->fresh)
			{
				go->pos += mapPosition;
				go->fresh = false;
			}
			else
			{
				//go->pos = Vector3(go->pos.x + diffx, go->pos.y + diffy, 1);
				go->pos.x += diffx;
				go->pos.y += diffy;
			}
		}
		if (gameStates == states::s_Tutorial ||
			gameStates == states::s_Level2 ||
			gameStates == states::s_Level3 ||
			gameStates == states::s_LevelBoss ||
			gameStates == states::s_MapEditor)
		{
			if (sound == 1 && Sound_Back != NULL)
			{
				Sound_Back->setIsPaused(true);
			}
		}
		else
		{
			if (sound == 1 && Sound_Back != NULL)
			{
				Sound_Back->setIsPaused(false);
			}
			if (Sound_Engine != NULL && sound == 1)
			{
				Sound_Engine->setIsPaused(true);
			}
		}
		if (gameStates == states::s_Tutorial ||
			gameStates == states::s_Level2 ||
			gameStates == states::s_Level3 ||
			gameStates == states::s_LevelBoss ||
			gameStates == states::s_MapEditor && testMode == 1)
		{
			/*if (leader != NULL)
			{
			std::cout << (leader->getPos() - player1->pos).Length() << std::endl;
			leader->setLeader(0);
			}*/

			if (Application::IsKeyPressed('Z'))
			{
				animalStat->GetAnimalStat("Zebra");
				enemy* animal = newEnemy(Math::RandFloatMinMax(-100, 100), Math::RandFloatMinMax(-100, 100), 0, 0, animalStat->get_stamina(), animalStat->get_speed(), animalStat->get_strength());
				animal->setVel(Math::RandFloatMinMax(-10, 10), Math::RandFloatMinMax(-10, 10), 0);
				animal->setLeader(0);
				enemyList.push_back(animal);
			}
			
			if (Application::IsKeyPressed('X'))
			{
				animalStat->GetAnimalStat("Rhino");
				enemy* animal = newEnemy(Math::RandFloatMinMax(-100, 100), Math::RandFloatMinMax(-100, 100), 0, 1, animalStat->get_stamina(), animalStat->get_speed(), animalStat->get_strength());
				animal->setVel(Math::RandFloatMinMax(-10, 10), Math::RandFloatMinMax(-10, 10), 0);
				animal->setLeader(0);
				enemyList.push_back(animal);
			}
			if (Application::IsKeyPressed('C'))
			{
				animalStat->GetAnimalStat("Lion");
				enemy* animal = newEnemy(Math::RandFloatMinMax(-100, 100), Math::RandFloatMinMax(-100, 100), 0, 2, animalStat->get_stamina(), animalStat->get_speed(), animalStat->get_strength());
				animal->setVel(Math::RandFloatMinMax(-10, 10), Math::RandFloatMinMax(-10, 10), 0);
				animal->setLeader(0);
				enemyList.push_back(animal);
			}

			if (Application::IsKeyPressed('V'))
			{
				animalStat->GetAnimalStat("Human");
				enemy* animal = newEnemy(Math::RandFloatMinMax(-100, 100), Math::RandFloatMinMax(-100, 100), 0, 3, animalStat->get_stamina(), animalStat->get_speed(), animalStat->get_strength());
				animal->setVel(Math::RandFloatMinMax(-10, 10), Math::RandFloatMinMax(-10, 10), 0);
				animal->setLeader(0);
				enemyList.push_back(animal);
			}

			

			for (std::vector<enemy*>::iterator itE2 = enemyList.begin(); itE2 != enemyList.end(); ++itE2)
			{
				enemy *goE2 = (enemy *)*itE2;
				if (goE2->getActive() == 1 && goE2->getType() != 3)
				{
					if (leader == NULL && (goE2->getPos() - player1->pos).Length() < 30)
					{
						leader = goE2;
					}
					else if (leader != NULL)
					{
						if ((goE2->getPos() - player1->pos).Length() < (leader->getPos() - player1->pos).Length() && (goE2->getPos() - player1->pos).Length() < 30)
						{
							leader->setLeader(0);
							leader = goE2;
							leader->setLeader(1);
						}
					}
					/*if ((goE2->getPos() - player1->pos).Length() < 30)
					{

					}*/
				}

			}


			for (std::vector<enemy*>::iterator itE = enemyList.begin(); itE != enemyList.end(); ++itE)
			{
				enemy *goE = (enemy *)*itE;
				if (goE->getActive() == 1 )
					{

						if (goE->getNewSpawn() == 1)
						{
							goE->setPos(goE->getPos().x + mapPosition.x, goE->getPos().y + mapPosition.y, 2);
							goE->setNewSpawn(false);
						}
						else
						{
							goE->setPos(goE->getPos().x + diffx, goE->getPos().y + diffy, 2);
							if (goE->getType() != 3)
							{

								if ((goE->getPos() - player1->pos).Length() < 30 && player1->engine != 0)
								{
									if (leader == NULL)
									{
										leader = goE;

									}
									goE->runOff(player1->pos, enemyList, leader);
									goE->increaseRunLonger(3);
								}
								else if (goE->getRunLonger() > 0 || goE->getCaught() == 1)
								{
									goE->runOff(player1->pos, enemyList, leader);
									if (goE->getCaught() == 1)
									{
										goE->increaseRunLonger(3);
										player1->vel += (0.0001 * goE->getStrength()) * goE->getVel();
									}
									else
									{
										goE->increaseRunLonger(-1);
									}
								}
								else
								{
									bool objCheck = 0;
									for(std::vector<GameObject *>::iterator itO = testMap.mapProps.begin(); itO != testMap.mapProps.end(); ++itO)
									{
										GameObject *goO = (GameObject *)*itO;
										if (goO->type == GameObject::MAP_OBJECTIVE && goO->active == 1)
										{
											objCheck = 1;
											if (objective == NULL)
											{
												objective = goO->pos;
											}
											else if ((goE->getPos() - goO->pos).Length() < (goE->getPos() - objective).Length())
											{
												objective = goO->pos;
											}
										}
										
									}
									if (objCheck == 0)
									{
										objective = center;
									}
									goE->slowDown(enemyList, objective);
									if (goE->getPos().x > objective.x - 10 && goE->getPos().x < objective.x + 10 && goE->getPos().y > objective.y - 10 && goE->getPos().y < objective.y + 10 && goE->getTaken() == 0)
									{
										goE->setTaken(1);
									}
									if (goE->getTaken() == 1)
									{

										if (center.x - goE->getPos().x < -(testMap.getMapSize().x) * 5 || center.x - goE->getPos().x > testMap.getMapSize().x * 5 ||  center.y - goE->getPos().y < -(testMap.getMapSize().y) * 5 || center.y - goE->getPos().y > testMap.getMapSize().y * 5)
										{
											float test = -(testMap.getMapSize().x) * 5;
											goE->setActive(false);
											//points--;
										}
									}
									objective = NULL;
								}
							}

							goE->checkCollision(enemyList);
							if (goE->getPos().x > player1->pos.x - 7 && goE->getPos().x < player1->pos.x + 7 && goE->getPos().y > player1->pos.y - 7 && goE->getPos().y < player1->pos.y + 7)
							{
								if (sound == 1)
								{
									bumpSound();
								}
								goE->addStrength(-50);
								if (goE->getStrength() <= 0)
								{
									goE->setActive(false);
									if (goE->getType() != 3)
									{
										points--;
									}
									else
									{
										points -= 3;
									}
									
								}
								else
								{
									Vector3 temp = goE->getPos();
									goE->setVel((temp.x - player1->pos.x) * 10, (temp.y - player1->pos.y) * 10, 0);
								}
								player1->vel = (0, 0, 0);
								player1->engine = 0;
							}

							goE->updatePos(dt);
							if (Dalasso->checkCaught(player1->pos, goE->getPos(), 5) == 1 || goE->getCaught() == 1)
							{
								goE->setCaught(1);
								if (Dalasso->caughtUpdate(player1->pos, goE->getPos(), goE->getActive()) == 1)
								{
									goE->setCaught(0);
									if (goE->getActive() == 0)
									{
										if (sound == 1)
										{
											dingSound();
										}
										if (goE->getType() == 0)
										{
											points++;
										}
										else if (goE->getType() == 1)
										{
											points += 2;
										}
										else if (goE->getType() == 2)
										{
											points += 5;
										}
										else if (goE->getType() == 3)
										{
											points -= 3;
										}
									}
								}
	
							}
						}
					}


			}
			Dalasso->updateLasso(player1->pos, dt);
			/*if (furthestAlerted != NULL)
			{
			furthestAlerted->setLeader(1);
			}*/

		}
		if (gameStates == states::s_Tutorial ||
			gameStates == states::s_Level2 ||
			gameStates == states::s_Level3 ||
			gameStates == states::s_LevelBoss ||
			gameStates == states::s_MapEditor && testMode == 1)
		{
			static bool bLButtonState = false;

			if (!bLButtonState && Application::IsMousePressed(0) && Dalasso->getLassoState() == 0)
			{
				bLButtonState = true;
				std::cout << "LBUTTON DOWN" << std::endl;
				double x, y;
				Application::GetCursorPos(&x, &y);
				int w = Application::GetWindowWidth();
				int h = Application::GetWindowHeight();
				float worldX = x * m_worldWidth / w;
				float worldY = (h - y) * m_worldHeight / h;
				//m_ghost->pos.Set(worldX, worldY, 0);
				//m_ghost->active = true;
				if (Dalasso->throwLasso(player1->pos, Vector3(worldX, worldY, 0)) == 1)
				{

					if (sound == 1)
					{
						throwSound();
					}
				}

				snapSet = 1;
				

			}
			else
			{
				bLButtonState = false;
			}

			if (Dalasso->getLassoState() == 3)
			{
				if (snapSet == 1)
				{
					if (sound == 1)
					{
						snapSound();
					}
					snapSet = 0;
				}
				if (player1->engine > 0)
				{
					player1->engine -= 1;
				}
				if (player1->engine < 0)
				{
					player1->engine += 1;
				}
			}
			Timer++;
		}
		
		if (Timer > 1.f / dt)
		{
			Timer = 0;
			seconds++;
		
			if (EventActive == false && Cooldown != 0)
			{
				Cooldown--;
			}
			if (EventActive)
			{
				EventSeconds++;
			}
		}
		if (EventActive == false && Cooldown == 0 && points > 0)
		{
			EventActive = true;
		}
		if (EventActive && EventType == 0)
		{
			int chance = Math::RandIntMinMax(0, 10);
			if (chance <= 3)
			{
				EventType = 1;
			}
			else if (chance > 3 && chance <= 7)
			{
				EventType = 2;
			}
			else
			{
				EventType = 3;
			}
		}
		if (EventActive == true)
		{
			if (EventType == 1)
			{
				SetEvent(e_Earthquake);
				std::cout << "earthquake" << std::endl;
			}
			else if (EventType == 2)
			{
				SetEvent(e_Fog);
				std::cout << "haze" << std::endl;
			}
			else if (EventType == 3)
			{
				SetEvent(e_Flood);
				std::cout << "surf" << std::endl;
			}
			if (EventSeconds > 10)
			{
				SetEvent(e_Nothing);
				EventActive = false;
				Cooldown = 20;
				EventType = 0;
				EventSeconds = 0;
			}
		}
}

void SceneSP3::SetEvent(events thisEvent)
{
	switch (thisEvent)
	{
	case e_Earthquake:
		player1->pos.x += Math::RandFloatMinMax(-1, 1);
		player1->pos.y += Math::RandFloatMinMax(-1, 1);
		break;
	case e_Fog:
		fogActive = true;
		break;
	case e_Flood:
		if (SpecialCount < 20)
		{
			//stuff
			GameObject* obs = new GameObject(GameObject::MAP_WATER);
			obs->pos.Set(Math::RandFloatMinMax(-testMap.getMapSize().x * 2.5, testMap.getMapSize().x * 2.5), Math::RandFloatMinMax(-testMap.getMapSize().y *2.5, testMap.getMapSize().y * 2.5), 1);
			obs->fresh = true;
			obs->active = true;
			obs->scale.Set(6, 6, 6);
			testMap.forceAddSingleProp(obs);
			SpecialCount++;
		}
		break;
	case e_Nothing:
		fogActive = false;
		break;
	}
}

void SceneSP3::mapEditorUpdate(double dt)
{
	static bool editName = false;
	static bool firstDrag = false;

	static bool bRButtonState = false;
	if (testMode == false)
	{
		if (editName == true && nameType == false)
		{
			if (Application::IsKeyPressed(VK_ESCAPE) && mapName.length() > 0)
			{
				mapName.pop_back();
				nameType = true;
			}
			else
			{
				for (int i = 0; i < 26; i++)
				{
					unsigned short key = 65 + i;
					if ((GetAsyncKeyState(key) & 0x8001) != 0 && mapName.length() < 9)
					{
						nameType = true;
						std::ostringstream ss;
						ss << mapName << static_cast<char>(key);
						mapName = ss.str();
					}
				}
			}
		}
		else
		{
			nameType = false;
			if (Application::IsKeyPressed(VK_ESCAPE))
			{
				nameType = true;
			}
			else
			{
				for (int i = 0; i < 26; i++)
				{
					unsigned short key = 65 + i;
					if ((GetAsyncKeyState(key) & 0x8001) != 0)
					{
						nameType = true;
					}
				}
			}
		}
		static bool bLButtonState = false;
		static bool multipleSpawn = false;
		if (dragObj == true)
		{
			double x, y;
			Application::GetCursorPos(&x, &y);
			int w = Application::GetWindowWidth();
			int h = Application::GetWindowHeight();
			float worldX = x * m_worldWidth / w;
			float worldY = (h - y) * m_worldHeight / h;
			std::vector<GameObject *>::iterator it = testMap.mapProps.end() - 1;
			GameObject *go = (GameObject *)*it;
			go->pos.Set(worldX, worldY, 1);

			if (Application::IsKeyPressed(VK_LSHIFT))
			{
				if ((int)(go->pos.x - mapPosition.x) % 10 > 5)
				{
					go->pos.x += 10 - ((int)(go->pos.x - mapPosition.x) % 10);
				}
				else
				{
					go->pos.x -= (int)(go->pos.x - mapPosition.x) % 10;
				}

				if ((int)(go->pos.y - mapPosition.y) % 10 > 5)
				{
					go->pos.y += 10 - ((int)(go->pos.y - mapPosition.y) % 10);
				}
				else
				{
					go->pos.y -= (int)(go->pos.y - mapPosition.y) % 10;
				}
			}

			if (!bRButtonState && Application::IsMousePressed(1) && multipleSpawn == false)
			{
				bRButtonState = true;
				std::cout << "RBUTTON DOWN NEW" << std::endl;
				multipleSpawn = true;

				GameObject* multi = new GameObject(go->type);
				multi->pos.Set(go->pos.x, go->pos.y, 1);
				multi->fresh = true;
				multi->active = true;
				multi->scale = go->scale;
				testMap.forceAddSingleProp(multi);
			}
			else if (bRButtonState && !Application::IsMousePressed(1) && multipleSpawn == true)
			{
				bRButtonState = false;
				std::cout << "RBUTTON UP" << std::endl;
				multipleSpawn = false;
			}
		}
		else if (!bLButtonState && Application::IsMousePressed(0))
		{
			bLButtonState = true;
			std::cout << "LBUTTON DOWN" << std::endl;
			double x, y;
			Application::GetCursorPos(&x, &y);
			int w = Application::GetWindowWidth();
			int h = Application::GetWindowHeight();
			float worldX = x * m_worldWidth / w;
			float worldY = (h - y) * m_worldHeight / h;
			if (deleteMode == 2)
			{
				deleteMode = 3;
				newMouseX = worldX;
				newMouseY = worldY;
			}
			if (worldX > 0.9310344f * m_worldWidth && worldX < 0.989068f * m_worldWidth)
			{
				if (worldY > 71 && worldY < 80)
				{
					if (dragObj == false)
					{
						if (panel2 == false)
						{
							dragObj = true;
							GameObject* testWater = new GameObject(GameObject::MAP_TREE);
							testWater->pos.Set(worldX, worldY, 1);
							testWater->fresh = true;
							testWater->active = true;
							testMap.forceAddSingleProp(testWater);
						}
						else
						{
							dragObj = true;
							GameObject* testWater = new GameObject(GameObject::MAP_LION);
							testWater->pos.Set(worldX, worldY, 1);
							testWater->scale.Set(6, 6, 6);
							testWater->fresh = true;
							testWater->active = true;
							testMap.forceAddSingleProp(testWater);
						}
					}
				}
				else if (worldY > 57 && worldY < 67)
				{
					if (dragObj == false)
					{
						if (panel2 == false)
						{
							dragObj = true;
							GameObject* testWater = new GameObject(GameObject::MAP_ROCK);
							testWater->pos.Set(worldX, worldY, 1);
							testWater->scale.Set(6, 6, 6);
							testWater->fresh = true;
							testWater->active = true;
							testMap.forceAddSingleProp(testWater);
						}
						else
						{
							dragObj = true;
							GameObject* testWater = new GameObject(GameObject::MAP_ZEBRA);
							testWater->pos.Set(worldX, worldY, 1);
							testWater->scale.Set(7, 6, 6);
							testWater->fresh = true;
							testWater->active = true;
							testMap.forceAddSingleProp(testWater);
						}
					}
				}
				else if (worldY > 44 && worldY < 54)
				{
					if (dragObj == false)
					{
						if (panel2 == false)
						{
							dragObj = true;
							GameObject* testWater = new GameObject(GameObject::MAP_WATER);
							testWater->pos.Set(worldX, worldY, 1);
							testWater->scale.Set(6, 6, 6);
							testWater->fresh = true;
							testWater->active = true;
							testMap.forceAddSingleProp(testWater);
						}
						else
						{
							dragObj = true;
							GameObject* testWater = new GameObject(GameObject::MAP_RHINO);
							testWater->pos.Set(worldX, worldY, 1);
							testWater->scale.Set(9, 6, 6);
							testWater->fresh = true;
							testWater->active = true;
							testMap.forceAddSingleProp(testWater);
						}
					}
				}
				else if (worldY > 30 && worldY < 40)
				{
					if (dragObj == false)
					{
						if (panel2 == false)
						{
							dragObj = true;
							GameObject* testWater = new GameObject(GameObject::MAP_MUD);
							testWater->pos.Set(worldX, worldY, 1);
							testWater->scale.Set(4, 4, 4);
							testWater->fresh = true;
							testWater->active = true;
							testMap.forceAddSingleProp(testWater);
						}
						else
						{
							dragObj = true;
							GameObject* testWater = new GameObject(GameObject::MAP_HUMAN);
							testWater->pos.Set(worldX, worldY, 1);
							testWater->scale.Set(3, 6, 6);
							testWater->fresh = true;
							testWater->active = true;
							testMap.forceAddSingleProp(testWater);
						}
					}
				}
				else if (worldY > 16 && worldY < 26)
				{
					panel2 = !panel2;
				}
				else if (worldY > 4 && worldY < 14)
				{
					deleteMode = 1;
				}
				else
				{
					dragObj = false;
				}
			}
			if (worldX > 0.454236f * m_worldWidth && worldX < 0.8412931f * m_worldWidth && worldY > 87 && worldY < 97)
			{
				editName = true;
			}
			else if ( worldX < 0.0834f * m_worldWidth && worldY < 15)
			{
				dragObj = true;
				GameObject* testWater = new GameObject(GameObject::MAP_OBJECTIVE);
				testWater->pos.Set(worldX, worldY, 1);
				testWater->scale.Set(5, 5, 5);
				testWater->fresh = true;
				testWater->active = true;
				testMap.forceAddSingleProp(testWater);
			}
			else if (worldX > 0.8625f * m_worldWidth && worldX < 0.918f * m_worldWidth && worldY > 87 && worldY < 97)
			{
				for (std::vector<GameObject *>::iterator it = testMap.mapProps.begin(); it != testMap.mapProps.end(); ++it)
				{
					GameObject *go = (GameObject *)* it;
					go->active = false;
					go->dead = true;
				}
				if (mapName == "")
				{
					cout << "No map name entered" << endl;
				}
				else
				{
					TextFile* map = new TextFile(TextFile::MAP);

					NameofMap = map->CreateMapFile(mapName, true);
					RenderMapFile();
				}
			}
			else if (worldX > 0.9362068f * m_worldWidth && worldX < 0.9827586f * m_worldWidth && worldY > 88 && worldY < 96)
			{
				string FileName;
					if (mapName == "")
					{
						cout << "No Name Entered" << endl;
					}
					else
					{
						TextFile* map = new TextFile(TextFile::MAP);
						if (map->CreateMapFile(mapName, false) == "fail")
						{
							cout << "Map name already in use." << endl;
						}
						else
						{
							FileName = map->CreateMapFile(mapName, false);
						}
					}
				for (std::vector<GameObject *>::iterator it = testMap.mapProps.begin(); it != testMap.mapProps.end(); ++it)
				{
					GameObject *go = (GameObject *)* it;
					if (go->active)
					{
						if (go->type == GameObject::MAP_TREE)
						{
							TextFile *tree = new TextFile(TextFile::MAP);
							tree->SetData("tree", go->pos.x - mapPosition.x, go->pos.y - mapPosition.y);
							tree->WriteFile(FileName);
						}
						else if (go->type == GameObject::MAP_ROCK)
						{
							TextFile *rock = new TextFile(TextFile::MAP);
							rock->SetData("rock", go->pos.x - mapPosition.x, go->pos.y - mapPosition.y);
							rock->WriteFile(FileName);
						}
						else if (go->type == GameObject::MAP_WATER)
						{
							TextFile *water = new TextFile(TextFile::MAP);
							water->SetData("water", go->pos.x - mapPosition.x, go->pos.y - mapPosition.y);
							water->WriteFile(FileName);
						}
						else if (go->type == GameObject::MAP_MUD)
						{
							TextFile *mud = new TextFile(TextFile::MAP);
							mud->SetData("mud", go->pos.x - mapPosition.x, go->pos.y - mapPosition.y);
							mud->WriteFile(FileName);
						}
						else if (go->type == GameObject::MAP_LION)
						{
							TextFile *lion = new TextFile(TextFile::MAP);
							lion->SetData("lion", go->pos.x - mapPosition.x, go->pos.y - mapPosition.y);
							lion->WriteFile(FileName);
						}
						else if (go->type == GameObject::MAP_ZEBRA)
						{
							TextFile *zebra = new TextFile(TextFile::MAP);
							zebra->SetData("zebra", go->pos.x - mapPosition.x, go->pos.y - mapPosition.y);
							zebra->WriteFile(FileName);
						}
						else if (go->type == GameObject::MAP_RHINO)
						{
							TextFile *rhino = new TextFile(TextFile::MAP);
							rhino->SetData("rhino", go->pos.x - mapPosition.x, go->pos.y - mapPosition.y);
							rhino->WriteFile(FileName);
						}
						else if (go->type == GameObject::MAP_HUMAN)
						{
							TextFile *human = new TextFile(TextFile::MAP);
							human->SetData("human", go->pos.x - mapPosition.x, go->pos.y - mapPosition.y);
							human->WriteFile(FileName);
						}
						else if (go->type == GameObject::MAP_OBJECTIVE)
						{
							TextFile *human = new TextFile(TextFile::MAP);
							human->SetData("objective", go->pos.x - mapPosition.x, go->pos.y - mapPosition.y);
							human->WriteFile(FileName);
						}
					}
				}
				cout << "SAVING" << endl;
			}
			else
			{
				editName = false;
				if (dragObj == false)
				{
					std::cout << "DRAGGING EXISTING" << endl;
					for (std::vector<GameObject *>::iterator it = testMap.mapProps.begin(); it != testMap.mapProps.end(); ++it)
					{
						GameObject *go = (GameObject *)*it;
						if (go->active == false)
							continue;
						if ((go->pos - Vector3(worldX, worldY, 1)).Length() < go->scale.x && dragObj == false)
						{
							go->active = false;
							dragObj = true;
							GameObject* newItem = new GameObject(go->type);
							newItem->pos.Set(worldX, worldY, 1);
							newItem->fresh = true;
							newItem->active = true;
							newItem->scale = go->scale;
							testMap.forceAddSingleProp(newItem);

							break;
						}
					}
				}
			}
		}
		static bool Loaded = false;
		if (Application::IsKeyPressed('L') && !Loaded)
		{
			Loaded = true;
			if (mapName == "")
			{
				cout << "No map name entered" << endl;
			}
			else
			{
				TextFile* map = new TextFile(TextFile::MAP);			

					NameofMap = map->CreateMapFile(mapName, true);
					RenderMapFile();
			}
		}
		else if (!Application::IsKeyPressed('L') && Loaded)
		{
			Loaded = false;
		}
		if (bLButtonState && !Application::IsMousePressed(0))
		{
			double x, y;
			Application::GetCursorPos(&x, &y);
			int w = Application::GetWindowWidth();
			int h = Application::GetWindowHeight();
			float worldX = x * m_worldWidth / w;
			float worldY = (h - y) * m_worldHeight / h;
			bLButtonState = false;
			dragObj = false;

			if (deleteMode == 1)
			{
				deleteMode = 2;
			}
			if (deleteMode == 3)
			{
				for (std::vector<GameObject *>::iterator it = testMap.mapProps.begin(); it != testMap.mapProps.end(); ++it)
				{
					GameObject *go = (GameObject *)*it;
					if (go->pos.x > newMouseX && go->pos.x < worldX)
					{
						if (go->pos.y > newMouseY && go->pos.y < worldY)
						{
							go->active = false;
						}
						else if (go->pos.y < newMouseY && go->pos.y > worldY)
						{
							go->active = false;
						}
					}
					else if (go->pos.x < newMouseX && go->pos.x > worldX)
					{
						if (go->pos.y > newMouseY && go->pos.y < worldY)
						{
							go->active = false;
						}
						else if (go->pos.y < newMouseY && go->pos.y > worldY)
						{
							go->active = false;
						}
					}
				}
				deleteMode = 0;
			}
		}
		static bool bRButtonState = false;

		if (!bLButtonState && Application::IsMousePressed(1))
		{
			bRButtonState = true;
			std::cout << "RBUTTON DOWN" << std::endl;
			double x, y;
			Application::GetCursorPos(&x, &y);
			int w = Application::GetWindowWidth();
			int h = Application::GetWindowHeight();
			float worldX = x * m_worldWidth / w;
			float worldY = (h - y) * m_worldHeight / h;
			if (!firstDrag)
			{
				newMouseX = worldX;
				newMouseY = worldY;
				firstDrag = true;
			}
			else
			{
				diffx = worldX - newMouseX;
				diffy = worldY - newMouseY;
				mapPosition = Vector3(mapPosition.x + diffx, mapPosition.y + diffy, 1);
				newMouseX = worldX;
				newMouseY = worldY;
			}
		}
		else if (bRButtonState && !Application::IsMousePressed(1))
		{
			bRButtonState = false;
			firstDrag = false;
		}
		if (Application::IsKeyPressed(VK_F1))
		{
			testMode = true;
			deleteMode = 0;
			for (std::vector<GameObject *>::iterator it = testMap.mapProps.begin(); it != testMap.mapProps.end(); ++it)
			{
				GameObject *go = (GameObject *)*it;
				if (go->active == false)
				{
					go->dead = true;
				}
				if (go->dead == true)
				{
					continue;
				}
				if (go->type == GameObject::MAP_LION)
				{
					animalStat->GetAnimalStat("Lion");
					enemy* animal = newEnemy(go->pos.x - mapPosition.x, go->pos.y - mapPosition.y, 0, 2, animalStat->get_stamina(), animalStat->get_speed(), animalStat->get_strength());
					animal->setVel(Math::RandFloatMinMax(-10, 10), Math::RandFloatMinMax(-10, 10), 0);
					animal->setLeader(0);
					enemyList.push_back(animal);
					go->active = false;
				}
				else if (go->type == GameObject::MAP_ZEBRA)
				{
					animalStat->GetAnimalStat("Zebra");
					enemy* animal = newEnemy(go->pos.x - mapPosition.x, go->pos.y - mapPosition.y, 0, 0, animalStat->get_stamina(), animalStat->get_speed(), animalStat->get_strength());
					animal->setVel(Math::RandFloatMinMax(-10, 10), Math::RandFloatMinMax(-10, 10), 0);
					animal->setLeader(0);
					enemyList.push_back(animal);
					go->active = false;
				}
				else if (go->type == GameObject::MAP_RHINO)
				{
					animalStat->GetAnimalStat("Rhino");
					enemy* animal = newEnemy(go->pos.x - mapPosition.x, go->pos.y - mapPosition.y, 0, 1, animalStat->get_stamina(), animalStat->get_speed(), animalStat->get_strength());
					animal->setVel(Math::RandFloatMinMax(-10, 10), Math::RandFloatMinMax(-10, 10), 0);
					animal->setLeader(0);
					enemyList.push_back(animal);
					go->active = false;
				}
				else if (go->type == GameObject::MAP_HUMAN)
				{
					animalStat->GetAnimalStat("Human");
					enemy* animal = newEnemy(go->pos.x - mapPosition.x, go->pos.y - mapPosition.y, 0, 3, animalStat->get_stamina(), animalStat->get_speed(), animalStat->get_strength());
					animal->setVel(Math::RandFloatMinMax(-10, 10), Math::RandFloatMinMax(-10, 10), 0);
					animal->setLeader(0);
					enemyList.push_back(animal);
					go->active = false;
				}
			}
		}
		if (panel2)
		{
			if (panel2x > 0)
			{
				panel2x--;
			}
			else
			{
				panel2x = 0;
			}
		}
		else
		{
			if (panel2x < 15)
			{
				panel2x++;
			}
			else
			{
				panel2x = 15;
			}
		}
	}
	else if (testMode == true)
	{
		if (Application::IsKeyPressed(VK_F2))
		{
			player1->vel.SetZero();
			for (std::vector<GameObject *>::iterator it = testMap.mapProps.begin(); it != testMap.mapProps.end(); ++it)
			{
				GameObject *go = (GameObject *)*it;
				if (go->dead != true)
					go->active = true;
			}
			for (std::vector<enemy *>::iterator it = enemyList.begin(); it != enemyList.end(); ++it)
			{
				enemy *go = (enemy *)*it;
				go->setActive(false);
			}
			testMode = false;
			deleteMode = 0;
		}
	}
}

bool SceneSP3::MapExist(string FileName)
{
	ifstream checkFile(FileName);
	if (checkFile.good())
	{
		return true;
	}
	else
	{
		return false;
	}
}

	//corner
	//wall = FetchGO();
	//wall->type = GameObject::GO_WALL
	//wall->pos.Set(112.5f, 88, 1);
	//wall->normal = Vector3(0.5f, 0.5f, 0).Normalized();
	//wall->active = true;
	//wall->scale.Set(2, 6, 1);
	//wall->rotationAngle = 45;

void SceneSP3::RenderLasso(lasso *go)
{
	if (go->getLassoState() != 0)
	{

		modelStack.PushMatrix();
		modelStack.Translate(go->getLassoPos().x, go->getLassoPos().y, 2);
		modelStack.Scale(3, 3, 1);
		RenderMesh(meshList[GEO_ENEMYBULLET], false);
		modelStack.PopMatrix();

		float yDist = go->getLassoPos().y - player1->pos.y;
		float xDist = go->getLassoPos().x - player1->pos.x;
		float angleDiff = Math::RadianToDegree(atan2(yDist, xDist));
		

		modelStack.PushMatrix();
		modelStack.Translate((go->getLassoPos().x + player1->pos.x) / 2, (go->getLassoPos().y + player1->pos.y) / 2, 2);
		modelStack.Rotate(angleDiff, 0 ,0, 1);
		modelStack.Scale((go->getLassoPos() - player1->pos).Length(), .2, 1);
		RenderMesh(meshList[GEO_ROPE], false);
		modelStack.PopMatrix();
	}
}

void SceneSP3::RenderGO(GameObject *go)
{
	switch (go->type)
	{
	case GameObject::GO_BALL:
		//Exercise 4: render a sphere using scale and pos
		modelStack.PushMatrix();
		modelStack.Translate(go->pos.x, go->pos.y, go->pos.z);
		modelStack.Rotate(go->rotationAngle, 0, 0, 1);
		modelStack.Rotate(go->ballrotated, 0, 1, 0);
		modelStack.Rotate(45, 1, 1, 0);
		modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
		RenderMesh(meshList[GEO_BALL], true);
		modelStack.PopMatrix();

		//Exercise 11: think of a way to give balls different colors
		break;
	case GameObject::GO_WALL:
		modelStack.PushMatrix();
		modelStack.Translate(go->pos.x, go->pos.y, go->pos.z);
		modelStack.Rotate(go->rotationAngle, 0, 0, 1);
		modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
		if (go->pos.x < 20)
			RenderMesh(meshList[GEO_GOAL1], false);
		if (go->pos.x > 110)
			RenderMesh(meshList[GEO_GOAL2], false);
		else
			RenderMesh(meshList[GEO_CUBE], false);
		modelStack.PopMatrix();

		break;
	case GameObject::GO_CAR:
		if (gameStates == states::s_Tutorial ||
			gameStates == states::s_Level2 ||
			gameStates == states::s_Level3 ||
			gameStates == states::s_LevelBoss ||
			gameStates == states::s_MapEditor && testMode == 1)
		{
			modelStack.PushMatrix();
			modelStack.Translate(go->pos.x, go->pos.y, go->pos.z);
			modelStack.Rotate(-10, 1, 0, 0);
			modelStack.Rotate(go->rotationAngle, 0, 0, 1);
			modelStack.Rotate(90, 0, 0, 1);
			modelStack.Rotate(90, 1, 0, 0);
			modelStack.Scale(go->scale.x / 10, go->scale.y / 5, go->scale.z / 2);
			modelStack.Scale(2, 2.5, 3.5);
			RenderMesh(meshList[GEO_DISPLAY_CAR1], false);
			modelStack.PopMatrix();
		}
		
		break;
	case GameObject::GO_PILLAR:
		modelStack.PushMatrix();
		modelStack.Translate(go->pos.x, go->pos.y, go->pos.z);
		modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
		if (go->pos.x < 20)
			RenderMesh(meshList[GEO_POST1], false);
		if (go->pos.x > 110)
			RenderMesh(meshList[GEO_POST2], false);
		modelStack.PopMatrix();
		break;
	case GameObject::GO_GOAL:

		break;
	case GameObject::GO_POWERUP:
		modelStack.PushMatrix();
		modelStack.Translate(go->pos.x, go->pos.y, go->pos.z);
		modelStack.Scale(go->scale.x, go->scale.y, 1);
		RenderMesh(meshList[GEO_POWERUP], false);
		modelStack.PopMatrix();
		break;
	case GameObject::GO_SAWBLADE:
		modelStack.PushMatrix();
		modelStack.Translate(go->pos.x, go->pos.y, go->pos.z + Math::RandFloatMinMax(-1.f, 1.f));
		modelStack.Scale(go->scale.x, go->scale.y, 1);
		RenderMesh(meshList[GEO_SAWBLADE], false);
		modelStack.PopMatrix();
		break;
	case GameObject::GO_PARTICLE:
		modelStack.PushMatrix();
		modelStack.Translate(go->pos.x, go->pos.y, go->pos.z);
		modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
		RenderMesh(meshList[GEO_ICE], false);
		modelStack.PopMatrix();
		break;
	}
}

void SceneSP3::RenderEnemy(enemy *go)
{
	if (go->getActive() == 1 && (go->getPos().x) < m_worldWidth && (go->getPos().x ) > 0 && go->getPos().y < m_worldHeight && go->getPos().y > 0)
	{

		if (center.x - go->getPos().x > -(testMap.getMapSize().x) * 5 && center.x - go->getPos().x < testMap.getMapSize().x * 5 && center.y - go->getPos().y > -(testMap.getMapSize().y) * 5 && center.y - go->getPos().y < testMap.getMapSize().y * 5)
		{

			if (go->getType() == 0)
			{
				float angle = Math::RadianToDegree(atan2(go->getVel().y, go->getVel().x));
				modelStack.PushMatrix();
				modelStack.Translate(go->getPos().x, go->getPos().y, go->getPos().z);
				//modelStack.Rotate(angle, 0, 0, 1);
				modelStack.Scale(7, 7, 3);
				RenderMesh(meshList[GEO_ZEBRA], true);
				modelStack.PopMatrix();
			}
			else if (go->getType() == 1)
			{
				float angle = Math::RadianToDegree(atan2(go->getVel().y, go->getVel().x));
				modelStack.PushMatrix();
				modelStack.Translate(go->getPos().x, go->getPos().y, go->getPos().z);
				//modelStack.Rotate(angle, 0, 0, 1);
				modelStack.Scale(7, 7, 3);
				RenderMesh(meshList[GEO_RHINO], true);
				modelStack.PopMatrix();
			}
			else if (go->getType() == 2)
			{
				float angle = Math::RadianToDegree(atan2(go->getVel().y, go->getVel().x));
				modelStack.PushMatrix();
				modelStack.Translate(go->getPos().x, go->getPos().y, go->getPos().z);
				//modelStack.Rotate(angle, 0, 0, 1);
				modelStack.Scale(7, 7, 3);
				RenderMesh(meshList[GEO_LION], true);
				modelStack.PopMatrix();
			}
			else if (go->getType() == 3)
			{
				float angle = Math::RadianToDegree(atan2(go->getVel().y, go->getVel().x));
				modelStack.PushMatrix();
				modelStack.Translate(go->getPos().x, go->getPos().y, go->getPos().z);
				//modelStack.Rotate(angle, 0, 0, 1);
				modelStack.Scale(7, 7, 3);
				RenderMesh(meshList[GEO_HUMAN], true);
				modelStack.PopMatrix();
			}
		}
	}

	/*modelStack.PushMatrix();
	modelStack.Translate(objective.x, objective.y, objective.z);
	modelStack.Scale(5, 5, 3);
	RenderMesh(meshList[GEO_SHIP], true);
	modelStack.PopMatrix();*/
}
void SceneSP3::UpgradeController()
{
	double x, y;
	Application::GetCursorPos(&x, &y);
	int w = Application::GetWindowWidth();
	int h = Application::GetWindowHeight();
	float worldX = x * m_worldWidth / w;
	float worldY = (h - y) * m_worldHeight / h;
	static bool bLButtonState = false;


	if (bLButtonState && !Application::IsMousePressed(0))
	{
		bLButtonState = false;
		//Buying upgrades
		if (gameStates == states::s_Upgrade_Cars1)
		{
			if (worldX > 0.75549f * m_worldWidth && worldX < 0.82577f * m_worldWidth)
			{
				if (worldY > 16.8f && worldY < 22.6f)
				{
					car1Bought = true;
				}
			}
		}
		if (gameStates == states::s_Upgrade_Cars2)
		{
			if (worldX > 0.75549f * m_worldWidth && worldX < 0.82577f * m_worldWidth)
			{
				if (worldY > 16.8f && worldY < 22.6f)
				{
					InitCarStat("Car2");

					if (money >= cost[0] && car2Bought == false)
					{
						car2Bought = true;
						money -= cost[0];
					}
				}
			}
		}
		if (gameStates == states::s_Upgrade_Cars3)
		{
			if (worldX > 0.75549f * m_worldWidth && worldX < 0.82577f * m_worldWidth)
			{
				if (worldY > 16.8f && worldY < 22.6f)
				{
					InitCarStat("Car3");
					if (money >= cost[1] && car3Bought == false)
					{
						car3Bought = true;
						money -= cost[1];
					}
				}
			}
		}
		if (gameStates == states::s_Upgrade_Tires1)
		{
			if (worldX > 0.75549f * m_worldWidth && worldX < 0.82577f * m_worldWidth)
			{
				if (worldY > 16.8f && worldY < 22.6f)
				{
					tire1Bought = true;
				}
			}
		}
		if (gameStates == states::s_Upgrade_Tires2)
		{
			if (worldX > 0.75549f * m_worldWidth && worldX < 0.82577f * m_worldWidth)
			{
				if (worldY > 16.8f && worldY < 22.6f)
				{
					if (money >= cost[2] && tire2Bought == false)
					{
						money -= cost[2];
						if (car1Bought == true)
						{
							int handling = player1->playerCar.handling;
							handling += 4;
							TextFile *HandlingStat = new TextFile();
							HandlingStat->SetCarStat("Car1", "handling", to_string(handling));
							HandlingStat->SaveFile("SavedCarStats.txt", "tempsave.txt");
						}
						if (car2Bought == true)
						{
							int handling = player1->playerCar.handling;
							handling += 4;
							TextFile *HandlingStat = new TextFile();
							HandlingStat->SetCarStat("Car2", "handling", to_string(handling));
						}
						if (car3Bought == true)
						{
							int handling = player1->playerCar.handling;
							handling += 4;
							TextFile *HandlingStat = new TextFile();
							HandlingStat->SetCarStat("Car3", "handling", to_string(handling));
						}
					}
					tire2Bought = true;
				}
			}
		}
		if (gameStates == states::s_Upgrade_Tires3)
		{
			if (worldX > 0.75549f * m_worldWidth && worldX < 0.82577f * m_worldWidth)
			{
				if (worldY > 16.8f && worldY < 22.6f)
				{
					if (money >= cost[3] && tire3Bought == false)
					{
						money -= cost[3];
						if (car1Bought == true)
						{
							int handling = player1->playerCar.handling;
							handling += 5;
							TextFile *HandlingStat = new TextFile();
							HandlingStat->SetCarStat("Car1", "handling", to_string(handling));
						}
						if (car2Bought == true)
						{
							int handling = player1->playerCar.handling;
							handling += 5;
							TextFile *HandlingStat = new TextFile();
							HandlingStat->SetCarStat("Car2", "handling", to_string(handling));
						}
						if (car3Bought == true)
						{
							int handling = player1->playerCar.handling;
							handling += 5;
							TextFile *HandlingStat = new TextFile();
							HandlingStat->SetCarStat("Car3", "handling", to_string(handling));
						}
					}
					tire3Bought = true;
				}
			}
		}
		if (gameStates == states::s_Upgrade_Lasso1)
		{
			if (worldX > 0.75549f * m_worldWidth && worldX < 0.82577f * m_worldWidth)
			{
				if (worldY > 16.8f && worldY < 22.6f)
				{
					lasso1Bought = true;
				}
			}
		}
		if (gameStates == states::s_Upgrade_Lasso2)
		{
			if (worldX > 0.75549f * m_worldWidth && worldX < 0.82577f * m_worldWidth)
			{
				if (worldY > 16.8f && worldY < 22.6f)
				{
					if (money >= cost[4] && lasso2Bought == false)
					{
						money -= cost[4];
						/*if (car1Bought == true)
						{
						int LassoLength = player1->playerCar.lassoLength;
						LassoLength += 5;
						TextFile *LassoLengthStat = new TextFile();
						LassoLengthStat->SetCarStat("Car1", "lassolength", to_string(LassoLength));

						int LassoStrength = player1->playerCar.lassoStrength;
						LassoStrength += 5;
						TextFile *LassoStrengthStat = new TextFile();
						LassoStrengthStat->SetCarStat("Car1", "lassostrength", to_string(LassoStrength));
						}
						if (car2Bought == true)
						{
						int LassoLength = player1->playerCar.lassoLength;
						LassoLength += 5;
						TextFile *LassoLengthStat = new TextFile();
						LassoLengthStat->SetCarStat("Car2", "lassolength", to_string(LassoLength));

						int LassoStrength = player1->playerCar.lassoStrength;
						LassoStrength += 5;
						TextFile *LassoStrengthStat = new TextFile();
						LassoStrengthStat->SetCarStat("Car2", "lassostrength", to_string(LassoStrength));
						}
						if (car3Bought == true)
						{
						int LassoLength = player1->playerCar.lassoLength;
						LassoLength += 5;
						TextFile *LassoLengthStat = new TextFile();
						LassoLengthStat->SetCarStat("Car3", "lassolength", to_string(LassoLength));

						int LassoStrength = player1->playerCar.lassoStrength;
						LassoStrength += 5;
						TextFile *LassoStrengthStat = new TextFile();
						LassoStrengthStat->SetCarStat("Car3", "lassostrength", to_string(LassoStrength));
						}*/
						Dalasso->setLassoRange(50.f);
						Dalasso->setLassoSpd(1.0f);
					}
					lasso2Bought = true;
				}
			}
		}
		if (gameStates == states::s_Upgrade_Lasso3)
		{
			if (worldX > 0.75549f * m_worldWidth && worldX < 0.82577f * m_worldWidth)
			{
				if (worldY > 16.8f && worldY < 22.6f)
				{
					if (money >= cost[5] && lasso3Bought == false)
					{
						money -= cost[5];
						/*if (car1Bought == true)
						{
						int LassoLength = player1->playerCar.lassoLength;
						LassoLength += 6;
						TextFile *LassoLengthStat = new TextFile();
						LassoLengthStat->SetCarStat("Car1", "lassolength", to_string(LassoLength));

						int LassoStrength = player1->playerCar.lassoStrength;
						LassoStrength += 6;
						TextFile *LassoStrengthStat = new TextFile();
						LassoStrengthStat->SetCarStat("Car1", "lassostrength", to_string(LassoStrength));
						}
						if (car2Bought == true)
						{
						int LassoLength = player1->playerCar.lassoLength;
						LassoLength += 6;
						TextFile *LassoLengthStat = new TextFile();
						LassoLengthStat->SetCarStat("Car2", "lassolength", to_string(LassoLength));

						int LassoStrength = player1->playerCar.lassoStrength;
						LassoStrength += 6;
						TextFile *LassoStrengthStat = new TextFile();
						LassoStrengthStat->SetCarStat("Car2", "lassostrength", to_string(LassoStrength));
						}
						if (car3Bought == true)
						{
						int LassoLength = player1->playerCar.lassoLength;
						LassoLength += 6;
						TextFile *LassoLengthStat = new TextFile();
						LassoLengthStat->SetCarStat("Car3", "lassolength", to_string(LassoLength));

						int LassoStrength = player1->playerCar.lassoStrength;
						LassoStrength += 6;
						TextFile *LassoStrengthStat = new TextFile();
						LassoStrengthStat->SetCarStat("Car3", "lassostrength", to_string(LassoStrength));
						}*/
						Dalasso->setLassoRange(60.f);
						Dalasso->setLassoSpd(1.2f);
					}
					lasso3Bought = true;
				}
			}
		}
		if (gameStates == states::s_Upgrade_Darts1)
		{
			if (worldX > 0.75549f * m_worldWidth && worldX < 0.82577f * m_worldWidth)
			{
				if (worldY > 16.8f && worldY < 22.6f)
				{
					dart1Bought = true;
				}
			}
		}
		if (gameStates == states::s_Upgrade_Darts2)
		{
			if (worldX > 0.75549f * m_worldWidth && worldX < 0.82577f * m_worldWidth)
			{
				if (worldY > 16.8f && worldY < 22.6f)
				{
					if (money >= cost[6] && dart2Bought == false)
					{
						money -= cost[6];
						if (car1Bought == true)
						{
							int TranqCount = player1->playerCar.tranqCount;
							TranqCount += 2;
							TextFile *TranqCountStat = new TextFile();
							TranqCountStat->SetCarStat("Car1", "tranqcount", to_string(TranqCount));

							int TranqDuration = player1->playerCar.tranqDuration;
							TranqDuration += 2;
							TextFile *TranqDurationStat = new TextFile();
							TranqDurationStat->SetCarStat("Car1", "tranqduration", to_string(TranqDuration));
						}
						if (car2Bought == true)
						{
							int TranqCount = player1->playerCar.tranqCount;
							TranqCount += 2;
							TextFile *TranqCountStat = new TextFile();
							TranqCountStat->SetCarStat("Car2", "tranqcount", to_string(TranqCount));

							int TranqDuration = player1->playerCar.tranqDuration;
							TranqDuration += 2;
							TextFile *TranqDurationStat = new TextFile();
							TranqDurationStat->SetCarStat("Car2", "tranqduration", to_string(TranqDuration));
						}
						if (car3Bought == true)
						{
							int TranqCount = player1->playerCar.tranqCount;
							TranqCount += 2;
							TextFile *TranqCountStat = new TextFile();
							TranqCountStat->SetCarStat("Car3", "tranqcount", to_string(TranqCount));

							int TranqDuration = player1->playerCar.tranqDuration;
							TranqDuration += 2;
							TextFile *TranqDurationStat = new TextFile();
							TranqDurationStat->SetCarStat("Car3", "tranqduration", to_string(TranqDuration));
						}
					}
					dart2Bought = true;
				}
			}
		}
		if (gameStates == states::s_Upgrade_Darts3)
		{
			if (worldX > 0.75549f * m_worldWidth && worldX < 0.82577f * m_worldWidth)
			{
				if (worldY > 16.8f && worldY < 22.6f)
				{
					if (money >= cost[7] && dart3Bought == false)
					{
						money -= cost[7];
						if (car1Bought == true)
						{
							int TranqCount = player1->playerCar.tranqCount;
							TranqCount += 3;
							TextFile *TranqCountStat = new TextFile();
							TranqCountStat->SetCarStat("Car1", "tranqcount", to_string(TranqCount));

							int TranqDuration = player1->playerCar.tranqDuration;
							TranqDuration += 3;
							TextFile *TranqDurationStat = new TextFile();
							TranqDurationStat->SetCarStat("Car1", "tranqduration", to_string(TranqDuration));
						}
						if (car2Bought == true)
						{
							int TranqCount = player1->playerCar.tranqCount;
							TranqCount += 3;
							TextFile *TranqCountStat = new TextFile();
							TranqCountStat->SetCarStat("Car2", "tranqcount", to_string(TranqCount));

							int TranqDuration = player1->playerCar.tranqDuration;
							TranqDuration += 3;
							TextFile *TranqDurationStat = new TextFile();
							TranqDurationStat->SetCarStat("Car2", "tranqduration", to_string(TranqDuration));
						}
						if (car3Bought == true)
						{
							int TranqCount = player1->playerCar.tranqCount;
							TranqCount += 3;
							TextFile *TranqCountStat = new TextFile();
							TranqCountStat->SetCarStat("Car3", "tranqcount", to_string(TranqCount));

							int TranqDuration = player1->playerCar.tranqDuration;
							TranqDuration += 3;
							TextFile *TranqDurationStat = new TextFile();
							TranqDurationStat->SetCarStat("Car3", "tranqduration", to_string(TranqDuration));
						}
					}
					dart3Bought = true;
				}
			}
		}
		if (gameStates == states::s_Upgrade)
		{
			if (worldX > 0.0f * m_worldWidth && worldX < 0.33073f * m_worldWidth)
			{
				if (worldY > 68.472f && worldY < 76.111f)
				{
					gameStates = states::s_Upgrade_Cars1;
				}
				if (worldY > 56.111f && worldY < 63.75f)
				{
					gameStates = states::s_Upgrade_Tires1;
				}
				if (worldY > 43.333f && worldY < 50.833f)
				{
					gameStates = states::s_Upgrade_Lasso1;
				}
				if (worldY > 30.694f && worldY < 38.333f)
				{
					gameStates = states::s_Upgrade_Darts1;
				}
			}
			if (worldX > 0.816f * m_worldWidth && worldX < 0.9423f * m_worldWidth)
			{
				if (worldY > 10.4f && worldY < 18.1f)
				{
					if (car1Bought && !car2Bought)
					{
						InitCarStat("Car1", "SavedCarStats.txt");
					}
					else if (car2Bought && !car3Bought)
					{
						InitCarStat("Car2", "SavedCarStats.txt");
					}
					else if (car3Bought)
					{
						InitCarStat("Car3", "SavedCarStats.txt");
					}
					gameStates = states::s_LevelSelect;
				}
			}
		}
	}
	if (!bLButtonState && Application::IsMousePressed(0))
	{
		bLButtonState = true;
	}
}

void SceneSP3::RenderProps(playMap* map)
{
	glDisable(GL_DEPTH_TEST);
	for (std::vector<GameObject *>::iterator it = map->mapProps.begin(); it != map->mapProps.end(); ++it)
	{
		GameObject *go = (GameObject *)*it;
		if (go->active == false)
			continue;
		if (abs(go->pos.x - player1->pos.x) > m_worldWidth || abs(go->pos.y - player1->pos.y) > m_worldHeight)
		{
			continue;
		}
		if (go->type == GameObject::MAP_TREE)
		{
			modelStack.PushMatrix();
			modelStack.Translate(go->pos.x, go->pos.y, go->pos.z + 2);
			modelStack.Scale(go->scale.x * 5, go->scale.y * 5, go->scale.z);
			RenderMesh(meshList[GEO_TREETOP], true);
			modelStack.PopMatrix();
		}
		else if (go->type == GameObject::MAP_ROCK)
		{
			glEnable(GL_DEPTH_TEST);
			modelStack.PushMatrix();
			//if (player1->pos.y > go->pos.y)
			//{
			//	go->pos.z = player1->pos.z + 5;
			//}
			//else
			//{
			//	go->pos.z = player1->pos.z - 5;
			//}
			modelStack.Translate(go->pos.x, go->pos.y, go->pos.z - 2);
			modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
			RenderMesh(meshList[GEO_ROCK], true);
			modelStack.PopMatrix();
			glDisable(GL_DEPTH_TEST);
		}
		else if (go->type == GameObject::MAP_WATER)
		{
			glEnable(GL_DEPTH_TEST);
			modelStack.PushMatrix();
			modelStack.Translate(go->pos.x, go->pos.y, go->pos.z - 2);
			modelStack.Rotate(90, 1, 0, 0);
			modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
			RenderMesh(meshList[GEO_ICE], true);
			modelStack.PopMatrix();
			glDisable(GL_DEPTH_TEST);
		}
		else if (go->type == GameObject::MAP_MUD)
		{
			glEnable(GL_DEPTH_TEST);
			modelStack.PushMatrix();
			modelStack.Translate(go->pos.x, go->pos.y, go->pos.z);
			modelStack.Rotate(90, 1, 0, 0);
			modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
			RenderMesh(meshList[GEO_MUD], true);
			modelStack.PopMatrix();
			glDisable(GL_DEPTH_TEST);
		}
		else if (go->type == GameObject::MAP_LION)
		{
			modelStack.PushMatrix();
			modelStack.Translate(go->pos.x, go->pos.y, go->pos.z);
			modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
			RenderMesh(meshList[GEO_LION], true);
			modelStack.PopMatrix();
		}
		else if (go->type == GameObject::MAP_ZEBRA)
		{
			modelStack.PushMatrix();
			modelStack.Translate(go->pos.x, go->pos.y, go->pos.z);
			modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
			RenderMesh(meshList[GEO_ZEBRA], true);
			modelStack.PopMatrix();
		}
		else if (go->type == GameObject::MAP_RHINO)
		{
			modelStack.PushMatrix();
			modelStack.Translate(go->pos.x, go->pos.y, go->pos.z);
			modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
			RenderMesh(meshList[GEO_RHINO], true);
			modelStack.PopMatrix();
		}
		else if (go->type == GameObject::MAP_HUMAN)
		{
			modelStack.PushMatrix();
			modelStack.Translate(go->pos.x, go->pos.y, go->pos.z);
			modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
			RenderMesh(meshList[GEO_HUMAN], true);
			modelStack.PopMatrix();
		}
		else if (go->type == GameObject::MAP_OBJECTIVE)
		{
			modelStack.PushMatrix();
			modelStack.Translate(go->pos.x, go->pos.y, go->pos.z);
			modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
			RenderMesh(meshList[GEO_OBJECTIVE], true);
			modelStack.PopMatrix();
		}
	}
	glEnable(GL_DEPTH_TEST);
}

void SceneSP3::mapEditorRender()
{
	modelStack.PushMatrix();
	modelStack.Translate(mapPosition.x, mapPosition.y, mapPosition.z - 9);
	modelStack.Scale(testMap.getMapSize().x, testMap.getMapSize().y, 1);
	RenderMesh(testMap.getBackground(), false);
	modelStack.PopMatrix();
	double x, y;
	Application::GetCursorPos(&x, &y);
	int w = Application::GetWindowWidth();
	int h = Application::GetWindowHeight();
	float worldX = x * m_worldWidth / w;
	float worldY = (h - y) * m_worldHeight / h;
	if (Application::IsKeyPressed(VK_LSHIFT) && testMode == false)
	{
		int posx = worldX;
		int posy = worldY;
		if ((int)(posx - mapPosition.x) % 10 > 5)
		{
			posx += 10 - ((int)(posx - mapPosition.x) % 10);
		}
		else
		{
			posx -= (int)(posx - mapPosition.x) % 10;
		}
		if ((int)(posy - mapPosition.y) % 10 > 5)
		{
			posy += 10 - ((int)(posy - mapPosition.y) % 10);
		}
		else
		{
			posy -= (int)(posy - mapPosition.y) % 10;
		}
		modelStack.PushMatrix();
		modelStack.Translate(posx, posy, -6);
		modelStack.Scale(55, 55, 1);
		RenderMesh(meshList[HUD_GRIDLOCK], false);
		modelStack.PopMatrix();
	}
	glDisable(GL_DEPTH_TEST);
	for (std::vector<GameObject *>::iterator it = testMap.mapProps.begin(); it != testMap.mapProps.end(); ++it)
	{
		GameObject *go = (GameObject *)*it;
		if (go->active == false)
			continue;
		if (abs(go->pos.x - player1->pos.x) > m_worldWidth || abs(go->pos.y - player1->pos.y) > m_worldHeight)
		{
			continue;
		}
		if (go->type == GameObject::MAP_TREE)
		{
			modelStack.PushMatrix();
			modelStack.Translate(go->pos.x, go->pos.y, go->pos.z + 2);
			modelStack.Scale(go->scale.x * 5, go->scale.y * 5, go->scale.z);
			RenderMesh(meshList[GEO_TREETOP], true);
			modelStack.PopMatrix();
		}
		else if (go->type == GameObject::MAP_ROCK)
		{
			glEnable(GL_DEPTH_TEST);
			modelStack.PushMatrix();
			modelStack.Translate(go->pos.x, go->pos.y, go->pos.z - 2);
			modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
			RenderMesh(meshList[GEO_ROCK], true);
			modelStack.PopMatrix();
			glDisable(GL_DEPTH_TEST);
		}
		else if (go->type == GameObject::MAP_WATER)
		{
			glEnable(GL_DEPTH_TEST);
			modelStack.PushMatrix();
			modelStack.Translate(go->pos.x, go->pos.y, go->pos.z - 2);
			modelStack.Rotate(90, 1, 0, 0);
			modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
			RenderMesh(meshList[GEO_ICE], true);
			modelStack.PopMatrix();
			glDisable(GL_DEPTH_TEST);
		}
		else if (go->type == GameObject::MAP_MUD)
		{
			glEnable(GL_DEPTH_TEST);
			modelStack.PushMatrix();
			modelStack.Translate(go->pos.x, go->pos.y, go->pos.z);
			modelStack.Rotate(90, 1, 0, 0);
			modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
			RenderMesh(meshList[GEO_MUD], true);
			modelStack.PopMatrix();
			glDisable(GL_DEPTH_TEST);
		}
		else if (go->type == GameObject::MAP_LION)
		{
			modelStack.PushMatrix();
			modelStack.Translate(go->pos.x, go->pos.y, go->pos.z);
			modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
			RenderMesh(meshList[GEO_LION], true);
			modelStack.PopMatrix();
		}
		else if (go->type == GameObject::MAP_ZEBRA)
		{
			modelStack.PushMatrix();
			modelStack.Translate(go->pos.x, go->pos.y, go->pos.z);
			modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
			RenderMesh(meshList[GEO_ZEBRA], true);
			modelStack.PopMatrix();
		}
		else if (go->type == GameObject::MAP_RHINO)
		{
			modelStack.PushMatrix();
			modelStack.Translate(go->pos.x, go->pos.y, go->pos.z);
			modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
			RenderMesh(meshList[GEO_RHINO], true);
			modelStack.PopMatrix();
		}
		else if (go->type == GameObject::MAP_HUMAN)
		{
			modelStack.PushMatrix();
			modelStack.Translate(go->pos.x, go->pos.y, go->pos.z);
			modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
			RenderMesh(meshList[GEO_HUMAN], true);
			modelStack.PopMatrix();
		}
		else if (go->type == GameObject::MAP_OBJECTIVE)
		{
			modelStack.PushMatrix();
			modelStack.Translate(go->pos.x, go->pos.y, go->pos.z);
			modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
			RenderMesh(meshList[GEO_OBJECTIVE], true);
			modelStack.PopMatrix();
		}
	}
	if (deleteMode > 0)
	{
		modelStack.PushMatrix();
		modelStack.Translate(worldX, worldY, 9);
		modelStack.Scale(3, 3, 1);
		RenderMesh(meshList[HUD_DELETEICON], false);
		modelStack.PopMatrix();
		if (Application::IsMousePressed(0) && deleteMode > 1)
		{
			renderSelection(newMouseX, newMouseY);
		}
	}
	glEnable(GL_DEPTH_TEST);
	if (testMode == false)
	{
		modelStack.PushMatrix();
		modelStack.Translate(m_worldWidth / 2, m_worldHeight / 2, -1);
		modelStack.Scale(m_worldWidth, m_worldHeight, 1);
		RenderMesh(meshList[HUD_MAPEDITOR3], false);
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		modelStack.Translate(m_worldWidth / 2, m_worldHeight / 2, 0);
		modelStack.Scale(m_worldWidth, m_worldHeight, 1);
		RenderMesh(meshList[HUD_MAPEDITOR], false);
		modelStack.PopMatrix();
		
		RenderTextOnScreen(meshList[GEO_TEXT], mapName, Color(0, 1, 0), 3, 39, 54);
	}
	else
	{
		modelStack.PushMatrix();
		modelStack.Translate(15, 10, 8);
		modelStack.Scale(20, 20, 1);
		RenderMesh(meshList[HUD_RADAR], false);
		modelStack.PopMatrix();
		modelStack.PushMatrix();
		modelStack.Translate(15, 10, 9);
		modelStack.Rotate(time, 0, 0, 1);
		modelStack.Scale(2.1f, 0.1f, 1);
		modelStack.Translate(2.f, 0, 0);
		RenderMesh(meshList[HUD_RADARLINE], false);
		modelStack.PopMatrix();
		renderMinimap(&testMap);
	}

	if (testMode == false)
	{
		modelStack.PushMatrix();
		modelStack.Translate(m_worldWidth / 2 + panel2x, m_worldHeight / 2, 9);
		modelStack.Scale(m_worldWidth, m_worldHeight, 1);
		RenderMesh(meshList[HUD_MAPEDITOR2], false);
		modelStack.PopMatrix();
	}
	else
	{
		modelStack.PushMatrix();
		modelStack.Translate(m_worldWidth / 2, 0.9f *  m_worldHeight, 9);
		modelStack.Scale(m_worldWidth / 5, m_worldHeight / 10, 1);
		RenderMesh(meshList[HUD_TESTMODE], false);
		modelStack.PopMatrix();
	}
}
void SceneSP3::renderMinimap(playMap* map)
{
	for (std::vector<GameObject *>::iterator it = testMap.mapProps.begin(); it != testMap.mapProps.end(); ++it)
	{
		GameObject *go = (GameObject *)*it;
		if (go->active == false)
			continue;
		if (go->dead == true)
			continue;
		if (go->radarVisible == false)
			continue;
		if ((go->pos - player1->pos).LengthSquared() < 14400)
		{
			int i = Math::RadianToDegree(atan2(go->pos.y - player1->pos.y, go->pos.x - player1->pos.x));
			if (i < 0)
			{
				i += 360;
			}
			//int j = (int)time % 360;
			//if (i > j && j < 100)
			//{
			//	j += 100;
			//	i -= 260;
			//}
			//
			//if (i < j && i > j - 100)
			int j = ((int)time % 360) + 360;
			if (i < j && j > 260)
			{
				j = 100 - (360 - j);
				i += 100;
			}

			if (i > j && i < j + 100)
			{
				//cout << Math::RadianToDegree(atan2(go->pos.y - player1->pos.y, go->pos.x - player1->pos.x)) << endl;
				modelStack.PushMatrix();
				modelStack.Translate((go->pos.x - player1->pos.x) / 15, (go->pos.y - player1->pos.y) / 15, 1);
				modelStack.Translate(15, 10, 8);
				modelStack.Scale(0.5f, 0.5f, 1);
				switch (go->type)
				{
				case GameObject::MAP_MUD:
				{
											modelStack.PushMatrix();
											modelStack.Rotate(90, 1, 0, 0);
											RenderMesh(meshList[GEO_MUD], false);
											modelStack.PopMatrix();
											break;
				}
				case GameObject::MAP_WATER:
				{
											  modelStack.PushMatrix();
											  modelStack.Rotate(90, 1, 0, 0);
											  RenderMesh(meshList[GEO_ICE], false);
											  modelStack.PopMatrix();
											break;
				}
				case GameObject::MAP_ROCK:
				{
											 RenderMesh(meshList[GEO_ROCK], false);
											 break;
				}
				case GameObject::MAP_TREE:
				{
											 modelStack.PushMatrix();
											 modelStack.Rotate(90, 1, 0, 0);
											 RenderMesh(meshList[HUD_RADARDETECT], false);
											 modelStack.PopMatrix();
											 //RenderMesh(meshList[GEO_TREETOP], false);
											 break;
				}
				case GameObject::MAP_LION:
				{
											 modelStack.PushMatrix();
											 modelStack.Rotate(90, 1, 0, 0);
											 RenderMesh(meshList[HUD_RADARENEMY], false);
											 modelStack.PopMatrix();
											// RenderMesh(meshList[GEO_LION], false);
											 break;
				}
				case GameObject::MAP_ZEBRA:
				{
											  modelStack.PushMatrix();
											  modelStack.Rotate(90, 1, 0, 0);
											  RenderMesh(meshList[HUD_RADARENEMY], false);
											  modelStack.PopMatrix();
											 //RenderMesh(meshList[GEO_ZEBRA], false);
											 break;
				}
				case GameObject::MAP_RHINO:
				{
											  modelStack.PushMatrix();
											  modelStack.Rotate(90, 1, 0, 0);
											  RenderMesh(meshList[HUD_RADARENEMY], false);
											  modelStack.PopMatrix();
											//RenderMesh(meshList[GEO_RHINO], false);
											 break;
				}
				case GameObject::MAP_HUMAN:
				{
											  modelStack.PushMatrix();
											  modelStack.Rotate(90, 1, 0, 0);
											  RenderMesh(meshList[HUD_RADARENEMY], false);
											  modelStack.PopMatrix();
											 //RenderMesh(meshList[GEO_HUMAN], false);
											 break;
				}
				default:
				{
						   RenderMesh(meshList[HUD_RADARDETECT], false);
						   break;
				}
				}
				modelStack.PopMatrix();
			}
			else
			{

			}
		}
	}
	for (std::vector<enemy*>::iterator it = enemyList.begin(); it != enemyList.end(); ++it)
	{
		enemy *go = (enemy *)*it;
		if (go->getActive() == false)
			continue;
		if ((go->getPos() - player1->pos).LengthSquared() < 14400)
		{
			int i = Math::RadianToDegree(atan2(go->getPos().y - player1->pos.y, go->getPos().x - player1->pos.x));
			if (i < 0)
			{
				i += 360;
			}
			int j = ((int)time % 360) + 360;
			if (i < j && j > 260)
			{
				j = 100 - (360 - j);
				i += 100;
			}

			if (i > j && i < j + 100)
			{
				//cout << Math::RadianToDegree(atan2(go->pos.y - player1->pos.y, go->pos.x - player1->pos.x)) << endl;
				modelStack.PushMatrix();
				modelStack.Translate((go->getPos().x - player1->pos.x) / 15, (go->getPos().y - player1->pos.y) / 15, 1);
				modelStack.Translate(15, 10, 8);
				modelStack.Scale(0.5f, 0.5f, 1);
					modelStack.PushMatrix();
					modelStack.Rotate(90, 1, 0, 0);
					RenderMesh(meshList[HUD_RADARENEMY], false);
					modelStack.PopMatrix();
				modelStack.PopMatrix();
			}
		}
	}
	for (std::vector<GameObject *>::iterator it = testMap.mapBorder.begin(); it != testMap.mapBorder.end(); ++it)
	{
		GameObject *go = (GameObject *)*it;
		if ((go->pos - player1->pos).LengthSquared() < 14400)
		{
			int i = Math::RadianToDegree(atan2(go->pos.y - player1->pos.y, go->pos.x - player1->pos.x));
			if (i < 0)
			{
				i += 360;
			}
			int j = ((int)time % 360) + 360;
	      if (i < j && j > 260)
	      {
	      	j = 100 - (360 - j);
	      	i += 100;
	      }
	      
	      if (i > j && i < j + 100)
			{
				modelStack.PushMatrix();
				modelStack.Translate((go->pos.x - player1->pos.x) / 15, (go->pos.y - player1->pos.y) / 15, 1);
				modelStack.Translate(15, 10, 8);
				modelStack.Scale(0.5f, 0.5f, 1);
					modelStack.PushMatrix();
					modelStack.Rotate(90, 1, 0, 0);
					RenderMesh(meshList[HUD_RADARDETECT], false);
					modelStack.PopMatrix();
				modelStack.PopMatrix();
			}
		}
	}
}

void SceneSP3::renderSelection(float x1, float y1)
{
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	double x, y;
	Application::GetCursorPos(&x, &y);
	int w = Application::GetWindowWidth();
	int h = Application::GetWindowHeight();
	float worldX = x * m_worldWidth / w;
	float worldY = (h - y) * m_worldHeight / h;
	modelStack.PushMatrix();
	modelStack.Translate((x1 + worldX) / 2.f, (y1 + worldY) / 2.f, 3);
	modelStack.Scale((x1 - worldX), (y1 - worldY), 1);
	RenderMesh(meshList[HUD_SELECTION], true);
	modelStack.PopMatrix();
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}
bool SceneSP3::RenderMapFile()
{
	int row = 0;
	TextFile* map = new TextFile(TextFile::MAP);
	row = map->getRows(NameofMap);
	cout << row << endl;
	for (int i = 1; i < row + 1; ++i)
	{
		map->LoadMap(NameofMap, i);

		GameObject* obs;
		string typeofobject = map->get_type();

		if (map->get_type() == "tree")
		{
			obs = new GameObject(GameObject::MAP_TREE);
			obs->pos.Set(map->get_x(), map->get_y(), 1);
			cout << obs->pos << endl;

			obs->fresh = true;
			obs->active = true;
			obs->scale.Set(1, 1, 1);
			testMap.forceAddSingleProp(obs);

			cout << "tree added at: " << obs->pos.x << "," << obs->pos.y << endl;
			cout << "map->get_y() = " << map->get_y() << "," << "mapPosition.y = " << mapPosition.y << endl;
		}
		else if (map->get_type() == "rock")
		{
			obs = new GameObject(GameObject::MAP_ROCK);
			obs->pos.Set(map->get_x(), map->get_y(), 1);
			obs->fresh = true;
			obs->active = true;
			obs->scale.Set(6,6,6);
			testMap.forceAddSingleProp(obs);

			cout << "rock added" << endl;
		}
		else if (map->get_type() == "water")
		{
			obs = new GameObject(GameObject::MAP_WATER);
			obs->pos.Set(map->get_x(), map->get_y(), 1);
			obs->fresh = true;
			obs->active = true;
			obs->scale.Set(6,6,6);
			testMap.forceAddSingleProp(obs);

			cout << "water added" << endl;
		}
		else if (map->get_type() == "mud")
		{
			obs = new GameObject(GameObject::MAP_MUD);
			obs->pos.Set(map->get_x(), map->get_y(), 1);
			obs->fresh = true;
			obs->active = true;
			obs->scale.Set(6,6,6);
			testMap.forceAddSingleProp(obs);

			cout << "mud added" << endl;
		}
		else if (map->get_type() == "lion")
		{
			obs = new GameObject(GameObject::MAP_LION);
			obs->pos.Set(map->get_x(), map->get_y(), 1);
			obs->fresh = true;
			obs->active = true;
			obs->scale.Set(6,6,6);
			testMap.forceAddSingleProp(obs);

			cout << "lion added" << endl;
		}
		else if (map->get_type() == "rhino")
		{
			obs = new GameObject(GameObject::MAP_RHINO);
			obs->pos.Set(map->get_x(), map->get_y(), 1);
			obs->fresh = true;
			obs->active = true;
			obs->scale.Set(6,6,6);
			testMap.forceAddSingleProp(obs);

			cout << "rhino added" << endl;
		}
		else if (map->get_type() == "zebra")
		{
			obs = new GameObject(GameObject::MAP_ZEBRA);
			obs->pos.Set(map->get_x(), map->get_y(), 1);
			obs->fresh = true;
			obs->active = true;
			obs->scale.Set(6,6,6);
			testMap.forceAddSingleProp(obs);

			cout << "zebra added" << endl;
		}
		else if (map->get_type() == "human")
		{
			obs = new GameObject(GameObject::MAP_HUMAN);
			obs->pos.Set(map->get_x(), map->get_y(), 1);
			obs->fresh = true;
			obs->active = true;
			obs->scale.Set(6,6,6);
			testMap.forceAddSingleProp(obs);

			cout << "human added" << endl;
		}\
		else if (map->get_type() == "objective")
		{
			obs = new GameObject(GameObject::MAP_OBJECTIVE);
			obs->pos.Set(map->get_x(), map->get_y(), 1);
			obs->fresh = true;
			obs->active = true;
			obs->scale.Set(5, 5, 5);
			testMap.forceAddSingleProp(obs);

			cout << "human added" << endl;
		}
	}
	return true;
}
void SceneSP3::renderMenu()
{

	if (gameStates == states::s_Menu)
	{
		modelStack.PushMatrix();
		modelStack.Translate(m_worldWidth / 2, m_worldHeight / 2, -4);
		modelStack.Scale(m_worldWidth, m_worldHeight, 0);
		RenderMesh(meshList[GEO_MENU_BACKGROUND], false);
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		modelStack.Translate(m_worldWidth / 1.1, m_worldHeight / 1.1, -3);
		modelStack.Scale(70, 70, 0);
		RenderMesh(meshList[GEO_MENU_MAPEDITOR], false);
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		modelStack.Translate(m_worldWidth / 1.25, m_worldHeight / 6.5, -2);
		modelStack.Scale(33, 33, 0);
		RenderMesh(meshList[GEO_MENU_SIGNBOARD], false);
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		modelStack.Translate(m_worldWidth / 1.5, 38 - (arrowSelection * 7), -1);
		modelStack.Scale(3, 3, 0);
		RenderMesh(meshList[GEO_MENU_ARROW], false);
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		modelStack.Translate(m_worldWidth / 1.25, m_worldHeight / 2.8, 0);
		modelStack.Scale(30, 20, 0);
		RenderMesh(meshList[GEO_MENU_STARTGAME], false);
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		modelStack.Translate(m_worldWidth / 1.25, m_worldHeight / 3.4, 1);
		modelStack.Scale(30, 20, 0);
		RenderMesh(meshList[GEO_MENU_INSTRUCTIONS], false);
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		modelStack.Translate(m_worldWidth / 1.25, m_worldHeight / 4.5, 2);
		modelStack.Scale(30, 20, 0);
		RenderMesh(meshList[GEO_MENU_OPTIONS], false);
		modelStack.PopMatrix();


		modelStack.PushMatrix();
		modelStack.Translate(m_worldWidth / 1.25, m_worldHeight / 6.4, 3);
		modelStack.Scale(30, 20, 0);
		RenderMesh(meshList[GEO_MENU_HIGHSCORE], false);
		modelStack.PopMatrix();


		modelStack.PushMatrix();
		modelStack.Translate(m_worldWidth / 1.25, m_worldHeight / 11, 4);
		modelStack.Scale(30, 20, 0);
		RenderMesh(meshList[GEO_MENU_QUIT], false);
		modelStack.PopMatrix();
	}
	if (gameStates == states::s_LevelSelect)
	{
		modelStack.PushMatrix();
		modelStack.Translate(m_worldWidth / 2, m_worldHeight / 2, -3);
		modelStack.Scale(m_worldWidth, m_worldHeight, 0);
		RenderMesh(meshList[GEO_LEVELSELECT], false);
		modelStack.PopMatrix();

	}
	if (gameStates == states::s_CustomLevelSelect)
	{
		modelStack.PushMatrix();
		modelStack.Translate(m_worldWidth / 2, m_worldHeight / 2, -3);
		modelStack.Scale(m_worldWidth, m_worldHeight, 0);
		RenderMesh(meshList[GEO_LEVELSELECT_CUSTOM], false);
		modelStack.PopMatrix();

	}
	if (gameStates == states::s_Instructions)
	{
		modelStack.PushMatrix();
		//modelStack.Translate(140, 58, 4);
		//modelStack.Scale(50, 20, 0);
		modelStack.Translate(m_worldWidth / 2, m_worldHeight / 2, 0);
		modelStack.Scale(m_worldWidth, m_worldHeight, 1);
		RenderMesh(meshList[GEO_MENU_INSTRUCTIONS_PAGE], false);
		modelStack.PopMatrix();

		if (Application::IsKeyPressed(VK_ESCAPE))
		{
			gameStates = states::s_Menu;
		}
	}
	else if (gameStates == states::s_Options)
	{
		modelStack.PushMatrix();
		modelStack.Translate(m_worldWidth / 2, m_worldHeight / 2, -4);
		modelStack.Scale(m_worldWidth / 2, m_worldHeight / 2, 0);
		RenderMesh(meshList[GEO_OPTIONS], false);
		modelStack.PopMatrix();

		if (sound == 1)
		{
			std::ostringstream sso1;
			sso1.precision(5);
			sso1 << "On";
			RenderTextOnScreen(meshList[GEO_TEXT], sso1.str(), Color(0, 1, 0), 5, 45, 43);
		}
		else
		{
			std::ostringstream sso1;
			sso1.precision(5);
			sso1 << "Off";
			RenderTextOnScreen(meshList[GEO_TEXT], sso1.str(), Color(1, 0, 0), 5, 45, 43);
		}
		if (highQ == 1)
		{
			std::ostringstream sso2;
			sso2.precision(5);
			sso2 << "On";
			RenderTextOnScreen(meshList[GEO_TEXT], sso2.str(), Color(0, 1, 0), 5, 45, 31.5);
		}
		else
		{
			std::ostringstream sso2;
			sso2.precision(5);
			sso2 << "Off";
			RenderTextOnScreen(meshList[GEO_TEXT], sso2.str(), Color(1, 0, 0), 5, 45, 31.5);
		}
	}
	if (gameStates == states::s_Upgrade)
	{
		modelStack.PushMatrix();
		modelStack.Translate(m_worldWidth / 2, m_worldHeight / 2, 0);
		modelStack.Scale(m_worldWidth, m_worldHeight, 1);
		RenderMesh(meshList[GEO_UPGRADE_BACKGROUND], false);
		modelStack.PopMatrix();

		std::ostringstream s13;
		s13.precision(5);
		s13 << "$" << money;
		RenderTextOnScreen(meshList[GEO_TEXT], s13.str(), Color(0, 1, 0), 2.5, 59.5, 1);
	}
	if (gameStates == states::s_Upgrade_Cars1 || gameStates == states::s_Upgrade_Cars2 || gameStates == states::s_Upgrade_Cars3)
	{
		modelStack.PushMatrix();
		modelStack.Translate(m_worldWidth / 2, m_worldHeight / 2, -7);
		modelStack.Scale(m_worldWidth, m_worldHeight, 1);
		RenderMesh(meshList[GEO_UPGRADE_UI_CARS], false);
		modelStack.PopMatrix();

		if (car1Bought == true)
		{
			modelStack.PushMatrix();
			modelStack.Translate(m_worldWidth / 8, m_worldHeight / 5, -4);
			modelStack.Scale(30, 30, 1);
			RenderMesh(meshList[GEO_SOLD], false);
			modelStack.PopMatrix();
		}

		if (car2Bought == true)
		{
			modelStack.PushMatrix();
			modelStack.Translate(m_worldWidth / 3.2, m_worldHeight / 5, -4);
			modelStack.Scale(30, 30, 1);
			RenderMesh(meshList[GEO_SOLD], false);
			modelStack.PopMatrix();


		}
		if (car3Bought == true)
		{
			modelStack.PushMatrix();
			modelStack.Translate(m_worldWidth / 2.1, m_worldHeight / 5, -4);
			modelStack.Scale(30, 30, 1);
			RenderMesh(meshList[GEO_SOLD], false);
			modelStack.PopMatrix();
		}

		if (Application::IsKeyPressed(VK_ESCAPE))
		{
			gameStates = states::s_Upgrade;
		}
	}
	if (gameStates == states::s_Upgrade_Tires1 || gameStates == states::s_Upgrade_Tires2 || gameStates == states::s_Upgrade_Tires3)
	{
		modelStack.PushMatrix();
		modelStack.Translate(m_worldWidth / 2, m_worldHeight / 2, -5);
		modelStack.Scale(m_worldWidth, m_worldHeight, 1);
		RenderMesh(meshList[GEO_UPGRADE_UI_TIRES], false);
		modelStack.PopMatrix();

		if (tire1Bought == true)
		{
			modelStack.PushMatrix();
			modelStack.Translate(m_worldWidth / 8, m_worldHeight / 5, -4);
			modelStack.Scale(30, 30, 1);
			RenderMesh(meshList[GEO_SOLD], false);
			modelStack.PopMatrix();
		}
		if (tire2Bought == true)
		{
			modelStack.PushMatrix();
			modelStack.Translate(m_worldWidth / 3.2, m_worldHeight / 5, -4);
			modelStack.Scale(30, 30, 1);
			RenderMesh(meshList[GEO_SOLD], false);
			modelStack.PopMatrix();
		}
		if (tire3Bought == true)
		{
			modelStack.PushMatrix();
			modelStack.Translate(m_worldWidth / 2.1, m_worldHeight / 5, -4);
			modelStack.Scale(30, 30, 1);
			RenderMesh(meshList[GEO_SOLD], false);
			modelStack.PopMatrix();
		}
		if (Application::IsKeyPressed(VK_ESCAPE))
		{
			gameStates = states::s_Upgrade;
		}
	}
	if (gameStates == states::s_Upgrade_Lasso1 || gameStates == states::s_Upgrade_Lasso2 || gameStates == states::s_Upgrade_Lasso3)
	{
		modelStack.PushMatrix();
		modelStack.Translate(m_worldWidth / 2, m_worldHeight / 2, -5);
		modelStack.Scale(m_worldWidth, m_worldHeight, 1);
		RenderMesh(meshList[GEO_UPGRADE_UI_LASSO], false);
		modelStack.PopMatrix();

		if (lasso1Bought == true)
		{
			modelStack.PushMatrix();
			modelStack.Translate(m_worldWidth / 8, m_worldHeight / 5, -4);
			modelStack.Scale(30, 30, 1);
			RenderMesh(meshList[GEO_SOLD], false);
			modelStack.PopMatrix();
		}

		if (lasso2Bought == true)
		{

			if (gameStates == states::s_Upgrade)

			{
				modelStack.PushMatrix();
				modelStack.Translate(m_worldWidth / 3.2, m_worldHeight / 5, -4);
				modelStack.Scale(30, 30, 1);
				RenderMesh(meshList[GEO_SOLD], false);
				modelStack.PopMatrix();

				std::ostringstream s13;
				s13.precision(5);
				s13 << "$" << money;
				RenderTextOnScreen(meshList[GEO_TEXT], s13.str(), Color(0, 1, 0), 2.5, 59.5, 1);
			}
			if (lasso3Bought == true)
			{
				modelStack.PushMatrix();
				modelStack.Translate(m_worldWidth / 2.1, m_worldHeight / 5, -4);
				modelStack.Scale(30, 30, 1);
				RenderMesh(meshList[GEO_SOLD], false);
				modelStack.PopMatrix();
			}

		}
		if (Application::IsKeyPressed(VK_ESCAPE))
		{
			gameStates = states::s_Upgrade;
		}
	}
	if (gameStates == states::s_Upgrade_Darts1 || gameStates == states::s_Upgrade_Darts2 || gameStates == states::s_Upgrade_Darts3)
	{
		modelStack.PushMatrix();
		modelStack.Translate(m_worldWidth / 2, m_worldHeight / 2, -5);
		modelStack.Scale(m_worldWidth, m_worldHeight, 1);
		RenderMesh(meshList[GEO_UPGRADE_UI_DARTS], false);
		modelStack.PopMatrix();

		if (dart1Bought == true)
		{
			modelStack.PushMatrix();
			modelStack.Translate(m_worldWidth / 8, m_worldHeight / 5, -4);
			modelStack.Scale(30, 30, 1);
			RenderMesh(meshList[GEO_SOLD], false);
			modelStack.PopMatrix();
		}

		if (dart2Bought == true)
		{
			modelStack.PushMatrix();
			modelStack.Translate(m_worldWidth / 3.2, m_worldHeight / 5, -4);
			modelStack.Scale(30, 30, 1);
			RenderMesh(meshList[GEO_SOLD], false);
			modelStack.PopMatrix();
		}

		if (dart3Bought == true)
		{
			modelStack.PushMatrix();
			modelStack.Translate(m_worldWidth / 2.1, m_worldHeight / 5, -4);
			modelStack.Scale(30, 30, 1);
			RenderMesh(meshList[GEO_SOLD], false);
			modelStack.PopMatrix();
		}

		if (Application::IsKeyPressed(VK_ESCAPE))
		{
			gameStates = states::s_Upgrade;
		}
	}

	if (gameStates == states::s_Upgrade_Cars1)
	{
		InitCarStat("Car1","SavedCarStats.txt");

		modelStack.PushMatrix();
		modelStack.Translate(m_worldWidth * 0.790, m_worldHeight * 0.400, 2);
		modelStack.Rotate(rotateDisplayY + 40, 1, 0, 0);
		modelStack.Rotate(rotateDisplayX - 45, 0, 1, 0);
		modelStack.Scale(10, 10, 10);
		RenderMesh(meshList[GEO_DISPLAY_CAR1], false);
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		modelStack.Translate(m_worldWidth * 0.790, m_worldHeight * 0.200, 2);
		modelStack.Scale(30, 30, 1);
		RenderMesh(meshList[GEO_BUY], false);
		modelStack.PopMatrix();

		std::ostringstream s10;
		s10.precision(5);
		s10 << "ENGINE";
		RenderTextOnScreen(meshList[GEO_TEXT], s10.str(), Color(0, 1, 0), 2.5, 4.5, 45);

		for (int i = 0; i < player1->playerCar.engine * 5; i += 5)
		{
			modelStack.PushMatrix();
			modelStack.Translate(m_worldWidth * 0.072 + i, m_worldHeight * 0.7, 2);
			modelStack.Scale(1, 1, 1);
			RenderMesh(meshList[GEO_CUBE], false);
			modelStack.PopMatrix();
		}

		std::ostringstream s11;
		s11.precision(5);
		s11 << "HP";
		RenderTextOnScreen(meshList[GEO_TEXT], s11.str(), Color(0, 1, 0), 2.5, 4.5, 35);

		for (int i = 0; i < player1->playerCar.hp * 5; i += 5)
		{
			modelStack.PushMatrix();
			modelStack.Translate(m_worldWidth * 0.072 + i, m_worldHeight * 0.55, 2);
			modelStack.Scale(1, 1, 1);
			RenderMesh(meshList[GEO_CUBE], false);
			modelStack.PopMatrix();
		}
		std::ostringstream s12;
		s12.precision(5);
		s12 << "FREE";
		RenderTextOnScreen(meshList[GEO_TEXT], s12.str(), Color(0, 1, 0), 2.5, 14.5, 1);

		std::ostringstream s13;
		s13.precision(5);
		s13 << "$" << money;
		RenderTextOnScreen(meshList[GEO_TEXT], s13.str(), Color(0, 1, 0), 2.5, 59.5, 1);
	}
	if (gameStates == states::s_Upgrade_Cars2)
	{
		InitCarStat("Car2", "SavedCarStats.txt");

		modelStack.PushMatrix();
		modelStack.Translate(m_worldWidth * 0.790, m_worldHeight * 0.400, 2);
		modelStack.Rotate(rotateDisplayY + 40, 1, 0, 0);
		modelStack.Rotate(rotateDisplayX + 45, 0, 1, 0);
		modelStack.Scale(15, 15, 15);
		RenderMesh(meshList[GEO_DISPLAY_CAR2], false);
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		modelStack.Translate(m_worldWidth * 0.790, m_worldHeight * 0.200, 2);
		modelStack.Scale(30, 30, 1);
		RenderMesh(meshList[GEO_BUY], false);
		modelStack.PopMatrix();

		std::ostringstream s10;
		s10.precision(5);
		s10 << "ENGINE";
		RenderTextOnScreen(meshList[GEO_TEXT], s10.str(), Color(0, 1, 0), 2.5, 4.5, 45);

		for (int i = 0; i < player1->playerCar.engine * 5; i += 5)
		{
			modelStack.PushMatrix();
			modelStack.Translate(m_worldWidth * 0.072 + i, m_worldHeight * 0.7, 2);
			modelStack.Scale(1, 1, 1);
			RenderMesh(meshList[GEO_CUBE], false);
			modelStack.PopMatrix();
		}

		std::ostringstream s11;
		s11.precision(5);
		s11 << "HP";
		RenderTextOnScreen(meshList[GEO_TEXT], s11.str(), Color(0, 1, 0), 2.5, 4.5, 35);

		for (int i = 0; i < player1->playerCar.hp * 5; i += 5)
		{
			modelStack.PushMatrix();
			modelStack.Translate(m_worldWidth * 0.072 + i, m_worldHeight * 0.55, 2);
			modelStack.Scale(1, 1, 1);
			RenderMesh(meshList[GEO_CUBE], false);
			modelStack.PopMatrix();
		}
		std::ostringstream s12;
		s12.precision(5);
		s12 << "$" << cost[0];
		RenderTextOnScreen(meshList[GEO_TEXT], s12.str(), Color(0, 0, 1), 2.5, 14.5, 1);

		std::ostringstream s13;
		s13.precision(5);
		s13 << "$" << money;
		RenderTextOnScreen(meshList[GEO_TEXT], s13.str(), Color(0, 1, 0), 2.5, 59.5, 1);

	}
	if (gameStates == states::s_Upgrade_Cars3)
	{
		InitCarStat("Car3", "SavedCarStats.txt");

		modelStack.PushMatrix();
		modelStack.Translate(m_worldWidth * 0.790, m_worldHeight * 0.400, 2);
		modelStack.Rotate(rotateDisplayY + 40, 1, 0, 0);
		modelStack.Rotate(rotateDisplayX + 45, 0, 1, 0);
		modelStack.Scale(15, 15, 15);
		RenderMesh(meshList[GEO_DISPLAY_CAR3], false);
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		modelStack.Translate(m_worldWidth * 0.790, m_worldHeight * 0.200, 2);
		modelStack.Scale(30, 30, 1);
		RenderMesh(meshList[GEO_BUY], false);
		modelStack.PopMatrix();

		std::ostringstream s10;
		s10.precision(5);
		s10 << "ENGINE";
		RenderTextOnScreen(meshList[GEO_TEXT], s10.str(), Color(0, 1, 0), 2.5, 4.5, 45);

		for (int i = 0; i < player1->playerCar.engine * 5; i += 5)
		{
			modelStack.PushMatrix();
			modelStack.Translate(m_worldWidth * 0.072 + i, m_worldHeight * 0.7, 2);
			modelStack.Scale(1, 1, 1);
			RenderMesh(meshList[GEO_CUBE], false);
			modelStack.PopMatrix();
		}

		std::ostringstream s11;
		s11.precision(5);
		s11 << "HP";
		RenderTextOnScreen(meshList[GEO_TEXT], s11.str(), Color(0, 1, 0), 2.5, 4.5, 35);

		for (int i = 0; i < player1->playerCar.hp * 5; i += 5)
		{
			modelStack.PushMatrix();
			modelStack.Translate(m_worldWidth * 0.072 + i, m_worldHeight * 0.55, 2);
			modelStack.Scale(1, 1, 1);
			RenderMesh(meshList[GEO_CUBE], false);
			modelStack.PopMatrix();
		}
		std::ostringstream s12;
		s12.precision(5);
		s12 << "$" << cost[1];
		RenderTextOnScreen(meshList[GEO_TEXT], s12.str(), Color(0, 0, 1), 2.5, 14.5, 1);

		std::ostringstream s13;
		s13.precision(5);
		s13 << "$" << money;
		RenderTextOnScreen(meshList[GEO_TEXT], s13.str(), Color(0, 1, 0), 2.5, 59.5, 1);
	}
	if (gameStates == states::s_Upgrade_Tires1)
	{
		/*InitCarStat("Car1");*/

		modelStack.PushMatrix();
		modelStack.Translate(m_worldWidth * 0.790, m_worldHeight * 0.500, 2);
		modelStack.Rotate(rotateDisplayY, 1, 0, 0);
		modelStack.Rotate(rotateDisplayX - 45, 0, 1, 0);
		modelStack.Scale(1.5, 1.5, 1.5);
		RenderMesh(meshList[GEO_DISPLAY_WHEEL], false);
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		modelStack.Translate(m_worldWidth * 0.790, m_worldHeight * 0.200, 2);
		modelStack.Scale(30, 30, 1);
		RenderMesh(meshList[GEO_BUY], false);
		modelStack.PopMatrix();

		TextFile* aa = new TextFile();
		aa->GetCarStat("Car1", "CarStats.txt");
		InitCarStat("Car1", "SavedCarStats.txt");

		std::ostringstream s10;
		s10.precision(5);
		s10 << "HANDLING";
		RenderTextOnScreen(meshList[GEO_TEXT], s10.str(), Color(0, 1, 0), 2, 5, 45);
		for (int i = 0; i < player1->playerCar.handling * 5; i += 5)
		{
			modelStack.PushMatrix();
			modelStack.Translate(m_worldWidth * 0.072 + i, m_worldHeight * 0.7, 2);
			modelStack.Scale(1, 1, 1);
			RenderMesh(meshList[GEO_CUBE], false);
			modelStack.PopMatrix();
		}
		std::ostringstream s12;
		s12.precision(5);
		s12 << "FREE";
		RenderTextOnScreen(meshList[GEO_TEXT], s12.str(), Color(0, 1, 0), 2.5, 14.5, 1);

		std::ostringstream s13;
		s13.precision(5);
		s13 << "$" << money;
		RenderTextOnScreen(meshList[GEO_TEXT], s13.str(), Color(0, 1, 0), 2.5, 59.5, 1);
	}
	if (gameStates == states::s_Upgrade_Tires2)
	{

		modelStack.PushMatrix();
		modelStack.Translate(m_worldWidth * 0.790, m_worldHeight * 0.500, 2);
		modelStack.Rotate(rotateDisplayY, 1, 0, 0);
		modelStack.Rotate(rotateDisplayX - 45, 0, 1, 0);
		modelStack.Scale(1.5, 1.5, 1.5);
		RenderMesh(meshList[GEO_DISPLAY_WHEEL], false);
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		modelStack.Translate(m_worldWidth * 0.790, m_worldHeight * 0.200, 2);
		modelStack.Scale(30, 30, 1);
		RenderMesh(meshList[GEO_BUY], false);
		modelStack.PopMatrix();

		TextFile* aa = new TextFile();
		aa->GetCarStat("Car2", "CarStats.txt");
		InitCarStat("Car2", "SavedCarStats.txt");

		std::ostringstream s10;
		s10.precision(5);
		s10 << "HANDLING";
		RenderTextOnScreen(meshList[GEO_TEXT], s10.str(), Color(0, 1, 0), 2, 5, 45);
		for (int i = 0; i < player1->playerCar.handling * 5; i += 5)
		{
			modelStack.PushMatrix();
			modelStack.Translate(m_worldWidth * 0.072 + i, m_worldHeight * 0.7, 2);
			modelStack.Scale(1, 1, 1);
			RenderMesh(meshList[GEO_CUBE], false);
			modelStack.PopMatrix();
		}
		std::ostringstream s12;
		s12.precision(5);
		s12 << "$" << cost[2];
		RenderTextOnScreen(meshList[GEO_TEXT], s12.str(), Color(0, 0, 1), 2.5, 14.5, 1);

		std::ostringstream s13;
		s13.precision(5);
		s13 << "$" << money;
		RenderTextOnScreen(meshList[GEO_TEXT], s13.str(), Color(0, 1, 0), 2.5, 59.5, 1);
	}
	if (gameStates == states::s_Upgrade_Tires3)
	{

		modelStack.PushMatrix();
		modelStack.Translate(m_worldWidth * 0.790, m_worldHeight * 0.500, 2);
		modelStack.Rotate(rotateDisplayY, 1, 0, 0);
		modelStack.Rotate(rotateDisplayX - 45, 0, 1, 0);
		modelStack.Scale(1.5, 1.5, 1.5);
		RenderMesh(meshList[GEO_DISPLAY_WHEEL], false);
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		modelStack.Translate(m_worldWidth * 0.790, m_worldHeight * 0.200, 2);
		modelStack.Scale(30, 30, 1);
		RenderMesh(meshList[GEO_BUY], false);
		modelStack.PopMatrix();

		TextFile* aa = new TextFile();
		aa->GetCarStat("Car3", "CarStats.txt");
		InitCarStat("Car3", "SavedCarStats.txt");

		std::ostringstream s10;
		s10.precision(5);
		s10 << "HANDLING";
		RenderTextOnScreen(meshList[GEO_TEXT], s10.str(), Color(0, 1, 0), 2, 5, 45);
		for (int i = 0; i < player1->playerCar.handling * 5; i += 5)
		{
			modelStack.PushMatrix();
			modelStack.Translate(m_worldWidth * 0.072 + i, m_worldHeight * 0.7, 2);
			modelStack.Scale(1, 1, 1);
			RenderMesh(meshList[GEO_CUBE], false);
			modelStack.PopMatrix();
		}
		std::ostringstream s12;
		s12.precision(5);
		s12 << "$" << cost[3];
		RenderTextOnScreen(meshList[GEO_TEXT], s12.str(), Color(0, 0, 1), 2.5, 14.5, 1);

		std::ostringstream s13;
		s13.precision(5);
		s13 << "$" << money;
		RenderTextOnScreen(meshList[GEO_TEXT], s13.str(), Color(0, 1, 0), 2.5, 59.5, 1);
	}
	if (gameStates == states::s_Upgrade_Lasso1)
	{

		modelStack.PushMatrix();
		modelStack.Translate(m_worldWidth * 0.790, m_worldHeight * 0.200, 2);
		modelStack.Scale(30, 30, 1);
		RenderMesh(meshList[GEO_BUY], false);
		modelStack.PopMatrix();

		TextFile* aa = new TextFile();
		aa->GetCarStat("Car1", "CarStats.txt");
		InitCarStat("Car1", "SavedCarStats.txt");

		std::ostringstream s10;
		s10.precision(5);
		s10 << "LENGTH";
		RenderTextOnScreen(meshList[GEO_TEXT], s10.str(), Color(0, 1, 0), 2.5, 5, 45);
		for (int i = 0; i < player1->playerCar.lassoLength * 5; i += 5)
		{
			modelStack.PushMatrix();
			modelStack.Translate(m_worldWidth * 0.072 + i, m_worldHeight * 0.7, 2);
			modelStack.Scale(1, 1, 1);
			RenderMesh(meshList[GEO_CUBE], false);
			modelStack.PopMatrix();
		}

		std::ostringstream s11;
		s11.precision(5);
		s11 << "SPEED";
		RenderTextOnScreen(meshList[GEO_TEXT], s11.str(), Color(0, 1, 0), 2.5, 4.5, 35);

		for (int i = 0; i < player1->playerCar.lassoSpeed * 5; i += 5)
		{
			modelStack.PushMatrix();
			modelStack.Translate(m_worldWidth * 0.072 + i, m_worldHeight * 0.55, 2);
			modelStack.Scale(1, 1, 1);
			RenderMesh(meshList[GEO_CUBE], false);
			modelStack.PopMatrix();
		}
		std::ostringstream s12;
		s12.precision(5);
		s12 << "FREE";
		RenderTextOnScreen(meshList[GEO_TEXT], s12.str(), Color(0, 1, 0), 2.5, 14.5, 1);

		std::ostringstream s13;
		s13.precision(5);
		s13 << "$" << money;
		RenderTextOnScreen(meshList[GEO_TEXT], s13.str(), Color(0, 1, 0), 2.5, 59.5, 1);
	}
	if (gameStates == states::s_Upgrade_Lasso2)
	{

		modelStack.PushMatrix();
		modelStack.Translate(m_worldWidth * 0.790, m_worldHeight * 0.200, 2);
		modelStack.Scale(30, 30, 1);
		RenderMesh(meshList[GEO_BUY], false);
		modelStack.PopMatrix();

		TextFile* aa = new TextFile();
		aa->GetCarStat("Car2", "CarStats.txt");
		InitCarStat("Car2", "SavedCarStats.txt");

		std::ostringstream s10;
		s10.precision(5);
		s10 << "LENGTH";
		RenderTextOnScreen(meshList[GEO_TEXT], s10.str(), Color(0, 1, 0), 2.5, 5, 45);
		for (int i = 0; i < player1->playerCar.lassoLength * 5; i += 5)
		{
			modelStack.PushMatrix();
			modelStack.Translate(m_worldWidth * 0.072 + i, m_worldHeight * 0.7, 2);
			modelStack.Scale(1, 1, 1);
			RenderMesh(meshList[GEO_CUBE], false);
			modelStack.PopMatrix();
		}

		std::ostringstream s11;
		s11.precision(5);
		s11 << "SPEED";
		RenderTextOnScreen(meshList[GEO_TEXT], s11.str(), Color(0, 1, 0), 2.5, 4.5, 35);

		for (int i = 0; i < player1->playerCar.lassoSpeed * 5; i += 5)
		{
			modelStack.PushMatrix();
			modelStack.Translate(m_worldWidth * 0.072 + i, m_worldHeight * 0.55, 2);
			modelStack.Scale(1, 1, 1);
			RenderMesh(meshList[GEO_CUBE], false);
			modelStack.PopMatrix();
		}
		std::ostringstream s12;
		s12.precision(5);
		s12 << "$" << cost[5];
		RenderTextOnScreen(meshList[GEO_TEXT], s12.str(), Color(0, 0, 1), 2.5, 14.5, 1);

		std::ostringstream s13;
		s13.precision(5);
		s13 << "$" << money;
		RenderTextOnScreen(meshList[GEO_TEXT], s13.str(), Color(0, 1, 0), 2.5, 59.5, 1);
	}
	if (gameStates == states::s_Upgrade_Lasso3)
	{
		modelStack.PushMatrix();
		modelStack.Translate(m_worldWidth * 0.790, m_worldHeight * 0.200, 2);
		modelStack.Scale(30, 30, 1);
		RenderMesh(meshList[GEO_BUY], false);
		modelStack.PopMatrix();

		TextFile* aa = new TextFile();
		aa->GetCarStat("Car3", "CarStats.txt");
		InitCarStat("Car3", "SavedCarStats.txt");

		std::ostringstream s10;
		s10.precision(5);
		s10 << "LENGTH";
		RenderTextOnScreen(meshList[GEO_TEXT], s10.str(), Color(0, 1, 0), 2.5, 5, 45);
		for (int i = 0; i < player1->playerCar.lassoLength * 5; i += 5)
		{
			modelStack.PushMatrix();
			modelStack.Translate(m_worldWidth * 0.072 + i, m_worldHeight * 0.7, 2);
			modelStack.Scale(1, 1, 1);
			RenderMesh(meshList[GEO_CUBE], false);
			modelStack.PopMatrix();
		}

		std::ostringstream s11;
		s11.precision(5);
		s11 << "SPEED";
		RenderTextOnScreen(meshList[GEO_TEXT], s11.str(), Color(0, 1, 0), 2.5, 4.5, 35);

		for (int i = 0; i < player1->playerCar.lassoSpeed * 5; i += 5)
		{
			modelStack.PushMatrix();
			modelStack.Translate(m_worldWidth * 0.072 + i, m_worldHeight * 0.55, 2);
			modelStack.Scale(1, 1, 1);
			RenderMesh(meshList[GEO_CUBE], false);
			modelStack.PopMatrix();
		}
		std::ostringstream s12;
		s12.precision(5);
		s12 << "$" << cost[6];
		RenderTextOnScreen(meshList[GEO_TEXT], s12.str(), Color(0, 0, 1), 2.5, 14.5, 1);

		std::ostringstream s13;
		s13.precision(5);
		s13 << "$" << money;
		RenderTextOnScreen(meshList[GEO_TEXT], s13.str(), Color(0, 1, 0), 2.5, 59.5, 1);
	}
	if (gameStates == states::s_Upgrade_Darts1)
	{

		modelStack.PushMatrix();
		modelStack.Translate(m_worldWidth * 0.790, m_worldHeight * 0.500, 2);
		modelStack.Rotate(rotateDisplayY, 1, 0, 0);
		modelStack.Rotate(rotateDisplayX + 90, 0, 1, 0);
		modelStack.Scale(5, 5, 5);
		RenderMesh(meshList[GEO_DISPLAY_DARTS], false);
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		modelStack.Translate(m_worldWidth * 0.790, m_worldHeight * 0.200, 2);
		modelStack.Scale(30, 30, 1);
		RenderMesh(meshList[GEO_BUY], false);
		modelStack.PopMatrix();

		TextFile* aa = new TextFile();
		aa->GetCarStat("Car1", "CarStats.txt");
		InitCarStat("Car1", "SavedCarStats.txt");

		std::ostringstream s10;
		s10.precision(5);
		s10 << "COUNT";
		RenderTextOnScreen(meshList[GEO_TEXT], s10.str(), Color(0, 1, 0), 2.5, 5, 45);
		for (int i = 0; i < player1->playerCar.tranqCount * 5; i += 5)
		{
			modelStack.PushMatrix();
			modelStack.Translate(m_worldWidth * 0.072 + i, m_worldHeight * 0.7, 2);
			modelStack.Scale(1, 1, 1);
			RenderMesh(meshList[GEO_CUBE], false);
			modelStack.PopMatrix();
		}

		std::ostringstream s11;
		s11.precision(5);
		s11 << "DURATION";
		RenderTextOnScreen(meshList[GEO_TEXT], s11.str(), Color(0, 1, 0), 2.5, 4.5, 35);

		for (int i = 0; i < player1->playerCar.tranqDuration * 5; i += 5)
		{
			modelStack.PushMatrix();
			modelStack.Translate(m_worldWidth * 0.072 + i, m_worldHeight * 0.55, 2);
			modelStack.Scale(1, 1, 1);
			RenderMesh(meshList[GEO_CUBE], false);
			modelStack.PopMatrix();
		}
		std::ostringstream s12;
		s12.precision(5);
		s12 << "FREE";
		RenderTextOnScreen(meshList[GEO_TEXT], s12.str(), Color(0, 1, 0), 2.5, 14.5, 1);

		std::ostringstream s13;
		s13.precision(5);
		s13 << "$" << money;
		RenderTextOnScreen(meshList[GEO_TEXT], s13.str(), Color(0, 1, 0), 2.5, 59.5, 1);
	}
	if (gameStates == states::s_Upgrade_Darts2)
	{

		modelStack.PushMatrix();
		modelStack.Translate(m_worldWidth * 0.790, m_worldHeight * 0.500, 2);
		modelStack.Rotate(rotateDisplayY, 1, 0, 0);
		modelStack.Rotate(rotateDisplayX + 90, 0, 1, 0);
		modelStack.Scale(5, 5, 5);
		RenderMesh(meshList[GEO_DISPLAY_DARTS], false);
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		modelStack.Translate(m_worldWidth * 0.790, m_worldHeight * 0.200, 2);
		modelStack.Scale(30, 30, 1);
		RenderMesh(meshList[GEO_BUY], false);
		modelStack.PopMatrix();

		TextFile* aa = new TextFile();
		aa->GetCarStat("Car2", "CarStats.txt");
		InitCarStat("Car2", "SavedCarStats.txt");

		std::ostringstream s10;
		s10.precision(5);
		s10 << "COUNT";
		RenderTextOnScreen(meshList[GEO_TEXT], s10.str(), Color(0, 1, 0), 2.5, 5, 45);
		for (int i = 0; i < player1->playerCar.tranqCount * 5; i += 5)
		{
			modelStack.PushMatrix();
			modelStack.Translate(m_worldWidth * 0.072 + i, m_worldHeight * 0.7, 2);
			modelStack.Scale(1, 1, 1);
			RenderMesh(meshList[GEO_CUBE], false);
			modelStack.PopMatrix();
		}

		std::ostringstream s11;
		s11.precision(5);
		s11 << "DURATION";
		RenderTextOnScreen(meshList[GEO_TEXT], s11.str(), Color(0, 1, 0), 2.5, 4.5, 35);

		for (int i = 0; i < player1->playerCar.tranqDuration * 5; i += 5)
		{
			modelStack.PushMatrix();
			modelStack.Translate(m_worldWidth * 0.072 + i, m_worldHeight * 0.55, 2);
			modelStack.Scale(1, 1, 1);
			RenderMesh(meshList[GEO_CUBE], false);
			modelStack.PopMatrix();
		}
		std::ostringstream s12;
		s12.precision(5);
		s12 << "$" << cost[7];
		RenderTextOnScreen(meshList[GEO_TEXT], s12.str(), Color(0, 0, 1), 2.5, 14.5, 1);

		std::ostringstream s13;
		s13.precision(5);
		s13 << "$" << money;
		RenderTextOnScreen(meshList[GEO_TEXT], s13.str(), Color(0, 1, 0), 2.5, 59.5, 1);
	}
	if (gameStates == states::s_Upgrade_Darts3)
	{
		modelStack.PushMatrix();
		modelStack.Translate(m_worldWidth * 0.790, m_worldHeight * 0.500, 2);
		modelStack.Rotate(rotateDisplayY, 1, 0, 0);
		modelStack.Rotate(rotateDisplayX + 90, 0, 1, 0);
		modelStack.Scale(5, 5, 5);
		RenderMesh(meshList[GEO_DISPLAY_DARTS], false);
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		modelStack.Translate(m_worldWidth * 0.790, m_worldHeight * 0.200, 2);
		modelStack.Scale(30, 30, 1);
		RenderMesh(meshList[GEO_BUY], false);
		modelStack.PopMatrix();

		TextFile* aa = new TextFile();
		aa->GetCarStat("Car3", "CarStats.txt");
		InitCarStat("Car3", "SavedCarStats.txt");

		std::ostringstream s10;
		s10.precision(5);
		s10 << "COUNT";
		RenderTextOnScreen(meshList[GEO_TEXT], s10.str(), Color(0, 1, 0), 2.5, 5, 45);
		for (int i = 0; i < player1->playerCar.tranqCount * 5; i += 5)
		{
			modelStack.PushMatrix();
			modelStack.Translate(m_worldWidth * 0.072 + i, m_worldHeight * 0.7, 2);
			modelStack.Scale(1, 1, 1);
			RenderMesh(meshList[GEO_CUBE], false);
			modelStack.PopMatrix();
		}

		std::ostringstream s11;
		s11.precision(5);
		s11 << "DURATION";
		RenderTextOnScreen(meshList[GEO_TEXT], s11.str(), Color(0, 1, 0), 2.5, 4.5, 35);

		for (int i = 0; i < player1->playerCar.tranqDuration * 5; i += 5)
		{
			modelStack.PushMatrix();
			modelStack.Translate(m_worldWidth * 0.072 + i, m_worldHeight * 0.55, 2);
			modelStack.Scale(1, 1, 1);
			RenderMesh(meshList[GEO_CUBE], false);
			modelStack.PopMatrix();
		}
		std::ostringstream s12;
		s12.precision(5);
		s12 << "$" << cost[7];
		RenderTextOnScreen(meshList[GEO_TEXT], s12.str(), Color(0, 0, 1), 2.5, 14.5, 1);

		std::ostringstream s13;
		s13.precision(5);
		s13 << "$" << money;
		RenderTextOnScreen(meshList[GEO_TEXT], s13.str(), Color(0, 1, 0), 2.5, 59.5, 1);
	}

	if (gameStates == states::s_Lose)
	{
		modelStack.PushMatrix();
		//modelStack.Translate(140, 58, 4);
		//modelStack.Scale(50, 20, 0);
		modelStack.Translate(m_worldWidth / 2, m_worldHeight / 2, 0);
		modelStack.Scale(m_worldWidth, m_worldHeight, 1);
		RenderMesh(meshList[GEO_LOSE_SCENE], false);
		modelStack.PopMatrix();
	}
	if (paused == true)
	{
		modelStack.PushMatrix();
		modelStack.Translate(m_worldWidth / 2, m_worldHeight / 2, 0);
		modelStack.Scale(m_worldWidth, m_worldHeight, 1);
		RenderMesh(meshList[GEO_MENU_PAUSE], false);
		modelStack.PopMatrix();
		}
		if (gameStates == states::s_Highscore)
		{
			modelStack.PushMatrix();
			modelStack.Translate(m_worldWidth / 2, m_worldHeight / 2, 0);
			modelStack.Scale(m_worldWidth, m_worldHeight, 1);
			RenderMesh(meshList[GEO_MENU_HIGHSCORE_PAGE], false);
			modelStack.PopMatrix();

			if (Application::IsKeyPressed(VK_ESCAPE))
			{
				gameStates = states::s_Menu;
			}

			std::ostringstream ss;
			int counter = 0;
			for (int i = 0; i < (sizeof(animalStat->scorearray) / sizeof(*(animalStat->scorearray))); i++)
			{
				if (animalStat->scorearray[i] > 0)
				{
					counter++;
					ss.str("");
					ss.precision(3);
					ss << "#" << i + 1 << " ";
					if (i < 99)
					{
						ss << " ";
					}
					if (i < 9)
					{
						ss << " ";
					}
					ss << animalStat->scorearray[i];
					RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 3, 20, 37 - (i * 3) + scroll);
				}
			}
			scrollimit = 275 - ((100 - counter) * 3);

			std::ostringstream s13;
			s13.precision(5);
			s13 << "$" << money;
			RenderTextOnScreen(meshList[GEO_TEXT], s13.str(), Color(0, 1, 0), 2.5, 59.5, 1);
		}
		if (gameStates == states::s_Win)
		{
			modelStack.PushMatrix();
			//modelStack.Translate(140, 58, 4);
			//modelStack.Scale(50, 20, 0);
			modelStack.Translate(m_worldWidth / 2, m_worldHeight / 2, 0);
			modelStack.Scale(m_worldWidth, m_worldHeight, 1);
			RenderMesh(meshList[GEO_WIN_SCENE], false);
			modelStack.PopMatrix();

			std::ostringstream ss22;
			ss22.precision(5);
			ss22 << " Final score: " << points;
			RenderTextOnScreen(meshList[GEO_TEXT], ss22.str(), Color(0, 1, 0), 3, 17, 21);
		}
		if (gameStates == states::s_Lose)
		{
			std::ostringstream ss22;
			ss22.precision(5);
			ss22 << "score: " << points;
			RenderTextOnScreen(meshList[GEO_TEXT], ss22.str(), Color(0, 1, 0), 3, 17, 21);

			modelStack.PushMatrix();
			//modelStack.Translate(140, 58, 4);
			//modelStack.Scale(50, 20, 0);
			modelStack.Translate(m_worldWidth / 2, m_worldHeight / 2, 0);
			modelStack.Scale(m_worldWidth, m_worldHeight, 1);
			RenderMesh(meshList[GEO_LOSE_SCENE], false);
			modelStack.PopMatrix();
		}
		if (paused == true)
		{
			modelStack.PushMatrix();
			modelStack.Translate(m_worldWidth / 2, m_worldHeight / 2, 7);
			modelStack.Scale(m_worldWidth, m_worldHeight, 1);
			RenderMesh(meshList[GEO_MENU_HIGHSCORE_PAGE2], false);
			modelStack.PopMatrix();
		}
}

void SceneSP3::Render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//Calculating aspect ratio
	m_worldHeight = 100.f;
	m_worldWidth = m_worldHeight * (float)Application::GetWindowWidth() / Application::GetWindowHeight();

	// Projection matrix : Orthographic Projection
	Mtx44 projection;
	projection.SetToOrtho(0, m_worldWidth, 0, m_worldHeight, -40, 10);
	//projection.SetToPerspective(45.0f, 4.0f / 3.0f, 0.1f, 10000.0f);
	projectionStack.LoadMatrix(projection);

	// Camera matrix
	viewStack.LoadIdentity();
	viewStack.LookAt(
		camera.position.x, camera.position.y, camera.position.z,
		camera.target.x, camera.target.y, camera.target.z,
		camera.up.x, camera.up.y, camera.up.z
		);
	// Model matrix : an identity matrix (model will be at the origin)
	modelStack.LoadIdentity();

	RenderMesh(meshList[GEO_AXES], false);

	for (std::vector<GameObject *>::iterator it = m_goList.begin(); it != m_goList.end(); ++it)
	{
		GameObject *go = (GameObject *)*it;
		if (go->active)
		{
			RenderGO(go);
		}
	}
	if (gameStates == states::s_Tutorial ||
		gameStates == states::s_Level2 ||
		gameStates == states::s_Level3 ||
		gameStates == states::s_LevelBoss || 
		gameStates == states::s_MapEditor && testMode == 1)
	{
		for (std::vector<enemy *>::iterator it = enemyList.begin(); it != enemyList.end(); ++it)
		{
			enemy *go = (enemy *)*it;

			RenderEnemy(go);
		}
		RenderLasso(Dalasso);

		modelStack.PushMatrix();
		modelStack.Translate(m_worldWidth / 2.f, m_worldHeight / 2.f, -8.5f);
		modelStack.Scale(m_worldWidth, m_worldHeight, 1);
		//modelStack.Scale(10, 10, 1);
		RenderMesh(meshList[GEO_BACKGROUND], false);
		modelStack.PopMatrix();
	}

	
	//if (m_ghost->active)
	//{
	//	modelStack.PushMatrix();
	//	modelStack.Translate(m_ghost->pos.x, m_ghost->pos.y, m_ghost->pos.z);
	//	modelStack.Scale(m_ghost->scale.x, m_ghost->scale.y, m_ghost->scale.z);
	//	RenderMesh(meshList[GEO_BALL], false);
	//	modelStack.PopMatrix();
	//}

	modelStack.PushMatrix();
	modelStack.Translate(mapPosition.x, mapPosition.y, mapPosition.z - 9);
	modelStack.Scale(testMap.getMapSize().x, testMap.getMapSize().y, testMap.getMapSize().z);
	RenderMesh(testMap.getBackground(), false);
	modelStack.PopMatrix();

	if (gameStates != states::s_MapEditor || gameStates != states::s_Tutorial || gameStates != states::s_Level2 || gameStates != states::s_Level3 || gameStates != states::s_LevelBoss)
	{
		renderMenu();
	}
	if (gameStates == states::s_Tutorial || gameStates == states::s_Level2 || gameStates == states::s_Level3 || gameStates == states::s_LevelBoss || (gameStates == states::s_MapEditor && testMode))
	{
		RenderProps(&testMap);
		//glDisable(GL_DEPTH_TEST);
		for (std::vector<GameObject *>::iterator it = testMap.mapBorder.begin(); it != testMap.mapBorder.end(); ++it)
		{
			GameObject *go = (GameObject *)*it;
			modelStack.PushMatrix();
			//cout << go->pos << endl;
			modelStack.Translate(go->pos.x, go->pos.y, go->pos.z);
			modelStack.Scale(go->scale.x * 5, go->scale.y * 5, go->scale.z);
			RenderMesh(meshList[GEO_TREETOP], false);
			modelStack.PopMatrix();
		}
		//glEnable(GL_DEPTH_TEST);
	}
	if (gameStates == states::s_Upgrade_Cars1)
	{
		modelStack.PushMatrix();
		modelStack.Translate(m_worldWidth * 0.790, m_worldHeight * 0.400, 2);
		modelStack.Rotate(rotateDisplayY + 40, 1, 0, 0);
		modelStack.Rotate(rotateDisplayX - 45, 0, 1, 0);
		modelStack.Scale(10, 10, 10);
		RenderMesh(meshList[GEO_DISPLAY_CAR1], false);
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		modelStack.Translate(m_worldWidth * 0.790, m_worldHeight * 0.200, 2);
		modelStack.Scale(30, 30, 1);
		RenderMesh(meshList[GEO_BUY], false);
		modelStack.PopMatrix();
	}
	if (gameStates == states::s_Upgrade_Cars2)
	{
		modelStack.PushMatrix();
		modelStack.Translate(m_worldWidth * 0.790, m_worldHeight * 0.400, 2);
		modelStack.Rotate(rotateDisplayY + 40, 1, 0, 0);
		modelStack.Rotate(rotateDisplayX + 45, 0, 1, 0);
		modelStack.Scale(15, 15, 15);
		RenderMesh(meshList[GEO_DISPLAY_CAR2], false);
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		modelStack.Translate(m_worldWidth * 0.790, m_worldHeight * 0.200, 2);
		modelStack.Scale(30, 30, 1);
		RenderMesh(meshList[GEO_BUY], false);
		modelStack.PopMatrix();
	}
	if (gameStates == states::s_Upgrade_Cars3)
	{
		modelStack.PushMatrix();
		modelStack.Translate(m_worldWidth * 0.790, m_worldHeight * 0.400, 2);
		modelStack.Rotate(rotateDisplayY + 40, 1, 0, 0);
		modelStack.Rotate(rotateDisplayX + 45, 0, 1, 0);
		modelStack.Scale(15, 15, 15);
		RenderMesh(meshList[GEO_DISPLAY_CAR3], false);
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		modelStack.Translate(m_worldWidth * 0.790, m_worldHeight * 0.200, 2);
		modelStack.Scale(30, 30, 1);
		RenderMesh(meshList[GEO_BUY], false);
		modelStack.PopMatrix();
	}
	if (gameStates == states::s_Upgrade_Tires1)
	{
		modelStack.PushMatrix();
		modelStack.Translate(m_worldWidth * 0.790, m_worldHeight * 0.500, 2);
		modelStack.Rotate(rotateDisplayY, 1, 0, 0);
		modelStack.Rotate(rotateDisplayX - 45, 0, 1, 0);
		modelStack.Scale(1.5, 1.5, 1.5);
		RenderMesh(meshList[GEO_DISPLAY_WHEEL], false);
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		modelStack.Translate(m_worldWidth * 0.790, m_worldHeight * 0.200, 2);
		modelStack.Scale(30, 30, 1);
		RenderMesh(meshList[GEO_BUY], false);
		modelStack.PopMatrix();
	}
	if (gameStates == states::s_Upgrade_Tires2)
	{
		modelStack.PushMatrix();
		modelStack.Translate(m_worldWidth * 0.790, m_worldHeight * 0.500, 2);
		modelStack.Rotate(rotateDisplayY, 1, 0, 0);
		modelStack.Rotate(rotateDisplayX - 45, 0, 1, 0);
		modelStack.Scale(1.5, 1.5, 1.5);
		RenderMesh(meshList[GEO_DISPLAY_WHEEL], false);
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		modelStack.Translate(m_worldWidth * 0.790, m_worldHeight * 0.200, 2);
		modelStack.Scale(30, 30, 1);
		RenderMesh(meshList[GEO_BUY], false);
		modelStack.PopMatrix();
	}
	if (gameStates == states::s_Upgrade_Tires3)
	{
		modelStack.PushMatrix();
		modelStack.Translate(m_worldWidth * 0.790, m_worldHeight * 0.500, 2);
		modelStack.Rotate(rotateDisplayY, 1, 0, 0);
		modelStack.Rotate(rotateDisplayX - 45, 0, 1, 0);
		modelStack.Scale(1.5, 1.5, 1.5);
		RenderMesh(meshList[GEO_DISPLAY_WHEEL], false);
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		modelStack.Translate(m_worldWidth * 0.790, m_worldHeight * 0.200, 2);
		modelStack.Scale(30, 30, 1);
		RenderMesh(meshList[GEO_BUY], false);
		modelStack.PopMatrix();
	}
	if (gameStates == states::s_Upgrade_Lasso1 || gameStates == states::s_Upgrade_Lasso2 || gameStates == states::s_Upgrade_Lasso3)
	{
		/*modelStack.PushMatrix();
		modelStack.Translate(m_worldWidth * 0.790, m_worldHeight * 0.500, 2);
		modelStack.Rotate(rotateDisplayY, 1, 0, 0);
		modelStack.Rotate(rotateDisplayX - 45, 0, 1, 0);
		modelStack.Scale(1.5, 1.5, 1.5);
		RenderMesh(meshList[GEO_DISPLAY_ROPE], false);
		modelStack.PopMatrix();*/

		modelStack.PushMatrix();
		modelStack.Translate(m_worldWidth * 0.790, m_worldHeight * 0.200, 2);
		modelStack.Scale(30, 30, 1);
		RenderMesh(meshList[GEO_BUY], false);
		modelStack.PopMatrix();
	}
	if (gameStates == states::s_Upgrade_Darts1 || gameStates == states::s_Upgrade_Darts2 || gameStates == states::s_Upgrade_Darts3)
	{
		modelStack.PushMatrix();
		modelStack.Translate(m_worldWidth * 0.790, m_worldHeight * 0.500, 2);
		modelStack.Rotate(rotateDisplayY, 1, 0, 0);
		modelStack.Rotate(rotateDisplayX + 90, 0, 1, 0);
		modelStack.Scale(5, 5, 5);
		RenderMesh(meshList[GEO_DISPLAY_DARTS], false);
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		modelStack.Translate(m_worldWidth * 0.790, m_worldHeight * 0.200, 2);
		modelStack.Scale(30, 30, 1);
		RenderMesh(meshList[GEO_BUY], false);
		modelStack.PopMatrix();
	}
	if (gameStates == states::s_MapEditor )
	{

		mapEditorRender();
	}

	//modelStack.PushMatrix();
	//modelStack.Translate(zebra->getPos().x, zebra->getPos().y, zebra->getPos().z );
	//modelStack.Scale(3,3,3);
	//RenderMesh(meshList[GEO_ENEMY], true);
	//modelStack.PopMatrix();

	//modelStack.PushMatrix();
	//modelStack.Translate(m_worldWidth - 6, 94, 0);
	//modelStack.Scale(4, 4, 1);
	//RenderMesh(meshList[GEO_POWERUP], false);
	//modelStack.PopMatrix();

	//On screen text

	//Exercise 5: Render m_objectCount
	//std::ostringstream ss3;
	//ss3.precision(5);
	//ss3 << "m_objectCount: " << m_objectCount;
	//RenderTextOnScreen(meshList[GEO_TEXT], ss3.str(), Color(0, 1, 0), 3, 0, 9);

	//Exercise 8c: Render initial and final momentum

	//ss2.str("");
	//ss2.precision(3);
	//ss2 << "Speed: " << m_speed;
	//RenderTextOnScreen(meshList[GEO_TEXT], ss2.str(), Color(0, 1, 0), 3, 0, 6);

	double x, y;
	Application::GetCursorPos(&x, &y);
	int w = Application::GetWindowWidth();
	int h = Application::GetWindowHeight();
	float worldX = x * m_worldWidth / w;
	float worldY = (h - y) * m_worldHeight / h;

	/*std::ostringstream ss11;
	ss11.precision(5);
	ss11 << "count: " << testMap.propCount;
	RenderTextOnScreen(meshList[GEO_TEXT], ss11.str(), Color(0, 1, 0), 3, 0, 6);*/

	if (gameStates == states::s_Tutorial ||
		gameStates == states::s_Level2 ||
		gameStates == states::s_Level3 ||
		gameStates == states::s_LevelBoss)
	{
		std::ostringstream ss22;
		ss22.precision(5);
		ss22 << "score: " << points;
		RenderTextOnScreen(meshList[GEO_TEXT], ss22.str(), Color(0, 1, 0), 3, 0, 9);
	}

	std::ostringstream ss;
	ss.precision(5);
	ss << "Pos: " << worldX / m_worldWidth << ", " << worldY;
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 3, 0, 3);

	std::ostringstream ssz;
	ssz.precision(5);
	ssz << "FPS: " << fps;
	RenderTextOnScreen(meshList[GEO_TEXT], ssz.str(), Color(0, 1, 0), 3, 0, 0);

	//RenderTextOnScreen(meshList[GEO_TEXT], "Collision", Color(0, 1, 0), 3, 0, 0);
	if (gameStates == states::s_Tutorial ||
		gameStates == states::s_Level2 ||
		gameStates == states::s_Level3 ||
		gameStates == states::s_LevelBoss ||
		gameStates == states::s_MapEditor && testMode == 1)
	{
		std::ostringstream ss3;
		ss3.precision(3);
		ss3 << "Fuel: " << fuelAmount;
		RenderTextOnScreen(meshList[GEO_TEXT], ss3.str(), Color(0, 1, 1), 3.5, 3, 55);

		std::ostringstream ss22;
		ss22.precision(5);
		ss22 << "score: " << points;
		RenderTextOnScreen(meshList[GEO_TEXT], ss22.str(), Color(0, 1, 1), 3.5, 50, 55);

		if (fogActive)
		{
			modelStack.PushMatrix();
			modelStack.Translate(0, 0, 9);
			modelStack.Scale(1000, 1000, 1);
			RenderMesh(meshList[GEO_FOG], false);
			modelStack.PopMatrix();
		}
	}

}

void SceneSP3::Exit()
{
	if (Sound_Snap != NULL)
	{
		Sound_Snap->drop();
	}
	if (Sound_Engine != NULL)
	{
		Sound_Engine->drop();
	}
	if (Sound_Bump != NULL)
	{
		Sound_Bump->drop();
	}
	if (Sound_Ding != NULL)
	{
		Sound_Ding->drop();
	}
	if (Sound_Throw != NULL)
	{
		Sound_Throw->drop();
	}
	if (Sound_Back != NULL)
	{
		Sound_Back->drop();
	}
	if (theSoundEngine != NULL)
	{
		theSoundEngine->drop();
	}
	SceneBase::Exit();
	//Cleanup GameObjects
	ofstream file;
	file.open("tempsave.txt", ios::trunc);
	file.close();
	while (m_goList.size() > 0)
	{
		GameObject *go = m_goList.back();
		delete go;
		m_goList.pop_back();
	}
	if (m_ghost)
	{
		delete m_ghost;
		m_ghost = NULL;
	}
}