#include "SceneAssignment2.h"
#include "GL\glew.h"
#include "Application.h"
#include <sstream>

SceneAssignment2::SceneAssignment2() 
{
}

SceneAssignment2::~SceneAssignment2()
{
}

void SceneAssignment2::Init()
{
	SceneBase::Init();

	//Physics code here
	m_speed = 1.f;

	Math::InitRNG();

	//Exercise 1: initialize m_objectCount
	m_objectCount = 0;

	m_ghost = new GameObject(GameObject::GO_BALL);

	player1 = new GameObject(GameObject::GO_CAR);
	player1->pos.Set(25, 50, 1);
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
	//m_goList.push_back(player1);

	player2 = new GameObject(GameObject::GO_CAR);
	player2->pos.Set(110, 50, 1);
	player2->mass = 5;
	player2->vel.Set(0, 0, 0);
	player2->rotationAngle = 180;
	player2->normal = Vector3(cos(Math::DegreeToRadian(player2->rotationAngle)), sin(Math::DegreeToRadian(player2->rotationAngle)), 0);
	player2->rotatedVel.SetToRotation(player2->rotationAngle, 0, 0, 1);
	player2->engine = 0;
	player2->turnPower = 3;
	player2->scale.Set(10, 5, 1);

	player2->playerCar.engine = 3;
	player2->playerCar.mass = 5;
	player2->playerCar.topSpeed = 50;
	player2->playerCar.turnSpeed = 4;
	player2->playerCar.acceleration = 0.3;
	player2->playerCar.powerUp = 1;
	player2->active = false;
	//player2->active = true;
	//m_goList.push_back(player2);

	playerAI = new GameObject(GameObject::GO_CAR);
	playerAI->pos.Set(110, 50, 1);
	playerAI->mass = 5;
	playerAI->vel.Set(0, 0, 0);
	playerAI->rotationAngle = 180;
	playerAI->normal = Vector3(cos(Math::DegreeToRadian(player1->rotationAngle)), sin(Math::DegreeToRadian(player1->rotationAngle)), 0);
	playerAI->rotatedVel.SetToRotation(player1->rotationAngle, 0, 0, 1);
	playerAI->engine = 0;
	playerAI->turnPower = 3;
	playerAI->scale.Set(10, 5, 1);

	playerAI->playerCar.engine = Math::RandIntMinMax(3, 5);
	playerAI->playerCar.mass = Math::RandIntMinMax(5, 9);
	playerAI->playerCar.topSpeed = Math::RandIntMinMax(30, 40);
	playerAI->playerCar.turnSpeed = Math::RandIntMinMax(2, 4);
	playerAI->playerCar.acceleration = Math::RandFloatMinMax(0.4f, 0.5f);
	playerAI->playerCar.powerUp = 1;
	playerAI->active = false;
	//playerAI->active = true;
	//m_goList.push_back(playerAI);

	player1goal = new GameObject(GameObject::GO_GOAL);
	player1goal->pos.Set(120, 50, 1);
	player1goal->active = true;
	player1goal->scale.Set(10, 20, 1);
	m_goList.push_back(player1goal);

	player2goal = new GameObject(GameObject::GO_GOAL);
	player2goal->pos.Set(10, 50, 1);
	player2goal->active = true;
	player2goal->scale.Set(10, 20, 1);
	m_goList.push_back(player2goal);

	//GameObject* ball = FetchGO();
	soccerBall = new GameObject(GameObject::GO_BALL);
	soccerBall->type = GameObject::GO_BALL;
	soccerBall->scale.Set(3.5f, 3.5f, 3.5f);
	soccerBall->pos = Vector3(67.5f, 50.f, 1.f);
	soccerBall->vel = Vector3(0, 0, 0);
	soccerBall->mass = 1.5f * 1.5f * 1.5f;
	soccerBall->rotatedVel.SetToRotation(0, 0, 0, 1);
	soccerBall->ballrotated = 0;
	soccerBall->active = false;
	m_goList.push_back(soccerBall);

	GameObject* power = FetchGO();
	power->type = GameObject::GO_POWERUP;
	power->scale.Set(1, 1, 1);
	power->pos = Vector3(37.5f, 75.f, 1.f);
	power->vel = Vector3(0, 0, 0);
	power->mass = 0;
	power->ticker = 10;
	power->normal.Set(0, 1, 0);
	power->active = true;

	GameObject* power2 = FetchGO();
	power2->type = GameObject::GO_POWERUP;
	power2->scale.Set(1, 1, 1);
	power2->pos = Vector3(97.5f, 75.f, 1.f);
	power2->vel = Vector3(0, 0, 0);
	power2->mass = 0;
	power2->ticker = 10;
	power2->normal.Set(0, 1, 0);
	power2->active = true;

	GameObject* power3 = FetchGO();
	power3->type = GameObject::GO_POWERUP;
	power3->scale.Set(1, 1, 1);
	power3->pos = Vector3(37.5f, 25.f, 1.f);
	power3->vel = Vector3(0, 0, 0);
	power3->mass = 0;
	power3->ticker = 10;
	power3->normal.Set(0, 1, 0);
	power3->active = true;

	GameObject* power4 = FetchGO();
	power4->type = GameObject::GO_POWERUP;
	power4->scale.Set(1, 1, 1);
	power4->pos = Vector3(97.5f, 25.f, 1.f);
	power4->vel = Vector3(0, 0, 0);
	power4->mass = 0;
	power4->ticker = 10;
	power4->normal.Set(0, 1, 0);
	power4->active = true;

	debug = Vector3(0, 0, 0);
	debugscale = Vector3(2, 2, 1);
	debugactive = false;
	friction = 0.95;
	timer = 0;
	timer2 = 0;
	gameStart = false;
	m_ballCount = 0;
	player1score = 0;
	player2score = 0;
	player1power = 0;
	player2power = 0;
	player1poweractive = 0;
	player1poweractive = 0;
	player1poweractived = false;
	player1poweractived = false;
	min3 = 1;
	second3 = 15;
	loading = true;
	internationalDT = 0;
	mainMenu = true;
	numOfPlayers = 1;
	gameModeSelected = false;
	player1preset = 0;
	player2preset = 0;
	maxCars = 5;
	player1ready = false;
	player2ready = false;
	spawnMap();
}

