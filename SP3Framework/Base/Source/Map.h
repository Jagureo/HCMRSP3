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
	

public:
	playMap();
	~playMap();

	void setMapSize(int x, int y);
	Vector3 getMapSize();

	void setBackground(Mesh* mesh);
	Mesh* getBackground();

	void addSingleProp(GameObject* newProp);
	void forceAddSingleProp(GameObject* newProp);
	void addClusterProp(GameObject* newProp, int density = 25);

	void optimize();

	std::vector<GameObject *> mapProps;
	int propCount;
};

#endif