#include "SceneSP3.h"
#include "GL\glew.h"
#include "Application.h"
#include <sstream>

SceneSP3::SceneSP3()
{
}

SceneSP3::~SceneSP3()
{
}

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

	friction = 0.95f;

	

	mapPosition = Vector3(m_worldWidth / 2, m_worldHeight / 2, 0);
	testMap.setBackground(meshList[GEO_TESTMAP]);
	testMap.setMapSize(20, 20);
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

GameObject* SceneSP3::FetchGO()
{
	//Exercise 2a: implement FetchGO()

	for (std::vector<GameObject*>::iterator iter = m_goList.begin(); iter != m_goList.end(); ++iter)
	{
		GameObject *go = *iter;
		if (go->active == false && (go->type == GameObject::GO_BALL || go->type == GameObject::GO_PARTICLE))
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
		float combinedRadiusSquared = (go->scale.x / 3 + other->scale.x) * (go->scale.x / 3 + other->scale.x);
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
			float combinedRadiusSquared = (go->scale.x / 3 + other->scale.x) * (go->scale.x / 3 + other->scale.x);
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
			float combinedRadiusSquared = (go->scale.x / 3 + other->scale.x) * (go->scale.x / 3 + other->scale.x);
			Vector3 relativeDisplacement = other->pos - go->pos;
			if (distanceSquared < combinedRadiusSquared && go->vel.Dot(relativeDisplacement) > 0)
			{
				friction = 0.99f;
				go->vel *= 0.98f;
			}
			break;
	}
	}
}

void SceneSP3::Update(double dt)
{
	SceneBase::Update(dt);
	

	float diffx = (m_worldWidth / 2) - player1->pos.x;
	float diffy = (m_worldHeight / 2) - player1->pos.y;
	mapPosition = Vector3(mapPosition.x + diffx, mapPosition.y + diffy, 1);
	player1->pos.x = (m_worldWidth / 2);
	player1->pos.y = (m_worldHeight / 2);

	static bool WPressed = false;
	static bool UpPressed = false;

	//std::cout<<zebra->getPos()<<std::endl;

	if (Application::IsKeyPressed('E'))
	{
		enemy* animal = newEnemy(30, 10, 0);
		enemyList.push_back(animal);
	}

	if (Application::IsKeyPressed('R'))
	{
		GameObject* testTree = new GameObject(GameObject::MAP_TREE);
		testTree->pos.Set(0, 0, 1);
		testTree->fresh = true;
		testTree->active = true;
		testMap.addSingleProp(testTree);
		TextFile * hi = new TextFile(TextFile::CAR);
		hi->SetData("tree", testTree->pos.x, testTree->pos.y, testTree->scale.x);
		hi->WriteFile("carz.txt");

		GameObject* testWater = new GameObject(GameObject::MAP_WATER);
		testWater->pos.Set(25, 25, 1);
		testWater->fresh = true;
		testWater->active = true;
		testMap.addSingleProp(testWater);

		GameObject* testMud = new GameObject(GameObject::MAP_MUD);
		testMud->pos.Set(-25 ,-25, 1);
		testMud->fresh = true;
		testMud->active = true;
		testMap.addSingleProp(testMud);

		GameObject* testRock = new GameObject(GameObject::MAP_ROCK);
		testRock->pos.Set(-25, 25, 1);
		testRock->fresh = true;
		testRock->active = true;
		testMap.addSingleProp(testRock);
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
		if (player1->vel.Length() < 3)
		{
			if (1 / player1->vel.Length() < player1->playerCar.turnSpeed)
				player1->rotationAngle += 1 / player1->vel.Length();
			else
				player1->rotationAngle += player1->playerCar.turnSpeed;
		}
		player1->normal = Vector3(cos(Math::DegreeToRadian(player1->rotationAngle)), sin(Math::DegreeToRadian(player1->rotationAngle)), 0);
	}
	if (Application::IsKeyPressed('A'))
	{
		player1->rotationAngle += player1->playerCar.turnSpeed;
		if (player1->vel.Length() < 3)
		{
			if (1 / player1->vel.Length() < player1->playerCar.turnSpeed)
				player1->rotationAngle -= 1 / player1->vel.Length();
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

	//std::cout << player1->pos << std::endl;

	if (player1->vel.x != 0 || player1->vel.y != 0)
	{
		player1->vel = player1->vel * friction;
	}

	//Mouse Section
	static bool bLButtonState = false;
	if (!bLButtonState && Application::IsMousePressed(0))
	{
		bLButtonState = true;
		std::cout << "LBUTTON DOWN" << std::endl;
		double x, y;
		Application::GetCursorPos(&x, &y);
		int w = Application::GetWindowWidth();
		int h = Application::GetWindowHeight();
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
		int w = Application::GetWindowWidth();
		int h = Application::GetWindowHeight();
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
		int w = Application::GetWindowWidth();
		int h = Application::GetWindowHeight();
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
		int w = Application::GetWindowWidth();
		int h = Application::GetWindowHeight();
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
		int w = Application::GetWindowWidth();
		int h = Application::GetWindowHeight();
		float worldX = x * m_worldWidth / w;
		float worldY = (h - y) * m_worldHeight / h;
		Vector3 size = Vector3(worldX, worldY, 0) - m_ghost->pos;
		float radius = Math::Clamp(size.Length(), 0.1f, 1.f);
		m_ghost->scale.Set(radius, radius, 1.0f);
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

			//Exercise 10: handle collision using momentum swap instead

			//Exercise 12: improve inner loop to prevent double collision

			//Exercise 13: improve collision detection algorithm [solution to be given later] 
		}
	}

	for (std::vector<GameObject *>::iterator it = testMap.mapProps.begin(); it != testMap.mapProps.end(); ++it)
	{
		GameObject *go = (GameObject *)*it;
		if (go->active == false)
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
			CollisionMap(player1, go, dt);
 		}
	}

	for (std::vector<enemy*>::iterator it = enemyList.begin(); it != enemyList.end(); ++it)
	{
		enemy *go = (enemy *)*it;
		if (go->getNewSpawn() == 1)
		{
			go->setPos(go->getPos().x + mapPosition.x, go->getPos().y + mapPosition.y, 2);
			go->setNewSpawn(false);
		}
		else
		{
			go->setPos(go->getPos().x + diffx, go->getPos().y + diffy, 2);
		}
		if ((go->getPos() - player1->pos).LengthSquared() < 1000)
		{
			go->runOff(player1->pos);
		}
		else
		{
			go->setVel(0, 0, 0);
		}
		std::cout << go->getPos() << std::endl;
		go->updatePos(dt);
	}

}


	//corner
	//wall = FetchGO();
	//wall->type = GameObject::GO_WALL;
	//wall->pos.Set(112.5f, 88, 1);
	//wall->normal = Vector3(0.5f, 0.5f, 0).Normalized();
	//wall->active = true;
	//wall->scale.Set(2, 6, 1);
	//wall->rotationAngle = 45;


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
		modelStack.PushMatrix();
		modelStack.Translate(go->pos.x, go->pos.y, go->pos.z);
		modelStack.Rotate(go->rotationAngle, 0, 0, 1);
		modelStack.Rotate(180, 0, 0, 1);
		modelStack.Rotate(90, 1, 0, 0);
		modelStack.Scale(go->scale.x / 10, go->scale.y / 5, go->scale.z / 2);
		modelStack.Scale(2.5, 2.5, 2.5);
		RenderMesh(meshList[GEO_CUBE], false);
		modelStack.PopMatrix();
		
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
	modelStack.PushMatrix();
	modelStack.Translate(go->getPos().x, go->getPos().y, go->getPos().z );
	modelStack.Scale(3,3,3);
	RenderMesh(meshList[GEO_ENEMY], true);
	modelStack.PopMatrix();
}