void SceneAssignment2::Reset()
{
	//Physics code here
	m_speed = 1.f;

	Math::InitRNG();

	//Exercise 1: initialize m_objectCount
	m_objectCount = 0;

	m_ghost = new GameObject(GameObject::GO_BALL);

	player1 = new GameObject(GameObject::GO_CAR);
	player1->pos.Set(25, 50, 1);
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
	//m_goList.push_back(player1);

	player2 = new GameObject(GameObject::GO_CAR);
	player2->pos.Set(110, 50, 1);
	player2->mass = 5;
	player2->vel.Set(0, 0, 0);
	player2->rotationAngle = 180;
	player2->normal = Vector3(cos(Math::DegreeToRadian(player2->rotationAngle)), sin(Math::DegreeToRadian(player2->rotationAngle)), 0);
	player2->rotatedVel.SetToRotation(player2->rotationAngle, 0, 0, 1);
	player2->engine = 0;
	player2->turnPower = 3;
	player2->scale.Set(10, 5, 1);

	player2->playerCar.engine = 3;
	player2->playerCar.mass = 5;
	player2->playerCar.topSpeed = 50;
	player2->playerCar.turnSpeed = 4;
	player2->playerCar.acceleration = 0.3;
	player2->playerCar.powerUp = 2;
	player2->active = false;
	//player2->active = true;
	//m_goList.push_back(player2);

	playerAI = new GameObject(GameObject::GO_CAR);
	playerAI->pos.Set(110, 50, 1);
	playerAI->mass = 5;
	playerAI->vel.Set(0, 0, 0);
	playerAI->rotationAngle = 180;
	playerAI->normal = Vector3(cos(Math::DegreeToRadian(player1->rotationAngle)), sin(Math::DegreeToRadian(player1->rotationAngle)), 0);
	playerAI->rotatedVel.SetToRotation(player1->rotationAngle, 0, 0, 1);
	playerAI->engine = 0;
	playerAI->turnPower = 3;
	playerAI->scale.Set(10, 5, 1);

	playerAI->playerCar.engine = Math::RandIntMinMax(3, 5);
	playerAI->playerCar.mass = Math::RandIntMinMax(5, 9);
	playerAI->playerCar.topSpeed = Math::RandIntMinMax(30, 40);
	playerAI->playerCar.turnSpeed = Math::RandIntMinMax(2, 4);
	playerAI->playerCar.acceleration = Math::RandFloatMinMax(0.4f, 0.5f);
	playerAI->playerCar.powerUp = 1;
	playerAI->active = false;
	//playerAI->active = true;
	//m_goList.push_back(playerAI);

	player1goal = new GameObject(GameObject::GO_GOAL);
	player1goal->pos.Set(120, 50, 1);
	player1goal->active = true;
	player1goal->scale.Set(10, 20, 1);
	m_goList.push_back(player1goal);

	player2goal = new GameObject(GameObject::GO_GOAL);
	player2goal->pos.Set(10, 50, 1);
	player2goal->active = true;
	player2goal->scale.Set(10, 20, 1);
	m_goList.push_back(player2goal);

	//GameObject* ball = FetchGO();
	soccerBall = new GameObject(GameObject::GO_BALL);
	soccerBall->type = GameObject::GO_BALL;
	soccerBall->scale.Set(3.5f, 3.5f, 3.5f);
	soccerBall->pos = Vector3(67.5f, 50.f, 1.f);
	soccerBall->vel = Vector3(0, 0, 0);
	soccerBall->mass = 1.5f * 1.5f * 1.5f;
	soccerBall->rotatedVel.SetToRotation(0, 0, 0, 1);
	soccerBall->ballrotated = 0;
	soccerBall->active = false;
	m_goList.push_back(soccerBall);

	GameObject* power = FetchGO();
	power->type = GameObject::GO_POWERUP;
	power->scale.Set(1, 1, 1);
	power->pos = Vector3(37.5f, 75.f, 1.f);
	power->vel = Vector3(0, 0, 0);
	power->mass = 0;
	power->ticker = 10;
	power->normal.Set(0, 1, 0);
	power->active = true;

	GameObject* power2 = FetchGO();
	power2->type = GameObject::GO_POWERUP;
	power2->scale.Set(1, 1, 1);
	power2->pos = Vector3(97.5f, 75.f, 1.f);
	power2->vel = Vector3(0, 0, 0);
	power2->mass = 0;
	power2->ticker = 10;
	power2->normal.Set(0, 1, 0);
	power2->active = true;

	GameObject* power3 = FetchGO();
	power3->type = GameObject::GO_POWERUP;
	power3->scale.Set(1, 1, 1);
	power3->pos = Vector3(37.5f, 25.f, 1.f);
	power3->vel = Vector3(0, 0, 0);
	power3->mass = 0;
	power3->ticker = 10;
	power3->normal.Set(0, 1, 0);
	power3->active = true;

	GameObject* power4 = FetchGO();
	power4->type = GameObject::GO_POWERUP;
	power4->scale.Set(1, 1, 1);
	power4->pos = Vector3(97.5f, 25.f, 1.f);
	power4->vel = Vector3(0, 0, 0);
	power4->mass = 0;
	power4->ticker = 10;
	power4->normal.Set(0, 1, 0);
	power4->active = true;

	debug = Vector3(0, 0, 0);
	debugscale = Vector3(2, 2, 1);
	debugactive = false;
	friction = 0.95;
	timer = 0;
	gameStart = false;
	m_ballCount = 0;
	player1score = 0;
	player2score = 0;
	player1power = 0;
	player2power = 0;
	player1poweractive = 0;
	player1poweractive = 0;
	player1poweractived = false;
	player1poweractived = false;
	min3 = 1;
	second3 = 15;
	loading = true;
	internationalDT = 0;
	mainMenu = true;
	numOfPlayers = 1;
	gameModeSelected = false;
	player1preset = 0;
	player2preset = 0;
	maxCars = 5;
	player1ready = false;
	player2ready = false;
	spawnMap();
}

GameObject* SceneAssignment2::FetchGO()
{
	//Exercise 2a: implement FetchGO()

	for (std::vector<GameObject*>::iterator iter = m_goList.begin(); iter != m_goList.end(); ++iter)
	{
		GameObject *go = *iter;
		if (go->active == false && (go->type == GameObject::GO_BALL || go->type == GameObject::GO_PARTICLE) && go != soccerBall)
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

bool SceneAssignment2::CheckCollision(GameObject *go, GameObject *other, double dt)
{
	float distanceSquared = ((other->pos + other->vel * dt) - (go->pos + go->vel * dt)).LengthSquared();
	float combinedRadiusSquared = (go->scale.x + other->scale.x) * (go->scale.x + other->scale.x);
	Vector3 relativeVelocity = go->vel - other->vel;
	Vector3 relativeDisplacement = other->pos - go->pos;
	return (distanceSquared < combinedRadiusSquared && relativeVelocity.Dot(relativeDisplacement) > 0);
}

float SceneAssignment2::CheckCollision2(GameObject *go1, GameObject *go2)
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

bool SceneAssignment2::CheckCollision3(GameObject *go, GameObject *other, double dt)
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
	case GameObject::GO_GOAL:
	{
								if (go->pos.x - go->scale.x > other->pos.x - other->scale.x / 2 && go->pos.x + go->scale.x < other->pos.x + other->scale.x / 2)
								{
									if (go->pos.y - go->scale.y > other->pos.y - other->scale.y / 2 && go->pos.y + go->scale.y < other->pos.y + other->scale.y / 2)
									{
										return true;
									}
								}
								return false;
								break;
	}
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
								   float combinedRadiusSquared = (go->scale.x) * (go->scale.x);
								   Vector3 relativeVelocity = go->vel - other->vel;
								   Vector3 relativeDisplacement = w0 - go->pos;
								   if (distanceSquared < combinedRadiusSquared && relativeVelocity.Dot(relativeDisplacement) > 0)
								   {
									   Vector3 u = go->vel;
									   Vector3 N = (w0 - go->pos).Normalized();
									   Vector3 uN = u.Dot(N) * N;
									   go->vel = other->rotatedVel * (u - 2 * uN) + other->vel;
									   go->vel.z = 0;
									   go->vel += other->vel;

									   v1 = other->rotatedVel * go->vel;
									   v2 = other->vel;
									   debug = go->pos;
									   debugactive = true;
									   debugscale = Vector3(2, 2, 2);
									   if (other == player1)
										   debugcolor = 1;
									   else if (other == player2 || other == playerAI)
										   debugcolor = 2;
									   else
										   debugcolor = 0;
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
											   debug = go->pos;
											   debugactive = true;
											   debugscale = Vector3(2, 2, 2);
											   if (other == player1)
												   debugcolor = 1;
											   else if (other == player2 || other == playerAI)
												   debugcolor = 2;
											   else
												   debugcolor = 0;
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
											   debug = go->pos;
											   debugactive = true;
											   debugscale = Vector3(2, 2, 2);
											   if (other == player1)
												   debugcolor = 1;
											   else if (other == player2 || other == playerAI)
												   debugcolor = 2;
											   else
												   debugcolor = 0;
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

bool SceneAssignment2::carballCollide(GameObject *go, GameObject *other, double dt)
{
	float b1 = cos(Math::DegreeToRadian(-other->rotationAngle)) * (go->pos.x - other->pos.x) - sin(Math::DegreeToRadian(-other->rotationAngle)) * (go->pos.y - other->pos.y) + other->pos.x;
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
		float combinedRadiusSquared = (go->scale.x) * (go->scale.x);
		Vector3 relativeVelocity = go->vel - other->vel;
		Vector3 relativeDisplacement = w0 - go->pos;
		if (distanceSquared < combinedRadiusSquared && relativeVelocity.Dot(relativeDisplacement) > 0)
		{
			return true;
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
			return true;
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
					return true;
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
					return true;
				}
			}
		}
	}
	return false;
}

void SceneAssignment2::CollisionResponse(GameObject *go, GameObject *other)
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
									m_ballCount--;
									//m_objectCount--;

									soccerBall->pos = Vector3(67.5f, 50.f, 1.f);
									soccerBall->vel = Vector3(0, 0, 0);
									soccerBall->ballrotated = 0;

									if (other == player1goal)
									{
										player1score++;
									}
									else if (other == player2goal)
									{
										player2score++;
									}
								}
	}
	}

}

