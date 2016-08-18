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
		cluster->pos.Set(newProp->pos.x + Math::RandFloatMinMax(-25.f, 25.f), newProp->pos.y + Math::RandFloatMinMax(-25.f, 25.f), Math::RandFloatMinMax(-1.f, 1.f));
		cluster->fresh = true;
		cluster->active = true;
		mapProps.push_back(cluster);
		if (i != density - 1)
			cluster = new GameObject(newProp->type);
		propCount++;
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
			if ((go->pos - other->pos).LengthSquared() < 5.f)
			{
				go->active = false;
				go->dead = true;
				propCount--;
				break;
			}
		}
		i++;
	}
}