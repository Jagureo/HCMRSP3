#include "SceneAsteroid.h"
#include "GL\glew.h"
#include "Application.h"
#include <sstream>

SceneAsteroid::SceneAsteroid()
{
}

SceneAsteroid::~SceneAsteroid()
{
}

void SceneAsteroid::Init()
{
	SceneBase::Init();

	//Physics code here
	m_speed = 1.f;
	
	Math::InitRNG();

	//Exercise 2a: Construct 100 GameObject with type GO_ASTEROID and add into m_goList
	for (int i = 0; i < 100; ++i)
	{
		GameObject *go = new GameObject(GameObject::GO_ASTEROID);
		m_goList.push_back(go);
	}

	//Exercise 2b: Initialize m_lives and m_score
	m_lives = 3;
	m_score = 0;

	fireDebounceTimer = 1.f / firerate;

	m_worldHeight = 100.f;
	m_worldWidth = m_worldHeight * (float)Application::GetWindowWidth() / (float)Application::GetWindowHeight();
																
	//Exercise 2c: Construct m_ship, set active, type, scale and pos
	m_ship = new GameObject(GameObject::GO_SHIP);
	m_ship->active = true;
	m_ship->scale.Set(5, 5, 5);
	m_ship->vel.Set(0, 0, 0);
	m_ship->pos.Set(m_worldWidth / 2, m_worldHeight / 2);
}

GameObject* SceneAsteroid::FetchGO()
{
	//Exercise 3a: Fetch a game object from m_goList and return it
	for (std::vector<GameObject*>::iterator iter = m_goList.begin(); iter != m_goList.end(); ++iter)
	{
		GameObject *go = *iter;
		if (go->active == false)
		{
			go->active = true;
			return go;
		}
	}
	for (unsigned int i = 0; i < 10; ++i)
	{
		m_goList.push_back(new GameObject(GameObject::GO_ASTEROID));
	}
	GameObject *go = m_goList.back();
	go->active = true;
	return go;
}