void SceneAssignment2::loadPlayer1()
{
	string data;
	ifstream myfile("carz.txt");
	if (myfile.is_open())
	{
		for (int i = 0; i <= player1preset; i++)	
		{
			getline(myfile, data);

		}
		myfile.close();
	}

	else std::cout << "Unable to open file";


	int pos1 = data.find(",");
	int pos2 = data.find("-");
	int pos3 = data.find("+");
	int pos4 = data.find("=");
	int pos5 = data.find("&");
	int pos6 = data.find("~");
	int pos7 = data.find("!");
	player1->playerCar.name = data.substr(0, pos6);
	player1->playerCar.engine = stoi((data.substr(pos6 + 1, pos1 - pos6 - 1)).c_str());
	player1->playerCar.topSpeed = stoi((data.substr(pos1 + 1, pos2 - pos1 - 1)).c_str());
	player1->playerCar.turnSpeed = stoi((data.substr(pos2 + 1, pos3 - pos2 - 1)).c_str());
	player1->playerCar.mass = stoi((data.substr(pos3 + 1, pos4 - pos3 - 1)).c_str());
	player1->playerCar.acceleration = 1.f / std::stof((data.substr(pos4 + 1, pos5 - pos4 - 1)).c_str());
	//player1->playerCar.acceleration = (float)((int)(player1->playerCar.acceleration * 100) / 100.f);
	player1->playerCar.powerUp = stoi((data.substr(pos5 + 1, pos7 - pos5 - 1)).c_str());

	float scale = (float)stoi((data.substr(pos7 + 1, data.length() - pos7)).c_str()) / 10.f;
	player1->scale.Set(10 * scale, 5 * scale, 1);
}

void SceneAssignment2::loadPlayer2()
{
	string data;
	ifstream myfile("carz.txt");
	if (myfile.is_open())
	{

		//while (getline(myfile, data)) {
		//	std::cout << data << std::endl;
		//}
		for (int i = 0; i <= player2preset; i++)
		{
			getline(myfile, data);

		}
		myfile.close();
	}

	else std::cout << "Unable to open file";


	int pos1 = data.find(",");
	int pos2 = data.find("-");
	int pos3 = data.find("+");
	int pos4 = data.find("=");
	int pos5 = data.find("&");
	int pos6 = data.find("~");
	int pos7 = data.find("!");
	player2->playerCar.name = data.substr(0, pos6);
	player2->playerCar.engine = stoi((data.substr(pos6 + 1, pos1 - pos6 - 1)).c_str());
	player2->playerCar.topSpeed = stoi((data.substr(pos1 + 1, pos2 - pos1 - 1)).c_str());
	player2->playerCar.turnSpeed = stoi((data.substr(pos2 + 1, pos3 - pos2 - 1)).c_str());
	player2->playerCar.mass = stoi((data.substr(pos3 + 1, pos4 - pos3 - 1)).c_str());
	player2->playerCar.acceleration = 1.f / std::stof((data.substr(pos4 + 1, pos5 - pos4 - 1)).c_str());
	//player2->playerCar.acceleration = (float)((int)(player1->playerCar.acceleration * 100) / 100.f);
	player2->playerCar.powerUp = stoi((data.substr(pos5 + 1, pos7 - pos5 - 1)).c_str());

	float scale = (float)stoi((data.substr(pos7 + 1, data.length() - pos7)).c_str()) / 10.f;
	player2->scale.Set(10 * scale, 5 * scale, 1);
}

void SceneAssignment2::menuUpdate(double dt)
{
	if (!gameModeSelected)
	{
		if (numOfPlayers == 1)
		{
			if (Application::IsKeyPressed(VK_RIGHT) || Application::IsKeyPressed('D'))
			{
				numOfPlayers = 2;
			}
		}
		else
		{
			if (Application::IsKeyPressed(VK_LEFT) || Application::IsKeyPressed('A'))
			{
				numOfPlayers = 1;
			}
		}
		if (Application::IsKeyPressed(VK_RETURN) || Application::IsKeyPressed(VK_SPACE))
		{
			gameModeSelected = true;
			loadPlayer1();
		}
	}
	else
	{
		if (numOfPlayers == 1)
		{
			static bool DButtonState = false;
			if (!DButtonState && Application::IsKeyPressed('D'))
			{
				DButtonState = true;
				player1preset++;
				if (player1preset > maxCars)
					player1preset = maxCars;
				loadPlayer1();
			}
			else if (DButtonState && !Application::IsKeyPressed('D'))
			{
				DButtonState = false;
			}
			static bool AButtonState = false;
			if (!AButtonState && Application::IsKeyPressed('A'))
			{
				AButtonState = true;
				player1preset--;
				if (player1preset < 0)
					player1preset = 0;

				loadPlayer1();
			}
			else if (AButtonState && !Application::IsKeyPressed('A'))
			{
				AButtonState = false;
			}
			if (Application::IsKeyPressed(VK_LSHIFT))
			{
				mainMenu = false;
				gameStart = true;
				player1->active = true;
				playerAI->active = true;
				m_goList.push_back(player1);
				m_goList.push_back(playerAI);
			}
		}
		else if (numOfPlayers == 2)
		{
			if (!player1ready)
			{
				static bool DButtonState = false;
				if (!DButtonState && Application::IsKeyPressed('D'))
				{
					DButtonState = true;
					player1preset++;
					if (player1preset > maxCars)
						player1preset = maxCars;

					loadPlayer1();
				}
				else if (DButtonState && !Application::IsKeyPressed('D'))
				{
					DButtonState = false;
				}
				static bool AButtonState = false;
				if (!AButtonState && Application::IsKeyPressed('A'))
				{
					AButtonState = true;
					player1preset--;
					if (player1preset < 0)
						player1preset = 0;

					loadPlayer1();
				}
				else if (AButtonState && !Application::IsKeyPressed('A'))
				{
					AButtonState = false;
				}
				if (Application::IsKeyPressed(VK_LSHIFT))
				{
					player1ready = true;

					player2preset = 0;
					loadPlayer2();
				}
			}
			else if (!player2ready)
			{
				static bool RKeyPress = false;
				if (!RKeyPress && Application::IsKeyPressed(VK_RIGHT))
				{
					RKeyPress = true;
					player2preset++;
					if (player2preset > maxCars)
						player2preset = maxCars;

					loadPlayer2();
				}
				else if (RKeyPress && !Application::IsKeyPressed(VK_RIGHT))
				{
					RKeyPress = false;
				}
				static bool LKeyPress = false;
				if (!LKeyPress && Application::IsKeyPressed(VK_LEFT))
				{
					LKeyPress = true;
					player2preset--;
					if (player2preset < 0)
						player2preset = 0;

					loadPlayer2();
				}
				else if (LKeyPress && !Application::IsKeyPressed(VK_LEFT))
				{
					LKeyPress = false;
				}
				if (Application::IsKeyPressed(VK_RSHIFT))
				{
					player2ready = true;
				}
			}
			else
			{
				mainMenu = false;
				//gameStart = true;
				player1->active = true;
				player2->active = true;
				m_goList.push_back(player1);
				m_goList.push_back(player2);
			}
		}
	}
	
}

