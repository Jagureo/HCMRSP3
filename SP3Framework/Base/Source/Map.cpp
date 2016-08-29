#include "Map.h"

playMap::playMap()
{
	mapSize = Vector3(10, 10, 10);
	propCount = 0;
}

playMap::~playMap()
{
	for (int i = 0; i < propCount; i++)
	{
		mapProps.pop_back();
	}
}

void playMap::setMapSize(int x, int y)
{
	mapSize = Vector3(x, y, y);
}

Vector3 playMap::getMapSize()
{
	return mapSize;
}

void playMap::setBackground(Mesh* mesh)
{
	background = mesh;
}

Mesh* playMap::getBackground()
{
	return background;
}

void playMap::addSingleProp(GameObject* newProp)
{
	bool spotFound = false;
	for (std::vector<GameObject *>::iterator it = mapProps.begin(); it != mapProps.end(); ++it)
	{
		GameObject *go = (GameObject *)*it;
		if (go->dead == true)
		{
			spotFound = true;
			go->active = true;
			go->type = newProp->type;
			go->scale = newProp->scale;
			go->pos = newProp->pos;
			go->fresh = newProp->fresh;
			break;
		}
	}
	if (spotFound == false)
	{
		mapProps.push_back(newProp);
		propCount++;
	}
}

void playMap::forceAddSingleProp(GameObject* newProp)
{
	mapProps.push_back(newProp);
	propCount++;
}

void playMap::addClusterProp(GameObject* newProp, int density)
{
	mapProps.push_back(newProp);
	GameObject* cluster = new GameObject(newProp->type);
	for (int i = 0; i < density; i++)
	{
		bool posFound = false;
		for (std::vector<GameObject *>::iterator it = mapProps.begin(); it != mapProps.end(); ++it)
		{
			GameObject *go = (GameObject *)*it;
			if (go->dead == true)
			{
				go->pos.Set(newProp->pos.x + Math::RandFloatMinMax(-25.f, 25.f), newProp->pos.y + Math::RandFloatMinMax(-25.f, 25.f), Math::RandFloatMinMax(-1.f, 1.f));
				go->fresh = true;
				go->type = newProp->type;
				go->scale = newProp->scale;
				go->active = true;
				go->dead = false;
				posFound = true;
				//mapProps.push_back(cluster);
				if (i != density - 1)
					cluster = new GameObject(newProp->type);
				break;
			}
		}
		if (!posFound)
		{
			cluster->pos.Set(newProp->pos.x + Math::RandFloatMinMax(-25.f, 25.f), newProp->pos.y + Math::RandFloatMinMax(-25.f, 25.f), Math::RandFloatMinMax(-1.f, 1.f));
			cluster->fresh = true;
			cluster->scale = newProp->scale;
			cluster->active = true;
			mapProps.push_back(cluster);
			propCount++;
			if (i != density - 1)
				cluster = new GameObject(newProp->type);
		}
		
	}
}

void playMap::optimize()
{
	int i = 0;
	for (std::vector<GameObject *>::iterator it = mapProps.begin(); it != mapProps.end(); ++it)
	{
		GameObject *go = (GameObject *)*it;
		if (go->dead == true)
		{
			continue;
		}
		if (go->active == false)
		{
			continue;
		}
		for (std::vector<GameObject *>::iterator it2 = it + 1; it2 != mapProps.end(); ++it2)
		{
			GameObject *other = (GameObject *)*it2;
			if (go->type != other->type)
			{
				continue;
			}
			if (other->dead == true)
			{
				continue;
			}
			if (other->active == false)
			{
				continue;
			}
			if ((go->pos - other->pos).LengthSquared() < 15.f)
			{
				go->radarVisible = false;
				if ((go->pos - other->pos).LengthSquared() < go->scale.x)
				{
					go->active = false;
					go->dead = true;
					//propCount--;
					break;
				}
			}
		}
		i++;
	}
	i = 0;
	if (mapProps.size() < 2)
		return;
	//else
		//std::cout << mapProps.size() << std::endl;
	for (std::vector<GameObject *>::iterator it = mapProps.begin(); it != mapProps.end();)
	{
		GameObject *go = (GameObject *)*it;
		if (go->dead == false || go->active == true)
			++it;
		else
		{
			std::cout << mapProps.size() << std::endl;
			it = mapProps.erase(it++);
			propCount--;
		}
		i++;
	}
	propCount = mapProps.size();
}

void playMap::addBorder()
{
	for (std::vector<GameObject *>::iterator it = mapBorder.begin(); it != mapBorder.end();)
	{
		GameObject *go = (GameObject *)*it;
		it = mapBorder.erase(it++);
	}

	for (int i = 0; i < mapSize.x * 2.5f; i++)
	{
		GameObject* testTree = new GameObject(GameObject::MAP_TREE);
		testTree->pos.Set(-mapSize.x * 5 + i * 4.f, mapSize.y * 5, 5 + Math::RandFloatMinMax(-1.f, 1.f));
		std::cout << testTree->pos << std::endl;
		testTree->fresh = true;
		testTree->active = true;
		mapBorder.push_back(testTree);

		GameObject* testTree2 = new GameObject(GameObject::MAP_TREE);
		testTree2->pos.Set(-mapSize.x * 5 + i * 4.f, -mapSize.y * 5, 5 + Math::RandFloatMinMax(-1.f, 1.f));
		testTree2->fresh = true;
		testTree2->active = true;
		mapBorder.push_back(testTree2);
	}

	for (int i = 0; i < mapSize.y * 2.5f; i++)
	{
		GameObject* testTree3 = new GameObject(GameObject::MAP_TREE);
		testTree3->pos.Set(-mapSize.x * 5, mapSize.y * 5 - i * 4.f, 5 + Math::RandFloatMinMax(-1.f, 1.f));
		testTree3->fresh = true;
		testTree3->active = true;
		mapBorder.push_back(testTree3);

		GameObject* testTree4 = new GameObject(GameObject::MAP_TREE);
		testTree4->pos.Set(mapSize.x * 5, mapSize.y * 5 - i * 4.f, 5 + Math::RandFloatMinMax(-1.f, 1.f));
		testTree4->fresh = true;
		testTree4->active = true;
		mapBorder.push_back(testTree4);
	}
}