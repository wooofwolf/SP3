#pragma once

#include "SceneBase.h"
#include "Map.h"

#include "Player.h"

class SceneTest : public SceneBase
{
private:
	// Variables
	float m_worldWidth;
	float m_worldHeight;
	Map* map;

	//std::vector<Entity*> EntityList;

	Player* player;

public:
	// Constructor
	SceneTest();
	// Destructor
	~SceneTest();

	virtual void Init();
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();
};