void SceneAssignment2::Update(double dt)
{
	SceneBase::Update(dt);

	if (Application::IsKeyPressed('R'))
	{
		mainMenu = true;
		soccerBall->active = false;
		gameStart = false;
		player1->active = false;
		player2->active = false;
		playerAI->active = false;
		gameModeSelected = false;
		//Cleanup GameObjects
		while (m_goList.size() > 0)
		{
			GameObject *go = m_goList.back();
			delete go;
			m_goList.pop_back();
		}
		Reset();
	}

	if (mainMenu)
	{
		menuUpdate(dt);
	}
	else
	{
		if (gameStart == true && !soccerBall->active)
		{
			soccerBall->scale.Set(3.5f, 3.5f, 3.5f);
			soccerBall->pos = Vector3(67.5f, 50.f, 1.f);
			soccerBall->vel = Vector3(0, 0, 0);
			soccerBall->mass = 1.5f * 1.5f * 1.5f;
			soccerBall->rotatedVel.SetToRotation(0, 0, 0, 1);
			soccerBall->ballrotated = 0;
			soccerBall->active = true;

			m_ballCount++;
			gameStart = false;
		}

		static bool WPressed = false;
		static bool UpPressed = false;

		timer++;
		timer2++;
		internationalDT++;

		if (timer2 > 10)
		{
			WPressed = false;
			UpPressed = false;
			if (playerAI->active == true)
			{
				GameObject* particle = FetchGO();
				particle->type = GameObject::GO_PARTICLE;
				particle->scale.Set(1.5f, 1.5f, 1.5f);
				particle->pos = playerAI->pos;
				particle->vel = Vector3(0, 0, 0);
				particle->mass = 10.f;
				particle->ballrotated = 0;
				particle->ticker = 5;
				particle->pos.z = -1;
			}
		}

		if (player1poweractive > 0)
			player1poweractive--;
		else
			player1poweractived = false;
		if (player2poweractive > 0)
			player2poweractive--;
		else
			player2poweractived = false;

		
		if (timer > 1 / dt)
		{
			
			loading = false;
			timer -= 1 / dt;
			if (gameStart == false && m_ballCount <= 0)
			{
				if (second3 || min3)
					gameStart = true;
			}
			if (second3 <= 0 && min3 > 0)
			{
				second3 = 59;
				min3--;
			}
			else if (second3 > 0)
			{
				second3--;
			}

			for (std::vector<GameObject *>::iterator it = m_goList.begin(); it != m_goList.end(); ++it)
			{
				GameObject *go = (GameObject *)*it;
				if (go->type == GameObject::GO_POWERUP)
				{
					//std::cout << go->ticker << std::endl;
					if (go->ticker > 0)
					{
						go->ticker--;
					}
					if (go->ticker <= 0)
					{
						go->active = true;
					}
				}
			}
			//std::cout << std::endl;
		}

		if (!second3 && !min3)
		{
			if (abs(player1score - player2score))
			{
				soccerBall->active = false;
				gameStart = false;
			}
		}

		if (Application::IsKeyPressed('9'))
		{
			m_speed = Math::Max(0.f, m_speed - 0.1f);
		}
		if (Application::IsKeyPressed('0'))
		{
			m_speed += 0.1f;
		}
		
		if (Application::IsKeyPressed('W'))
		{
			player1->engine += player1->playerCar.acceleration;
			if (!WPressed)
			{
				WPressed = true;
				GameObject* particle = FetchGO();
				particle->type = GameObject::GO_PARTICLE;
				particle->scale.Set(1.5f, 1.5f, 1.5f);
				particle->pos = player1->pos;
				particle->vel = Vector3(0, 0, 0);
				particle->mass = 10.f;
				particle->ballrotated = 0;
				particle->ticker = 5;
				particle->pos.z = -1;
			}
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
		if (Application::IsKeyPressed('D'))
		{
			player1->rotationAngle -= player1->playerCar.turnSpeed;
			player1->normal = Vector3(cos(Math::DegreeToRadian(player1->rotationAngle)), sin(Math::DegreeToRadian(player1->rotationAngle)), 0);
		}
		if (Application::IsKeyPressed('A'))
		{
			player1->rotationAngle += player1->playerCar.turnSpeed;
			player1->normal = Vector3(cos(Math::DegreeToRadian(player1->rotationAngle)), sin(Math::DegreeToRadian(player1->rotationAngle)), 0);
		}
		if (player1->vel.Length() < player1->playerCar.topSpeed)
		{
			player1->vel += Vector3(cos(Math::DegreeToRadian(player1->rotationAngle)) * player1->engine, sin(Math::DegreeToRadian(player1->rotationAngle)) * player1->engine, 0);
		}
		friction = 0.96f - player1->mass * 0.01f;
		if (player1->vel.x != 0 || player1->vel.y != 0)
		{
			player1->vel = player1->vel * friction;
		}
		if (Application::IsKeyPressed(VK_LSHIFT))
		{
			if (player1poweractive <= 0 && player1power > 0)
			{
				player1power--;
				player1poweractive = 1 / dt;
				player1poweractived = true;
				if (player1->playerCar.powerUp == 2)
				{
					GameObject* saw = FetchGO();
					saw->type = GameObject::GO_SAWBLADE;
					saw->scale.Set(5, 5, 5);
					saw->pos = player1->pos;
					saw->vel = player1->vel + Vector3(1, 1, 0);
					saw->vel = saw->vel.Normalized() * 150.f;
					saw->mass = 10.f;
					saw->ballrotated = 0;
					saw->ticker = (player1power + 1) * 2;
					saw->pos.z = 1;
					player1power = 0;
				}
			}
			if (player1poweractived)
			{
				if (player1->playerCar.powerUp == 1)
				{
					if (player1->vel.Length() < player1->playerCar.topSpeed + player1->playerCar.topSpeed / 2)
						player1->vel += Vector3(cos(Math::DegreeToRadian(player1->rotationAngle)) * player1->engine * 2.5f, sin(Math::DegreeToRadian(player1->rotationAngle)) * player1->engine * 2.5f, 0);
				}
				else if (player1->playerCar.powerUp == 2)
				{

				}
				else if (player1->playerCar.powerUp == 3)
				{
					soccerBall->vel += (player1->pos - soccerBall->pos).Normalized() * (player1power + 1) * 0.5f;
					player1power = 0;
				}
			}
		}

		if (Application::IsKeyPressed(VK_UP))
		{
			player2->engine += player2->playerCar.acceleration;
			if (!UpPressed && player2->active)
			{
				UpPressed = true;
				GameObject* particle = FetchGO();
				particle->type = GameObject::GO_PARTICLE;
				particle->scale.Set(1.5f, 1.5f, 1.5f);
				particle->pos = player2->pos;
				particle->vel = Vector3(0, 0, 0);
				particle->mass = 10.f;
				particle->ballrotated = 0;
				particle->ticker = 5;
				particle->pos.z = -1;
			}
		}
		else if (Application::IsKeyPressed(VK_DOWN))
		{
			player2->engine = -1;
		}
		else
		{
			player2->engine = 0;
		}
		if (player2->engine > player2->playerCar.engine)
		{
			player2->engine = player2->playerCar.engine;
		}
		if (Application::IsKeyPressed(VK_RIGHT))
		{
			player2->rotationAngle -= player2->playerCar.turnSpeed;
			player2->normal = Vector3(cos(Math::DegreeToRadian(player2->rotationAngle)), sin(Math::DegreeToRadian(player2->rotationAngle)), 0);
		}
		if (Application::IsKeyPressed(VK_LEFT))
		{
			player2->rotationAngle += player2->playerCar.turnSpeed;
			player2->normal = Vector3(cos(Math::DegreeToRadian(player2->rotationAngle)), sin(Math::DegreeToRadian(player2->rotationAngle)), 0);
		}
		if (player2->vel.Length() < player2->playerCar.topSpeed)
		{
			player2->vel += Vector3(cos(Math::DegreeToRadian(player2->rotationAngle)) * player2->engine, sin(Math::DegreeToRadian(player2->rotationAngle)) * player2->engine, 0);
		}
		friction = 0.96f - player2->mass * 0.01f;
		if (player2->vel.x != 0 || player2->vel.y != 0)
		{
			player2->vel = player2->vel * friction;
		}
		if (Application::IsKeyPressed(VK_RSHIFT))
		{
			if (player2poweractive <= 0 && player2power > 0)
			{
				player2power--;
				player2poweractive = 1 / dt;
				player2poweractived = true;
				if (player2->playerCar.powerUp == 2)
				{
					GameObject* saw = FetchGO();
					saw->type = GameObject::GO_SAWBLADE;
					saw->scale.Set(5, 5, 5);
					saw->pos = player2->pos;
					saw->vel = player2->vel + Vector3(1, 1, 0);
					saw->vel = saw->vel.Normalized() * 150.f;
					saw->mass = 10.f;
					saw->ballrotated = 0;
					saw->ticker = (player2power + 1) * 2;
					saw->pos.z = 1;
					player2power = 0;
				}
			}
			if (player2poweractived)
			{
				if (player2->playerCar.powerUp == 1)
				{
					if (player2->vel.Length() < player2->playerCar.topSpeed + player2->playerCar.topSpeed / 2)
						player2->vel += Vector3(cos(Math::DegreeToRadian(player2->rotationAngle)) * player2->engine * 2.5f, sin(Math::DegreeToRadian(player2->rotationAngle)) * player2->engine * 2.5f, 0);
				}
				else if (player2->playerCar.powerUp == 2)
				{

				}
				else if (player2->playerCar.powerUp == 3)
				{
					soccerBall->vel += (player2->pos - soccerBall->pos).Normalized() * (player2power + 1) * 0.5f;
					player2power = 0;
				}
			}
		}

	}

	//Mouse Section
	static bool bLButtonState = false;
	if (!bLButtonState && Application::IsMousePressed(0))
	{
		bLButtonState = true;
		std::cout << "LBUTTON DOWN" << std::endl;
		double x, y;
		Application::GetCursorPos(&x, &y);
		int w = 1366;
		int h = 768;
		float worldX = x * m_worldWidth / w;
		float worldY = (h - y) * m_worldHeight / h;
		m_ghost->pos.Set(worldX, worldY, 0);
		m_ghost->active = true;
	}
	else if (bLButtonState && !Application::IsMousePressed(0))
	{
		bLButtonState = false;
		std::cout << "LBUTTON UP" << std::endl;

		//Exercise 6: spawn small GO_BALL
		double x, y;
		Application::GetCursorPos(&x, &y);
		int w = 1366;
		int h = 768;
		float worldX = x * m_worldWidth / w;
		float worldY = (h - y) * m_worldHeight / h;
		m_ghost->active = false;

		GameObject* ball = FetchGO();
		ball->type = GameObject::GO_BALL;
		Vector3 size = Vector3(worldX, worldY, 0) - m_ghost->pos;
		float radius = Math::Clamp(size.Length(), 1.f, 1.f);
		ball->scale.Set(radius, radius, radius);
		ball->pos = m_ghost->pos;
		ball->vel = m_ghost->pos - Vector3(worldX, worldY, 0);
		ball->mass = radius;
		ball->ballrotated = 0;
		m_ghost->active = false;

		m_estimatedTime = -1;
	}
	static bool bRButtonState = false;
	if (!bRButtonState && Application::IsMousePressed(1))
	{
		bRButtonState = true;
		std::cout << "RBUTTON DOWN" << std::endl;
		double x, y;
		Application::GetCursorPos(&x, &y);
		int w = 1366;
		int h = 768;
		float worldX = x * m_worldWidth / w;
		float worldY = (h - y) * m_worldHeight / h;
		m_ghost->pos.Set(worldX, worldY, 0);
	}
	else if (bRButtonState && !Application::IsMousePressed(1))
	{
		bRButtonState = false;
		std::cout << "RBUTTON UP" << std::endl;

		//Exercise 10: spawn large GO_BALL
		double x, y;
		Application::GetCursorPos(&x, &y);
		int w = 1366;
		int h = 768;
		float worldX = x * m_worldWidth / w;
		float worldY = (h - y) * m_worldHeight / h;

		GameObject* ball = FetchGO();
		ball->type = GameObject::GO_BALL;
		ball->scale.Set(3.5f, 3.5f, 3.5f);
		ball->pos = m_ghost->pos;
		ball->vel = m_ghost->pos - Vector3(worldX, worldY, 0);
		ball->mass = 1.5f * 1.5f * 1.5f;
		ball->ballrotated = 0;
		m_ghost->active = false;
	}

	if (m_ghost->active)
	{
		double x, y;
		Application::GetCursorPos(&x, &y);
		int w = 1366;
		int h = 768;
		float worldX = x * m_worldWidth / w;
		float worldY = (h - y) * m_worldHeight / h;
		Vector3 size = Vector3(worldX, worldY, 0) - m_ghost->pos;
		float radius = Math::Clamp(size.Length(), 0.1f, 1.f);
		m_ghost->scale.Set(radius, radius, 1.0f);
	}

	debugscale *= 0.9f;
	if (debugscale.x < 0.1f)
		debugactive = false;

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
					if (go == soccerBall)
					{
						m_ballCount--;
					}
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
					if (go == soccerBall)
					{
						m_ballCount--;
					}
						
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

				if (go->pos.y > 90)
				{
					if (go->vel.y > 0)
						go->vel.y *= -1;
					go->pos.y -= go->pos.y - 90;
				}
				else if (go->pos.y < 10)
				{
					if (go->vel.y < 0)
						go->vel.y *= -1;
					go->pos.y += 10 - go->pos.y;
				}
				else if (go->pos.y > 60 || go->pos.y < 40)
				{
					if (go->pos.x < 15)
					{
						if (go->vel.x < 0)
							go->vel.x *= -1;
						go->pos.x += 15 - go->pos.x;
					}
					if (go->pos.x > 115)
					{
						if (go->vel.x > 0)
							go->vel.x *= -1;
						go->pos.x -= go->pos.x - 115;
					}
				}
			}
				//Exercise 8a: handle collision between GO_BALL and GO_BALL using velocity swap

			for (std::vector<GameObject *>::iterator it2 = it ; it2 != m_goList.end(); ++it2)
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

			if (go == playerAI)
			{
				// AI code
				playerAI->engine += playerAI->playerCar.acceleration;

				if (playerAI->engine > playerAI->playerCar.engine)
				{
					playerAI->engine = playerAI->playerCar.engine;
				}
				if (go->rotationAngle < 0)
					go->rotationAngle += 360;
				if (go->rotationAngle > 360)
					go->rotationAngle -= 360;

				float b1, b2;
				for (std::vector<GameObject *>::iterator it2 = m_goList.begin(); it2 != m_goList.end(); ++it2)
				{
					GameObject *other = (GameObject*)*it2;
					if (other->type == GameObject::GO_BALL)
					{
						if (go->active == false)
							continue;
						if (other->pos.x == 0)
							continue;

						float newX = other->pos.x + (player1goal->pos - other->pos).Normalized().x;
						float newY = other->pos.y + (player1goal->pos - other->pos).Normalized().y;

						float angleDiff = Math::RadianToDegree(atan2((newY - go->pos.y), (newX - go->pos.x)));
						if (angleDiff < 0)
							angleDiff += 360;

						if ((angleDiff - go->rotationAngle  > 0 && angleDiff - go->rotationAngle < 180) || (go->rotationAngle > 180 && go->rotationAngle - angleDiff > 180))
						{
							playerAI->rotationAngle += playerAI->playerCar.turnSpeed;
							playerAI->normal = Vector3(cos(Math::DegreeToRadian(playerAI->rotationAngle)), sin(Math::DegreeToRadian(playerAI->rotationAngle)), 0);
						}
						else if ((angleDiff - go->rotationAngle  < 0 && angleDiff - go->rotationAngle < 180) || (go->rotationAngle < 180 && go->rotationAngle - angleDiff < 180))
						{
							playerAI->rotationAngle -= playerAI->playerCar.turnSpeed;
							playerAI->normal = Vector3(cos(Math::DegreeToRadian(playerAI->rotationAngle)), sin(Math::DegreeToRadian(playerAI->rotationAngle)), 0);
						}
					}
				}

				if (playerAI->vel.Length() < playerAI->playerCar.topSpeed)
				{
					playerAI->vel += Vector3(cos(Math::DegreeToRadian(playerAI->rotationAngle)) * playerAI->engine, sin(Math::DegreeToRadian(playerAI->rotationAngle)) * playerAI->engine, 0);
				}
				if (playerAI->vel.x != 0 || playerAI->vel.y != 0)
				{
					playerAI->vel = playerAI->vel * friction;
				}

				if (player2poweractive <= 0 && player2power > 0)
				{
					player2power--;
					player2poweractive = 1 / dt;
					player2poweractived = true;
				}
				if (player2poweractived)
				{
					if (playerAI->playerCar.powerUp == 1)
					{
						if (playerAI->vel.Length() < playerAI->playerCar.topSpeed + playerAI->playerCar.topSpeed / 3)
							playerAI->vel += Vector3(cos(Math::DegreeToRadian(playerAI->rotationAngle)) * playerAI->engine * 2.5f, sin(Math::DegreeToRadian(playerAI->rotationAngle)) * playerAI->engine * 2.5f, 0);
					}
				}

			}
			
			if (go->type == GameObject::GO_CAR)
			{
				if (!go->active)
					continue;
				if (loading)
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
						if (go == player1 && player1power >= 5)
						{
							player1power = 5;
							continue;
						}
						if ((go == player2 || go == playerAI) && player2power >= 5)
						{
							player2power = 5;
							continue;
						}
						bool checkCollide = carballCollide(other, go, dt);
						if (checkCollide)
						{
							other->active = false;
							other->ticker = 10;
							if (go == player1)
							{
								player1power++;
							}
							else
							{
								player2power++;
							}
						}
					}
					if (other->type == GameObject::GO_SAWBLADE)
					{
						if (!other->active)
							continue;
						bool checkCollide = carballCollide(other, go, dt);
						if (checkCollide)
						{
							go->vel = 0;
							go->rotationAngle = Math::RandFloatMinMax(-5, 5) + go->rotationAngle;
						}
					}
				}
			}

			if (go->type == GameObject::GO_SAWBLADE)
			{
				if (go->pos.x > m_worldWidth - go->scale.x)
				{
					go->ticker--;
					if (go->vel.x > 0)
						go->vel.x = -go->vel.x;
				}
				else if (go->pos.x < 0 + go->scale.x)
				{
					go->ticker--;
					if (go->vel.x < 0)
						go->vel.x = -go->vel.x;
				}
				if (go->pos.y > m_worldHeight - go->scale.y)
				{
					go->ticker--;
					if (go->vel.y > 0)
						go->vel.y = -go->vel.y;
				}
				else if (go->pos.y < 0 + go->scale.y)
				{
					go->ticker--;
					if (go->vel.y < 0)
						go->vel.y = -go->vel.y;
				}
				if (go->ticker <= 0)
					go->active = false;
			}

			if (go->type == GameObject::GO_PARTICLE)
			{
				go->scale *= 0.9f;
				if (go->scale.x < 0.1f)
				{
					go->active = false;
				}
			}

			//Exercise 10: handle collision using momentum swap instead

			//Exercise 12: improve inner loop to prevent double collision

			//Exercise 13: improve collision detection algorithm [solution to be given later] 
		}
	}
}

