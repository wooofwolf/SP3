#pragma once

#include "SceneBase.h"
#include "Map.h"
#include "Guns.h"

#include "BaseEnemy.h"
#include "Player.h"

class SceneTest : public SceneBase
{
private:
	// Variables
	float m_worldWidth;
	float m_worldHeight;
	Map* map;

public:
	GunComponent* shotgun;
	GunComponent* minigun;
	GunComponent* activeGun;

	std::vector<Entity*> EntityList;

	Player* player;

	BaseEnemy* baseEnemy;
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

