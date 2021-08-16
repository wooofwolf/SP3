#pragma once

#include "SceneBase.h"
#include "Map.h"
#include "Guns.h"

#include "Player.h"

class SceneTest : public SceneBase
{
private:
	// Variables
	float m_worldWidth;
	float m_worldHeight;
	Map* map;
	GunComponent* simpleGun;
	GunComponent* revolver;
	GunComponent* shotgun;
	GunComponent* minigun;
	GunComponent* activeGun;

	std::vector<Entity*> EntityList;

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