bool SceneAssignment2::spawnMap()
{
	GameObject* wall = FetchGO();
	wall->type = GameObject::GO_WALL;
	wall->pos.Set(65, 90, 1);
	wall->normal = Vector3(0, -1, 0);
	wall->active = true;
	wall->scale.Set(2, 100, 1);
	wall->rotationAngle = 90;

	wall = FetchGO();
	wall->type = GameObject::GO_WALL;
	wall->pos.Set(65, 10, 1);
	wall->normal = Vector3(0, 1, 0);
	wall->active = true;
	wall->scale.Set(2, 100, 1);
	wall->rotationAngle = 90;

	// right wall
	wall = FetchGO();
	wall->type = GameObject::GO_WALL;
	wall->pos.Set(115, 76, 1);
	wall->normal = Vector3(1, 0, 0);
	wall->active = true;
	wall->scale.Set(2, 30, 1);
	wall->rotationAngle = 0;

	wall = FetchGO();
	wall->type = GameObject::GO_WALL;
	wall->pos.Set(115, 24, 1);
	wall->normal = Vector3(1, 0, 0);
	wall->active = true;
	wall->scale.Set(2, 30, 1);
	wall->rotationAngle = 0;

	//right goal
	wall = FetchGO();
	wall->type = GameObject::GO_WALL;
	wall->pos.Set(125, 50, 1);
	wall->normal = Vector3(1, 0, 0);
	wall->active = true;
	wall->scale.Set(2, 20, 1);
	wall->rotationAngle = 0;

	wall = FetchGO();
	wall->type = GameObject::GO_WALL;
	wall->pos.Set(120, 40, 1);
	wall->normal = Vector3(0, 1, 0);
	wall->active = true;
	wall->scale.Set(2, 12, 1);
	wall->rotationAngle = 90;

	wall = FetchGO();
	wall->type = GameObject::GO_WALL;
	wall->pos.Set(120, 60, 1);
	wall->normal = Vector3(0, -1, 0);
	wall->active = true;
	wall->scale.Set(2, 12, 1);
	wall->rotationAngle = 90;

	wall = FetchGO();
	wall->type = GameObject::GO_PILLAR;
	wall->pos.Set(115, 40, 1);
	wall->normal = Vector3(0, 1, 0);
	wall->active = true;
	wall->scale.Set(1.3f, 1.3f, 1.3f);
	wall->rotationAngle = 90;

	wall = FetchGO();
	wall->type = GameObject::GO_PILLAR;
	wall->pos.Set(115, 60, 1);
	wall->normal = Vector3(0, 1, 0);
	wall->active = true;
	wall->scale.Set(1.3f, 1.3f, 1.3f);
	wall->rotationAngle = 90;

	//left wall
	wall = FetchGO();
	wall->type = GameObject::GO_WALL;
	wall->pos.Set(15, 76, 1);
	wall->normal = Vector3(-1, 0, 0);
	wall->active = true;
	wall->scale.Set(2, 30, 1);
	wall->rotationAngle = 0;

	wall = FetchGO();
	wall->type = GameObject::GO_WALL;
	wall->pos.Set(15, 24, 1);
	wall->normal = Vector3(-1, 0, 0);
	wall->active = true;
	wall->scale.Set(2, 30, 1);
	wall->rotationAngle = 0;

	//left goal
	wall = FetchGO();
	wall->type = GameObject::GO_WALL;
	wall->pos.Set(5, 50, 1);
	wall->normal = Vector3(1, 0, 0);
	wall->active = true;
	wall->scale.Set(2, 20, 1);
	wall->rotationAngle = 0;

	wall = FetchGO();
	wall->type = GameObject::GO_WALL;
	wall->pos.Set(10, 40, 1);
	wall->normal = Vector3(0, 1, 0);
	wall->active = true;
	wall->scale.Set(2, 12, 1);
	wall->rotationAngle = 90;

	wall = FetchGO();
	wall->type = GameObject::GO_WALL;
	wall->pos.Set(10, 60, 1);
	wall->normal = Vector3(0, -1, 0);
	wall->active = true;
	wall->scale.Set(2, 12, 1);
	wall->rotationAngle = 90;

	wall = FetchGO();
	wall->type = GameObject::GO_PILLAR;
	wall->pos.Set(15, 40, 1);
	wall->normal = Vector3(0, 1, 0);
	wall->active = true;
	wall->scale.Set(1.3f, 1.3f, 1.3f);
	wall->rotationAngle = 90;

	wall = FetchGO();
	wall->type = GameObject::GO_PILLAR;
	wall->pos.Set(15, 60, 1);
	wall->normal = Vector3(0, 1, 0);
	wall->active = true;
	wall->scale.Set(1.3f, 1.3f, 1.3f);
	wall->rotationAngle = 90;

	//corner
	wall = FetchGO();
	wall->type = GameObject::GO_WALL;
	wall->pos.Set(112.5f, 88, 1);
	//wall->pos.Set(50, 50, 1);
	wall->normal = Vector3(0.5f, 0.5f, 0).Normalized();
	wall->active = true;
	wall->scale.Set(2, 6, 1);
	wall->rotationAngle = 45;

	wall = FetchGO();
	wall->type = GameObject::GO_WALL;
	wall->pos.Set(17.5f, 88, 1);
	//wall->pos.Set(50, 50, 1);
	wall->normal = Vector3(0.5f, -0.5f, 0).Normalized();
	wall->active = true;
	wall->scale.Set(2, 6, 1);
	wall->rotationAngle = -45;

	wall = FetchGO();
	wall->type = GameObject::GO_WALL;
	wall->pos.Set(17.5f, 12, 1);
	//wall->pos.Set(50, 50, 1);
	wall->normal = Vector3(0.5f, 0.5f, 0).Normalized();
	wall->active = true;
	wall->scale.Set(2, 6, 1);
	wall->rotationAngle = 45;

	wall = FetchGO();
	wall->type = GameObject::GO_WALL;
	wall->pos.Set(112.5f, 12, 1);
	//wall->pos.Set(50, 50, 1);
	wall->normal = Vector3(-0.5f, 0.5f, 0).Normalized();
	wall->active = true;
	wall->scale.Set(2, 6, 1);
	wall->rotationAngle = -45;

	return true;
}

