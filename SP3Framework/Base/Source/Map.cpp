#include "Map.h"

playMap::playMap()
{
	mapSize = Vector3(10, 10, 10);
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
	mapProps.push_back(newProp);
	propCount++;
}

void playMap::addClusterProp(GameObject* newProp, int density)
{
	mapProps.push_back(newProp);
	GameObject* cluster = new GameObject(newProp->type);
	for (int i = 0; i < density; i++)
	{
		cluster->pos.Set(newProp->pos.x + Math::RandFloatMinMax(-25.f, 25.f), newProp->pos.y + Math::RandFloatMinMax(-25.f, 25.f), 1); 
		mapProps.push_back(cluster);
		propCount++;
	}
}