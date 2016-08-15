#ifndef MAP_H
#define MAP_H

#include "Vector3.h"
#include "GameObject.h"
#include "SceneBase.h"
#include "MyMath.h"
#include <vector>

class playMap
{
private:
	Mesh* background;
	Vector3 mapSize;
	int propCount;

public:
	playMap();
	~playMap();

	void setMapSize(int x, int y);
	Vector3 getMapSize();

	void setBackground(Mesh* mesh);
	Mesh* getBackground();

	void addSingleProp(GameObject* newProp);
	void addClusterProp(GameObject* newProp, int density = 25);

	std::vector<GameObject *> mapProps;
};

#endif