void SceneAsteroid::Update(double dt)
{
	SceneBase::Update(dt);
	
	if(Application::IsKeyPressed(VK_OEM_MINUS))
	{
		m_speed = Math::Max(0.f, m_speed - 0.1f);
	}
	if(Application::IsKeyPressed(VK_OEM_PLUS))
	{
		m_speed += 0.1f;
	}
	m_force.SetZero();
	//Exercise 6: set m_force values based on WASD
	if(Application::IsKeyPressed('W'))
	{
		m_force.y += 100;
	}
	if(Application::IsKeyPressed('A'))
	{
		m_force.x -= 100;
	}
	if(Application::IsKeyPressed('S'))
	{
		m_force.y -= 100;
	}
	if(Application::IsKeyPressed('D'))
	{
		m_force.x += 100;
	}
	//Exercise 8: use 2 keys to increase and decrease mass of ship
	if (Application::IsKeyPressed('9'))
	{
		m_ship->mass = Math::Max(0.1f, m_ship->mass - 0.1f);
	}
	if (Application::IsKeyPressed('0'))
	{
		m_ship->mass += 0.1f;
	}

	//Exercise 11: use a key to spawn some asteroids
	if (Application::IsKeyPressed('V'))
	{
		GameObject* go = FetchGO();
		go->type = GameObject::GO_ASTEROID;
		go->scale.Set(1, 1, 1);
		go->pos.Set(Math::RandFloatMinMax(0.f, m_worldWidth), Math::RandFloatMinMax(0, m_worldHeight), 0);
		go->vel.Set(Math::RandFloatMinMax(-5, 5), Math::RandFloatMinMax(-5, 5), 0);
	}

	fireDebounceTimer += dt;

	//Exercise 14: use a key to spawn a bullet
	if (Application::IsKeyPressed(VK_SPACE) && fireDebounceTimer > 1.f / firerate)
	{
		fireDebounceTimer = 0;
		GameObject* bullet = FetchGO();
		bullet->type = GameObject::GO_BULLET;
		bullet->scale.Set(0.2f, 0.2f, 0.2f);
		bullet->pos = m_ship->pos;
		if (m_ship->vel.IsZero())
		{
			bullet->vel.Set(1, 0, 0);
		}
		else
		{
			bullet->vel = m_ship->vel.Normalized() * BULLET_SPEED;
		}
	}
	if (Application::IsKeyPressed('M') && fireDebounceTimer > 1.f / firerate)
	{
		fireDebounceTimer = 0;
		GameObject* bullet = FetchGO();
		bullet->type = GameObject::GO_MISSILE;
		bullet->scale.Set(0.5f, 0.5f, 0.5f);
		bullet->pos = m_ship->pos;
		if (m_ship->vel.IsZero())
		{
			bullet->vel.Set(MISSILE_SPEED, 0, 0);
		}
		else
		{
			bullet->vel = m_ship->vel.Normalized() * MISSILE_SPEED;
		}
	}
	//Exercise 15: limit the spawn rate of bullets

	//Mouse Section
	static bool bLButtonState = false;
	if(!bLButtonState && Application::IsMousePressed(0))
	{
		bLButtonState = true;
		std::cout << "LBUTTON DOWN" << std::endl;
	}
	else if(bLButtonState && !Application::IsMousePressed(0))
	{
		bLButtonState = false;
		std::cout << "LBUTTON UP" << std::endl;
	}
	static bool bRButtonState = false;
	if(!bRButtonState && Application::IsMousePressed(1))
	{
		bRButtonState = true;
		std::cout << "RBUTTON DOWN" << std::endl;
	}
	else if(bRButtonState && !Application::IsMousePressed(1))
	{
		bRButtonState = false;
		std::cout << "RBUTTON UP" << std::endl;
	}

	//Physics Simulation Section

	//Exercise 7: Update ship's velocity based on m_force
	Vector3 acceleration(0, 0, 0);
	if (m_ship->mass > Math::EPSILON)
		acceleration = m_force * (1.f / m_ship->mass);
	m_ship->vel += acceleration * dt;
	m_ship->pos += m_ship->vel * dt;

	if (m_ship->vel.LengthSquared() > (MAX_SPEED)* (MAX_SPEED))
	{
		//m_ship->vel = m_ship->vel.Normalized() * MAX_SPEED;
		m_ship->vel.Normalize() *= MAX_SPEED;
	}

	//Exercise 9: wrap ship position if it leaves screen
	if (m_ship->pos.x > m_worldWidth)
		m_ship->pos.x -= m_worldWidth;
	if (m_ship->pos.x < 0)
		m_ship->pos.x += m_worldWidth;
	if (m_ship->pos.y > m_worldHeight)
		m_ship->pos.y -= m_worldHeight;
	if (m_ship->pos.y < 0)
		m_ship->pos.y += m_worldHeight;

	for(std::vector<GameObject *>::iterator it = m_goList.begin(); it != m_goList.end(); ++it)
	{
		GameObject *go = (GameObject *)*it;
		if(go->active)
		{
			go->pos += go->vel * dt;
			//Exercise 12: handle collision between GO_SHIP and GO_ASTEROID using simple distance-based check
			if (go->type == GameObject::GO_ASTEROID)
			{
				if (go->active == true)
				{
					float distanceSquared = (go->pos - m_ship->pos).LengthSquared();
					float combinedRadiusSquared = (go->scale.x + m_ship->scale.x) * (go->scale.x + m_ship->scale.x);
					if (distanceSquared < combinedRadiusSquared)
					{
						go->active = false;
						m_lives--;
						if (m_lives == 0)
						{
							m_lives = 3;
							m_score = 0;
							//your game logic to reset game here
						}
						m_ship->pos.Set(m_worldWidth / 2, m_worldHeight / 2, 0);
						m_ship->vel.SetZero();
						break;
					}
				}

				//Exercise 13: asteroids should wrap around the screen like the ship
				if (go->pos.x > m_worldWidth)
					go->pos.x -= m_worldWidth;
				if (go->pos.x < 0)
					go->pos.x += m_worldWidth;
				if (go->pos.y > m_worldHeight)
					go->pos.y -= m_worldHeight;
				if (go->pos.y < 0)
					go->pos.y += m_worldHeight;
			}

			//Exercise 16: unspawn bullets when they leave screen
			if (go->type == GameObject::GO_BULLET)
			{
				if (go->pos.y < 0 || go->pos.x < 0 || go->pos.y > m_worldHeight || go->pos.x > m_worldWidth)
				{
					go->active = false;
				}
			}

			//Exercise 18: collision check between GO_BULLET and GO_ASTEROID
			if (go->type == GameObject::GO_BULLET)
			{ 
				for (auto other : m_goList)
				{
					if (other->type == GameObject::GO_ASTEROID)
					{
						if (other->active == true)
						{
							float distanceSquared = (other->pos - go->pos).LengthSquared();
							float combinedRadiusSquared = (go->scale.x + other->scale.x) * (go->scale.x + other->scale.x);
							if (distanceSquared < combinedRadiusSquared)
							{
								other->active = false;
								go->active = false;
								m_score += 2;
								break;
							}
						}
					}
				}
			}
			else if (go->type == GameObject::GO_MISSILE)
			{
				float closestDistanceSquared = FLT_MAX;
				GameObject* closestAsteroid = nullptr;
				for (std::vector<GameObject*>::iterator it = m_goList.begin(); it != m_goList.end(); ++it)
				{
					
					GameObject* other = *it;
					if (other->type == GameObject::GO_ASTEROID)
					{
						if (other->active == true)
						{
							float lengthSquared = (go->pos - other->pos).LengthSquared();
							if (lengthSquared < closestDistanceSquared)
							{
								closestDistanceSquared = lengthSquared;
								closestAsteroid = other;
							}
							float combinedRadiusSquared = (go->scale.x + other->scale.x) * (go->scale.x + other->scale.x);
							if (lengthSquared < combinedRadiusSquared)
							{
								other->active = false;
								go->active = false;
								m_score += 1;
								break;
							}
						}
					}
				}
				//after getting closest asteroid
				if (closestAsteroid != nullptr)
				{
					//float angleBetween = Math::RadianToDegree(atan2((go->pos.y - closestAsteroid->pos.y), (go->pos.x - closestAsteroid->pos.x)));
					//Mtx44 rotation;
					//rotation.SetToRotation(angleBetween, 1, 0, 0);
					//Vector3 missileVel = go->vel.Normalize();
					//missileVel = (rotation * missileVel) * MISSILE_SPEED;
					//go->vel.Set(missileVel.x, missileVel.y, missileVel.z);
					Vector3 displacement = (closestAsteroid->pos - go->pos).Normalized();
					go->vel = displacement * MISSILE_SPEED;
				}
			}
			
		}
	}
}