void SceneSP3::RenderProps(playMap* map)
{
	glDisable(GL_DEPTH_TEST);
	for (std::vector<GameObject *>::iterator it = map->mapProps.begin(); it != map->mapProps.end(); ++it)
	{
		GameObject *go = (GameObject *)*it;
		if (go->active == false)
			continue;
		if ((go->pos - player1->pos).Length() > 100)
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
			modelStack.Scale(go->scale.x * 3, go->scale.y * 3, go->scale.z);
			RenderMesh(meshList[GEO_ROCK], true);
			modelStack.PopMatrix();
			glDisable(GL_DEPTH_TEST);
		}
		else if (go->type == GameObject::MAP_WATER)
		{
			modelStack.PushMatrix();
			modelStack.Translate(go->pos.x, go->pos.y, go->pos.z);
			modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
			RenderMesh(meshList[GEO_ICE], true);
			modelStack.PopMatrix();
		}
		else if (go->type == GameObject::MAP_MUD)
		{
			modelStack.PushMatrix();
			modelStack.Translate(go->pos.x, go->pos.y, go->pos.z);
			modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
			RenderMesh(meshList[GEO_CUBE], true);
			modelStack.PopMatrix();
		}
	}
	glEnable(GL_DEPTH_TEST);
}

void SceneSP3::Render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//Calculating aspect ratio
	m_worldHeight = 100.f;
	m_worldWidth = m_worldHeight * (float)Application::GetWindowWidth() / Application::GetWindowHeight();

	// Projection matrix : Orthographic Projection
	Mtx44 projection;
	projection.SetToOrtho(0, m_worldWidth, 0, m_worldHeight, -10, 10);
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

	for (std::vector<enemy *>::iterator it = enemyList.begin(); it != enemyList.end(); ++it)
	{
		enemy *go = (enemy *)*it;

		RenderEnemy(go);
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
	modelStack.Translate(mapPosition.x, mapPosition.y, mapPosition.z - 9);
	modelStack.Scale(testMap.getMapSize().x, testMap.getMapSize().y, testMap.getMapSize().z);
	RenderMesh(testMap.getBackground(), false);
	modelStack.PopMatrix();

	RenderProps(&testMap);

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

	std::ostringstream ss11;
	ss11.precision(5);
	ss11 << "count: " << testMap.propCount;
	RenderTextOnScreen(meshList[GEO_TEXT], ss11.str(), Color(0, 1, 0), 3, 0, 6);

	std::ostringstream ss;
	ss.precision(5);
	ss << "Pos: " << worldX << ", " << worldY;
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 3, 0, 3);

	std::ostringstream ssz;
	ssz.precision(5);
	ssz << "FPS: " << fps;
	RenderTextOnScreen(meshList[GEO_TEXT], ssz.str(), Color(0, 1, 0), 3, 0, 0);

	//RenderTextOnScreen(meshList[GEO_TEXT], "Collision", Color(0, 1, 0), 3, 0, 0);
}

void SceneSP3::Exit()
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