void SceneAssignment2::RenderGO(GameObject *go)
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
		RenderMesh(meshList[GEO_SOCC], true);
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
		modelStack.PushMatrix();
		modelStack.Translate(go->pos.x, go->pos.y, go->pos.z);
		modelStack.Rotate(go->rotationAngle, 0, 0, 1);
		modelStack.Rotate(180, 0, 0, 1);
		modelStack.Rotate(90, 1, 0, 0);
		modelStack.Scale(go->scale.x / 10, go->scale.y / 5, go->scale.z);
		modelStack.Scale(2.5, 2.5, 2.5);
		if (go == player1)
			RenderMesh(meshList[GEO_CAR1], false);
		if (go == player2)
			RenderMesh(meshList[GEO_CAR2], false);
		if (go == playerAI)
			RenderMesh(meshList[GEO_CAR2], false);
		modelStack.PopMatrix();
		
		if (debugactive)
		{
			modelStack.PushMatrix();
			modelStack.Translate(debug.x, debug.y, debug.z);
			modelStack.Scale(debugscale.x, debugscale.y, debugscale.z);
			if (debugcolor == 1)
				RenderMesh(meshList[GEO_POST1], false);
			if (debugcolor == 2)
				RenderMesh(meshList[GEO_POST2], false);
			else
				RenderMesh(meshList[GEO_BALL], false);
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
		modelStack.Rotate(internationalDT * 25.f, 0, 0, 1);
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

void SceneAssignment2::menuRender()
{
	if (!gameModeSelected)
	{
		std::ostringstream ssq;
		ssq.precision(5);
		ssq << "SELECT NUMBER";
		RenderTextOnScreen(meshList[GEO_TEXT], ssq.str(), Color(0, 1, 0), 3, 21, 50);

		ssq.str("");
		ssq.precision(5);
		ssq << "OF PLAYERS";
		RenderTextOnScreen(meshList[GEO_TEXT], ssq.str(), Color(0, 1, 0), 3, 26, 47);

		ssq.str("");
		ssq.precision(5);
		ssq << "< " << numOfPlayers << " >";
		RenderTextOnScreen(meshList[GEO_TEXT], ssq.str(), Color(0, 1, 0), 3, 33, 40);

		ssq.str("");
		ssq.precision(5);
		ssq << "PRESS ENTER TO START";
		RenderTextOnScreen(meshList[GEO_TEXT], ssq.str(), Color(0, 1, 0), 2, 21, 7);
	}
	else
	{
		if (numOfPlayers == 1)
		{
			std::ostringstream ssq;
			ssq.precision(5);
			ssq << "SINGLE-PLAYER";
			RenderTextOnScreen(meshList[GEO_TEXT], ssq.str(), Color(0, 1, 0), 3, 21, 50);

			ssq.str("");
			ssq.precision(5);
			ssq << "MODE";
			RenderTextOnScreen(meshList[GEO_TEXT], ssq.str(), Color(0, 1, 0), 3, 35, 47);

			ssq.str("");
			ssq.precision(5);
			ssq << "SELECT CAR";
			RenderTextOnScreen(meshList[GEO_TEXT], ssq.str(), Color(1, 1, 1), 3, 26, 42);

			ssq.str("");
			ssq.precision(5);
			ssq << player1->playerCar.name;
			RenderTextOnScreen(meshList[GEO_TEXT], ssq.str(), Color(1, 144.f / 255.f, 0), 3, 14, 37);

			ssq.str("");
			ssq.precision(5);
			ssq << "Engine: " << player1->playerCar.engine;
			RenderTextOnScreen(meshList[GEO_TEXT], ssq.str(), Color(1, 1, 1), 3, 14, 34);
		
			ssq.str("");
			ssq.precision(5);
			ssq << "Max Speed: " << player1->playerCar.topSpeed;
			RenderTextOnScreen(meshList[GEO_TEXT], ssq.str(), Color(1, 1, 1), 3, 14, 31);

			ssq.str("");
			ssq.precision(5);
			ssq << "Turn Power: " << player1->playerCar.turnSpeed;
			RenderTextOnScreen(meshList[GEO_TEXT], ssq.str(), Color(1, 1, 1), 3, 14, 28);

			ssq.str("");
			ssq.precision(5);
			ssq << "Mass: " << player1->playerCar.mass;
			RenderTextOnScreen(meshList[GEO_TEXT], ssq.str(), Color(1, 1, 1), 3, 14, 25);

			ssq.str("");
			ssq.precision(2);
			ssq << "Acceleration: " << player1->playerCar.acceleration;
			RenderTextOnScreen(meshList[GEO_TEXT], ssq.str(), Color(1, 1, 1), 3, 14, 22);

			ssq.str("");
			ssq.precision(5);
			ssq << "Power: ";
			if (player1->playerCar.powerUp == 1)
			{
				ssq << "Boost";
			}
			else if (player1->playerCar.powerUp == 2)
			{
				ssq << "Sawblade";
			}
			else if (player1->playerCar.powerUp == 3)
			{
				ssq << "Magnetism";
			}
			RenderTextOnScreen(meshList[GEO_TEXT], ssq.str(), Color(1, 1, 1), 3, 14, 19);

			ssq.str("");
			ssq.precision(5);
			ssq << "PRESS LSHIFT TO START";
			RenderTextOnScreen(meshList[GEO_TEXT], ssq.str(), Color(0, 1, 0), 2, 20, 7);
		}
		else if (numOfPlayers == 2)
		{
			std::ostringstream ssq;
			ssq.precision(5);
			ssq << "TWO-PLAYER";
			RenderTextOnScreen(meshList[GEO_TEXT], ssq.str(), Color(0, 1, 0), 3, 25, 50);

			ssq.str("");
			ssq.precision(5);
			ssq << "MODE";
			RenderTextOnScreen(meshList[GEO_TEXT], ssq.str(), Color(0, 1, 0), 3, 35, 47);

			ssq.str("");
			ssq.precision(5);
			ssq << "SELECT CAR";
			RenderTextOnScreen(meshList[GEO_TEXT], ssq.str(), Color(1, 1, 1), 3, 26, 42);

			if (!player1ready)
			{
				ssq.str("");
				ssq.precision(5);
				ssq << "PLAYER1 SELECT";
				RenderTextOnScreen(meshList[GEO_TEXT], ssq.str(), Color(1, 144.f / 255.f, 0), 2, 27, 45);

				ssq.str("");
				ssq.precision(5);
				ssq << player1->playerCar.name;
				RenderTextOnScreen(meshList[GEO_TEXT], ssq.str(), Color(1, 144.f / 255.f, 0), 3, 14, 37);

				ssq.str("");
				ssq.precision(5);
				ssq << "Engine: " << player1->playerCar.engine;
				RenderTextOnScreen(meshList[GEO_TEXT], ssq.str(), Color(1, 1, 1), 3, 14, 34);

				ssq.str("");
				ssq.precision(5);
				ssq << "Max Speed: " << player1->playerCar.topSpeed;
				RenderTextOnScreen(meshList[GEO_TEXT], ssq.str(), Color(1, 1, 1), 3, 14, 31);

				ssq.str("");
				ssq.precision(5);
				ssq << "Turn Power: " << player1->playerCar.turnSpeed;
				RenderTextOnScreen(meshList[GEO_TEXT], ssq.str(), Color(1, 1, 1), 3, 14, 28);

				ssq.str("");
				ssq.precision(5);
				ssq << "Mass: " << player1->playerCar.mass;
				RenderTextOnScreen(meshList[GEO_TEXT], ssq.str(), Color(1, 1, 1), 3, 14, 25);

				ssq.str("");
				ssq.precision(2);
				ssq << "Acceleration: " << player1->playerCar.acceleration;
				RenderTextOnScreen(meshList[GEO_TEXT], ssq.str(), Color(1, 1, 1), 3, 14, 22);

				ssq.str("");
				ssq.precision(5);
				ssq << "Power: ";
				if (player1->playerCar.powerUp == 1)
				{
					ssq << "Boost";
				}
				else if (player1->playerCar.powerUp == 2)
				{
					ssq << "Sawblade";
				}
				else if (player1->playerCar.powerUp == 3)
				{
					ssq << "Magnetism";
				}
				RenderTextOnScreen(meshList[GEO_TEXT], ssq.str(), Color(1, 1, 1), 3, 14, 19);

				ssq.str("");
				ssq.precision(5);
				ssq << "PRESS LSHIFT TO READY";
				RenderTextOnScreen(meshList[GEO_TEXT], ssq.str(), Color(0, 1, 0), 2, 20, 7);
			}
			else if (!player2ready)
			{
				ssq.str("");
				ssq.precision(5);
				ssq << "PLAYER2 SELECT";
				RenderTextOnScreen(meshList[GEO_TEXT], ssq.str(), Color(1, 0, 238.f / 255.f), 2, 27, 45);

				ssq.str("");
				ssq.precision(5);
				ssq << player2->playerCar.name;
				RenderTextOnScreen(meshList[GEO_TEXT], ssq.str(), Color(1, 0, 238.f / 255.f), 3, 14, 37);

				ssq.str("");
				ssq.precision(5);
				ssq << "Engine: " << player2->playerCar.engine;
				RenderTextOnScreen(meshList[GEO_TEXT], ssq.str(), Color(1, 1, 1), 3, 14, 34);

				ssq.str("");
				ssq.precision(5);
				ssq << "Max Speed: " << player2->playerCar.topSpeed;
				RenderTextOnScreen(meshList[GEO_TEXT], ssq.str(), Color(1, 1, 1), 3, 14, 31);

				ssq.str("");
				ssq.precision(5);
				ssq << "Turn Power: " << player2->playerCar.turnSpeed;
				RenderTextOnScreen(meshList[GEO_TEXT], ssq.str(), Color(1, 1, 1), 3, 14, 28);

				ssq.str("");
				ssq.precision(5);
				ssq << "Mass: " << player2->playerCar.mass;
				RenderTextOnScreen(meshList[GEO_TEXT], ssq.str(), Color(1, 1, 1), 3, 14, 25);

				ssq.str("");
				ssq.precision(2);
				ssq << "Acceleration: " << player2->playerCar.acceleration;
				RenderTextOnScreen(meshList[GEO_TEXT], ssq.str(), Color(1, 1, 1), 3, 14, 22);

				ssq.str("");
				ssq.precision(5);
				ssq << "Power: ";
				if (player2->playerCar.powerUp == 1)
				{
					ssq << "Boost";
				}
				else if (player2->playerCar.powerUp == 2)
				{
					ssq << "Sawblade";
				}
				else if (player2->playerCar.powerUp == 3)
				{
					ssq << "Magnetism";
				}
				RenderTextOnScreen(meshList[GEO_TEXT], ssq.str(), Color(1, 1, 1), 3, 14, 19);

				ssq.str("");
				ssq.precision(5);
				ssq << "PRESS RSHIFT TO START";
				RenderTextOnScreen(meshList[GEO_TEXT], ssq.str(), Color(0, 1, 0), 2, 20, 7);
			}
		}
	}
}

void SceneAssignment2::Render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//Calculating aspect ratio
	m_worldHeight = 100.f;
	m_worldWidth = m_worldHeight * (float)Application::GetWindowWidth() / Application::GetWindowHeight();

	// Projection matrix : Orthographic Projection
	Mtx44 projection;
	projection.SetToOrtho(0, m_worldWidth, 0, m_worldHeight, -10, 10);
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

	if (mainMenu)
		menuRender();

	for (std::vector<GameObject *>::iterator it = m_goList.begin(); it != m_goList.end(); ++it)
	{
		GameObject *go = (GameObject *)*it;
		if (go->active)
		{
			RenderGO(go);
		}
	}

	if (m_ghost->active)
	{
		modelStack.PushMatrix();
		modelStack.Translate(m_ghost->pos.x, m_ghost->pos.y, m_ghost->pos.z);
		modelStack.Scale(m_ghost->scale.x, m_ghost->scale.y, m_ghost->scale.z);
		RenderMesh(meshList[GEO_BALL], false);
		modelStack.PopMatrix();
	}

	modelStack.PushMatrix();
	modelStack.Translate(6, 94, 0);
	modelStack.Scale(4, 4, 1);
	RenderMesh(meshList[GEO_POWERUP], false);
	modelStack.PopMatrix();

	if (Application::IsKeyPressed('Q') && !mainMenu)
	{
		modelStack.PushMatrix();
		modelStack.Translate(soccerBall->pos.x, soccerBall->pos.y, 0);
		modelStack.Scale(6, 6, 25);
		RenderMesh(meshList[GEO_ICE], false);
		modelStack.PopMatrix();
	}

	std::ostringstream ssa;
	ssa.precision(5);
	ssa << player1power;
	RenderTextOnScreen(meshList[GEO_TEXT], ssa.str(), Color(0, 0, 1), 5, 2.6f, 54);

	modelStack.PushMatrix();
	modelStack.Translate(m_worldWidth - 6, 94, 0);
	modelStack.Scale(4, 4, 1);
	RenderMesh(meshList[GEO_POWERUP], false);
	modelStack.PopMatrix();

	ssa.str("");
	ssa.precision(5);
	ssa << player2power;
	RenderTextOnScreen(meshList[GEO_TEXT], ssa.str(), Color(0, 0, 1), 5, 75.5f, 54);

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

	std::ostringstream ss2;
	ss2.str("");
	ss2.precision(3);
	ss2 << player1score << " - " << player2score;
	RenderTextOnScreen(meshList[GEO_TEXT], ss2.str(), Color(1, 1, 1), 3, 33, 55);

	ss2.str("");
	ss2.precision(3);
	if (second3 > 9)
	{
		ss2 << min3 << ":" << second3;
		RenderTextOnScreen(meshList[GEO_TEXT], ss2.str(), Color(0.8f, 0.8f, 1), 2.3f, 36, 58);
	}
	else if (min3 || second3)
	{
		ss2 << min3 << ":0" << second3;
		RenderTextOnScreen(meshList[GEO_TEXT], ss2.str(), Color(0.8f, 0.8f, 1), 2.3f, 36, 58);
	}
	else
	{
		ss2 << "OVERTIME";
		RenderTextOnScreen(meshList[GEO_TEXT], ss2.str(), Color(1, 0.8f, 0.8f), 2.3f, 31.5f, 58);
	}
	

	if (gameStart == false && min3 <= 0 && second3 <= 0 && abs(player1score - player2score))
	{
		ss2.str("");
		ss2.precision(3);
		if (player1score > player2score)
		{
			ss2 << " PLAYER1 WINS";
			RenderTextOnScreen(meshList[GEO_TEXT], ss2.str(), Color(255, 144, 0), 3, 19, 50);
		}
		else if (player1score < player2score)
		{
			if (player2->active)
				ss2 << " PLAYER2 WINS";
			if (playerAI->active)
				ss2 << "COMPUTER WINS";
			RenderTextOnScreen(meshList[GEO_TEXT], ss2.str(), Color(255, 0, 228), 3, 19, 50);
		}
		else
		{
			ss2 << "     DRAW    ";
			RenderTextOnScreen(meshList[GEO_TEXT], ss2.str(), Color(0, 255, 0), 3, 19, 50);
		}
	}

	double x, y;
	Application::GetCursorPos(&x, &y);
	int w = Application::GetWindowWidth();
	int h = Application::GetWindowHeight();
	float worldX = x * m_worldWidth / w;
	float worldY = (h - y) * m_worldHeight / h;

	//std::ostringstream ss;
	//ss.precision(5);
	//ss << "Pos: " << worldX << ", " << worldY;
	//RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 3, 0, 3);

	std::ostringstream ssz;
	ssz.precision(5);
	ssz << "FPS: " << fps;
	RenderTextOnScreen(meshList[GEO_TEXT], ssz.str(), Color(0, 1, 0), 3, 0, 0);

	//RenderTextOnScreen(meshList[GEO_TEXT], "Collision", Color(0, 1, 0), 3, 0, 0);
}

void SceneAssignment2::Exit()
{
	SceneBase::Exit();
	//Cleanup GameObjects
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