void SceneAsteroid::RenderGO(GameObject *go)
{
	switch(go->type)
	{
	case GameObject::GO_SHIP:
		//Exercise 4a: render a sphere with radius 1
		//modelStack.PushMatrix();
		//modelStack.Translate(go->pos.x, go->pos.y, go->pos.z);
		//modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
		//RenderMesh(meshList[GEO_BALL], false);
		//modelStack.PopMatrix();

		//Exercise 17a: render a ship texture or 3D ship model
		modelStack.PushMatrix();
		modelStack.Translate(go->pos.x, go->pos.y, go->pos.z);
		

		//if (m_ship->vel.LengthSquared() > Math::EPSILON)
		//{
		//	if (m_ship->vel.y > Math::EPSILON)
		//	{
		//		modelStack.Rotate(Math::RadianToDegree(acos(m_ship->vel.Normalized().Dot(Vector3(1, 0, 0)))), 0, 0, 1);
		//	}
		//	else if (m_ship->vel.y < Math::EPSILON)
		//	{
		//		modelStack.Rotate(-Math::RadianToDegree(acos(m_ship->vel.Normalized().Dot(Vector3(1, 0, 0)))), 0, 0, 1);
		//	}
		//}

		modelStack.Rotate(Math::RadianToDegree(atan2(go->vel.y, go->vel.x)), 0, 0, 1);
		modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
		RenderMesh(meshList[GEO_SHIP], false);
		modelStack.PopMatrix();
		//Exercise 17b:	re-orientate the ship with velocity
		break;
	case GameObject::GO_ASTEROID:
		//Exercise 4b: render a cube with length 2
		modelStack.PushMatrix();
		modelStack.Translate(go->pos.x, go->pos.y, go->pos.z);
		modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
		RenderMesh(meshList[GEO_CUBE], false);
		modelStack.PopMatrix();
		break;
	case GameObject::GO_BULLET:
	case GameObject::GO_MISSILE:
		modelStack.PushMatrix();
		modelStack.Translate(go->pos.x, go->pos.y, go->pos.z);
		modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
		RenderMesh(meshList[GEO_BALL], false);
		modelStack.PopMatrix();
		break;
	}
}

void SceneAsteroid::Render()
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

	RenderGO(m_ship);

	for(std::vector<GameObject *>::iterator it = m_goList.begin(); it != m_goList.end(); ++it)
	{
		GameObject *go = (GameObject *)*it;
		if(go->active)
		{
			RenderGO(go);
		}
	}

	//On screen text

	//Exercise 5a: Render m_lives, m_score

	//Exercise 5b: Render position, velocity & mass of ship

	std::ostringstream ss2;
	ss2.precision(3);
	ss2 << "Speed: " << m_speed;
	RenderTextOnScreen(meshList[GEO_TEXT], ss2.str(), Color(0, 1, 0), 3, 0, 6);
	
	std::ostringstream ss;
	ss.precision(5);
	ss << "FPS: " << fps;
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 3, 0, 3);

	ss.str("");
	ss.precision(5);
	ss << "Lives: " << m_lives;
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 3, 0, 9);

	ss.str("");
	ss.precision(5);
	ss << "Score: " << m_score;
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 3, 0, 12);

	ss.str("");
	ss.precision(5);
	ss << "Position: " << m_ship->pos;
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 3, 0, 15);

	ss.str("");
	ss.precision(5);
	ss << "Velocity: " << m_ship->vel;
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 3, 0, 18);

	ss.str("");
	ss.precision(5);
	ss << "Mass: " << m_ship->mass;
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 3, 0, 21);
	
	RenderTextOnScreen(meshList[GEO_TEXT], "Asteroid", Color(0, 1, 0), 3, 0, 0);
}

void SceneAsteroid::Exit()
{
	SceneBase::Exit();
	//Cleanup GameObjects
	while(m_goList.size() > 0)
	{
		GameObject *go = m_goList.back();
		delete go;
		m_goList.pop_back();
	}
	if(m_ship)
	{
		delete m_ship;
		m_ship = NULL;
	}
}
